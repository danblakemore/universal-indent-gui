/*
 * DO NOT REMOVE THIS NOTICE
 *
 * PROJECT:   JsDecoder
 * VERSION:   1.0.2
 * COPYRIGHT: (c) 2004-2006 Cezary Tomczak
 * LINK:      http://code.gosu.pl
 * LICENSE:   GPL
 */

function JsDecoder() {
    this.s = ""; /* encoded string */
    this.i = 0;
    this.len = 0;

    this.code = [""];
    this.row = 0;

    this.lvl = 0; /* block level */

    this.lastWord = "";
    this.nextChar = "";
    this.prevChar = "";

    this.switches = []; /* searching on each { and } */

    /* DECODE */
    this.decode = function() {

        this.s = this.s.replace(/(\r\n|\r|\n)/g, "\n");

        this.len = this.s.length;
        while (this.i < this.len) {

            var c = this.s.charAt(this.i);

            if (this.len - 1 == this.i) {
                this.nextChar = "";
            } else {
                this.nextChar = this.s.charAt(this.i + 1);
            }

            switch (c) {

                case "\n": this.linefeed(); break;

                case " ":
                case "\t":
                    this.space();
                    break;

                case "{":  this.blockBracketOn();  break;
                case "}":  this.blockBracketOff(); break;

                case ":":  this.colon();     break;
                case ";":  this.semicolon(); break;

                case "(":  this.bracketOn();        break;
                case ")":  this.bracketOff();       break;
                case "[":  this.squareBracketOn();  break;
                case "]":  this.squareBracketOff(); break;

                case '"':
                case "'":
                    this.quotation(c);
                    break;

                case "/":
                    if ("/" == this.nextChar) {
                        this.lineComment();
                    } else if ("*" == this.nextChar) {
                        this.comment();
                    } else {
                        this.slash();
                    }
                    break;

                case ",":  this.comma(); break;
                case ".":  this.dot(); break;

                case "~":
                case "^":
                    this.symbol1(c);
                    break;

                case "-": case "+": case "*": case "%":
                case "<": case "=": case ">": case "?":
                case ":": case "&": case "|": case "/":
                    this.symbol2(c);
                    break;

                case "!":
                    if ("=" == this.nextChar) {
                        this.symbol2(c);
                    } else {
                        this.symbol1(c);
                    }
                    break;

                default:
                    if (/\w/.test(c)) { this.alphanumeric(c); }
                    else { this.unknown(c); }
                    break;
            }
            if (!/\w/.test(c)) {
                if (c != " " && c != "\t") {
                    this.lastWord = "";
                }
            }
            this.prevChar = c;
            this.i++;
        }
        return this.code.join("\n");
    }

    /* WRITE */
    this.write = function(s) {
        if (0 == this.code[this.row].length) {
            var lvl = ("}" == s ? this.lvl - 1 : this.lvl);
            for (var i = 0; i < lvl; i++) {
                this.code[this.row] += "    ";
            }
            this.code[this.row] += s;
        } else {
            this.code[this.row] += s;
        }
    }
    this.removePrevChar = function()
    {
        if (this.code[this.row].length) {
            this.code[this.row].length = this.code[this.row].length-1;
        }
    }

    /* WRITELINE */
    this.writeLine = function() {
        this.code.push("");
        this.row++;
    }

    /* WRITETAB */
    this.writeTab = function() {
        this.write("    ");
    }

    /* GETCURRENTLINE */
    this.getCurrentLine = function() {
        return this.code[this.row];
    }

    /* ISKEYWORD
     * Check if this.lastWord is a keyword */
    this.isKeyword = function() {
        return this.lastWord && this.keywords.indexOf(this.lastWord) != -1;
    }

    /* LINEFEED */
    this.linefeed = function() {
    }

    /* SPACE
     * Example: "var  asd" (2 spaces) does not change
     * Example: for (var a in b) {}
     * Example: return new test(); */
    this.space = function() {
        if (this.isKeyword()) {
            this.write(" ");
            this.lastWord = "";
        } else {
            var multi = ["in", "new"];
            for (var i = 0; i < multi.length; i++) {
                var isKeywordNext = true;
                for (var j = 0; j < multi[i].length; j++) {
                    if (multi[i][j] != this.s.charAt(this.i + 1 + j)) {
                        isKeywordNext = false;
                        break;
                    }
                }
                if (isKeywordNext) {
                    this.write(" ");
                    this.lastWord = "";
                    break;
                }
            }
        }
    }

    /* BLOCKBRACKETON
     * search for switch */
    this.blockBracketOn = function() {
        if (/^\s*switch\s/.test(this.getCurrentLine())) {
            this.switches.push(this.lvl);
        }
        var currentLine = this.getCurrentLine();
        if (currentLine.length) {
            var lastChar = currentLine[currentLine.length - 1];
            if (lastChar != " " && lastChar != "\t") {
                this.write(" {");
            } else {
                this.write("{");
            }
        } else {
            this.write("{");
        }
        this.writeLine();
        this.lvl++;
    }
    this.lastChar = function()
    {
        var line = this.getCurrentLine();
        if (line.length) return line[line-1];
        else return "notfound";
    }

    /* BLOCKBRACKETOFF
     * search for switch */
    this.blockBracketOff = function()
    {
        if (this.getCurrentLine().length && this.lastChar != ';') {
            this.semicolon();
        }
        this.write("}");
        this.writeLine();
        this.lvl--;
        if (this.switches.length && this.switches[this.switches.length - 1] == this.lvl) {
            var row = this.row - 1;
            var spaces = (this.lvl + 1) * 4;
            var caseRegexp = new RegExp("^\\s{"+spaces+"}case\\s");
            while (row > 0) {
                row--;
                if (/^\s*switch\s/.test(this.code[row])) {
                    break;
                }
                if (!caseRegexp.test(this.code[row])) {
                    this.code[row] = "    " + this.code[row];
                }
            }
            this.switches.pop();
        }
    }

    /* COLON
     * case 6:
     * expr ? stat : stat */
    this.colon = function() {
        if (/^\s*case\s/.test(this.getCurrentLine())) {
            this.write(":");
            this.writeLine();
        } else {
            this.symbol2(":");
        }
    }

    this.isStart = function()
    {
        return this.getCurrentLine().length == 0;
    }
    this.backLine = function()
    {
        if (!this.isStart) {
            throw "backLine() may be called only at the start of the line";
            return;
        }
        this.code.length = this.code.length-1;
        this.row--;
    }

    /* SEMICOLON
     * for statement: for (i = 1; i < len; i++) */
    this.semicolon = function()
    {
        if (this.isStart()) {
            this.backLine();
        }
        this.write(";");
        if (/^\s*for\s/.test(this.getCurrentLine())) {
            this.write(" ");
        } else {
            this.writeLine();
        }
    }

    /* BRACKETON */
    this.bracketOn = function() {
        if (this.isKeyword() && this.prevChar != " " && this.prevChar != "\t") {
            this.write(" (");
        } else {
            this.write("(");
        }
    }

    /* BRACKETOFF */
    this.bracketOff = function() {
        this.write(")");
    }

    /* SQUAREBRACKETON */
    this.squareBracketOn = function() {
        this.write("[");
    }

    /* SQUARE BRACKETOFF */
    this.squareBracketOff = function() {
        this.write("]");
    }

    /* QUOTATION */
    this.quotation = function(quotation) {
        var escaped = false;
        this.write(quotation);
        while (this.i < this.len - 1) {
            this.i++;
            var c = this.s.charAt(this.i);
            if ("\\" == c) {
                escaped = (escaped ? false : true);
            }
            this.write(c);
            if (c == quotation) {
                if (!escaped) {
                    break;
                }
            }
            if ("\\" != c) {
                escaped = false;
            }
        }
    }

    /* LINECOMMENT */
    this.lineComment = function() {
        this.write("//");
        this.i++;
        while (this.i < this.len - 1) {
            this.i++;
            var c = this.s.charAt(this.i);
            if ("\n" == c) {
                this.writeLine();
                break;
            }
            this.write(c);
        }
    }

    /* COMMENT
     * multilines */
    this.comment = function() {
        this.write("/*");
        this.i++;
        var c = "";
        var prevC = "";
        while (this.i < this.len - 1) {
            this.i++;
            prevC = c;
            c = this.s.charAt(this.i);
            if (" " == c || "\t" == c || "\n" == c) {
                if (" " == c) {
                    if (this.getCurrentLine()) {
                        this.write(" ");
                    }
                } else if ("\t" == c) {
                    if (this.getCurrentLine()) {
                        this.writeTab();
                    }
                } else if ("\n" == c) {
                    this.writeLine();
                }
            } else {
                this.write(c);
            }
            if ("/" == c && "*" == prevC) {
                break;
            }
        }
        this.writeLine();
    }

    /* SLASH
     * divisor /= or *\/ (4/5 , a/5)
     * regexp /\w/ (//.test() , var asd = /some/;) */
    this.slash = function()
    {
        var a = (this.lastWord);
        var b = ("*" == this.prevChar);
        if (a || b) {
            if (a) {
                if ("=" == this.nextChar) {
                    this.write(" /");
                } else {
                    this.write(" / ");
                }
            } else if (b) {
                this.write("/ ");
            }
        } else if (')' == this.prevChar) {
            this.write(' / ');
        } else {
            if ("=" == this.prevChar) {
                this.write(" /");
            } else {
                this.write("/");
            }
            var escaped = false;
            while (this.i < this.len - 1) {
                this.i++;
                var c = this.s.charAt(this.i);
                if ("\\" == c) {
                    escaped = (escaped ? false : true);
                }
                this.write(c);
                if ("/" == c) {
                    if (!escaped) {
                        break;
                    }
                }
                if ("\\" != c) {
                    escaped = false;
                }
            }
        }
    }

    /* COMMA
     * function arguments seperator
     * array values seperator
     * object values seperator
     * todo: col > 80 this.writeLine() ? */
    this.comma = function() {
        this.write(", ");
    }

    /* DOT
     * method calling */
    this.dot = function() {
        this.write(".");
    }

    /* SYMBOL 1
     * See: this.symbols1 */
    this.symbol1 = function(c) {
        this.write(c);
    }

    /* SYMBOL 2
     * ++ , --
     * See: this.symbols2 */
    this.symbol2 = function(c) {
        if ("+" == c || "-" == c) {
            if (c == this.nextChar || c == this.prevChar) {
                this.write(c);
                return;
            }
        }
        if (this.symbols2.indexOf(this.prevChar) != -1) {
            if (this.symbols2.indexOf(this.nextChar) != -1) {
                this.write(c);
            } else {
                this.write(c + " ");
            }
        } else {
            if (this.symbols2.indexOf(this.nextChar) != -1) {
                this.write(" " + c);
            } else {
                this.write(" " + c + " ");
            }
        }
    }

    /* ALPHANUMERIC
     * /[a-zA-Z0-9_]/ == /\w/ */
    this.alphanumeric = function(c) {
        if (this.lastWord) {
            this.lastWord += c;
        } else {
            this.lastWord = c;
        }
        this.write(c);
    }

    /* UNKNOWN */
    this.unknown = function(c) {
        //throw "Unknown char: '"+c+"' , this.i = " + this.i;
        this.write(c);
    }

    this.symbols1 = "~!^";
    this.symbols2 = "-+*%<=>?:&|/!";

    this.keywords = ["abstract", "boolean", "break", "byte", "case", "catch", "char", "class",
        "const", "continue", "default", "delete", "do", "double", "else", "extends", "false",
        "final", "finally", "float", "for", "function", "goto", "if", "implements", "import",
        "in", "instanceof", "int", "interface", "long", "native", "new", "null", "package",
        "private", "protected", "public", "return", "short", "static", "super", "switch",
        "synchronized", "this", "throw", "throws", "transient", "true", "try", "typeof", "var",
        "void", "while", "with"];
}

/* Finds the index of the first occurrence of item in the array, or -1 if not found */
if (typeof Array.prototype.indexOf == "undefined") {
    Array.prototype.indexOf = function(item) {
        for (var i = 0; i < this.length; i++) {
            if ((typeof this[i] == typeof item) && (this[i] == item)) {
                return i;
            }
        }
        return -1;
    }
}


var jsdecoder = new JsDecoder();
var formattedCode;

jsdecoder.s = unformattedCode;
formattedCode = jsdecoder.decode();

return formattedCode;
