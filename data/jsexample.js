function decode() {
var jsdecoder = new JsDecoder();var jscolorizer = new JsColorizer();var code;
jsdecoder.s = document.getElementById("a1").value;
code = jsdecoder.decode();

if (document.all) { document.getElementById("a2").innerText = code; }
else {
    code = code.replace(/&/g, "&amp;"); code = code.replace(/</g, "&lt;");
    code = code.replace(/>/g, "&gt;"); jscolorizer.s = code;
    code = jscolorizer.colorize(); document.getElementById("a2").innerHTML = code;
}
}