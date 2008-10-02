/***************************************************************************
 *   Copyright (C) 2006-2008 by Thomas Schweitzer                          *
 *   thomas-schweitzer(at)arcor.de                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 2.0 as   *
 *   published by the Free Software Foundation.                            *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program in the file LICENSE.GPL; if not, write to the *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QtGui>

#include "UiguiHighlighter.h"

#include "SettingsPaths.h"

//! \defgroup grp_EditorComponent All concerning editor widget.

/*!
    \class UiguiHighlighter
    \ingroup grp_EditorComponent
    \brief UiguiHighlighter used for selecting the syntax highlighter/lexer for the QsciScintilla component.
*/

/*!
    \brief The constructor initializes some regular expressions and keywords to identify cpp tokens
 */
UiguiHighlighter::UiguiHighlighter(QsciScintilla *parent)
: QObject(parent)
{
    this->parent = parent;

    // If a "indenters" subdir in the applications binary path exists, use local config files (portable mode)
    if ( SettingsPaths::getPortableMode() ) {
        this->settings = new QSettings(SettingsPaths::getGlobalFilesPath() + "/config/UiGuiSyntaxHighlightConfig.ini", QSettings::IniFormat, this);
    } 
    // ... otherwise use the users application data default dir.
    else {
        this->settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), "UiGuiSyntaxHighlightConfig", this);
    }

    highlightningIsOn = true;

    mapHighlighternameToExtension["Bash"] = QStringList() << "sh";
    mapHighlighternameToExtension["Batch"] = QStringList() << "bat";
    mapHighlighternameToExtension["CMake"] = QStringList() << "cmake";
    mapHighlighternameToExtension["C++"] = QStringList() << "c" << "h" << "cpp" << "hpp" << "cxx" << "hxx";
    mapHighlighternameToExtension["C#"] = QStringList() << "cs";
    mapHighlighternameToExtension["CSS"] = QStringList() << "css";
    mapHighlighternameToExtension["D"] = QStringList() << "d";
    mapHighlighternameToExtension["Diff"] = QStringList() << "diff";
    mapHighlighternameToExtension["Fortran"] = QStringList() << "f" << "for";
    mapHighlighternameToExtension["Fortran77"] = QStringList() << "f77";
    mapHighlighternameToExtension["HTML"] = QStringList() << "html";
    mapHighlighternameToExtension["IDL"] = QStringList() << "idl";
    mapHighlighternameToExtension["Java"] = QStringList() << "java";
    mapHighlighternameToExtension["JavaScript"] = QStringList() << "js";
    mapHighlighternameToExtension["LUA"] = QStringList() << "lua";
    mapHighlighternameToExtension["Makefile"] = QStringList() << "makefile";
    mapHighlighternameToExtension["Pascal"] = QStringList() << "pas";
    mapHighlighternameToExtension["Perl"] = QStringList() << "perl" << "pl" << "pm";
    mapHighlighternameToExtension["PHP"] = QStringList() << "php";
    mapHighlighternameToExtension["PostScript"] = QStringList() << "ps" << "eps" << "pdf" << "ai" << "fh";
    mapHighlighternameToExtension["POV"] = QStringList() << "pov";
    mapHighlighternameToExtension["Ini"] = QStringList() << "ini";
    mapHighlighternameToExtension["Python"] = QStringList() << "py";
    mapHighlighternameToExtension["Ruby"] = QStringList() << "rub" << "rb";
    mapHighlighternameToExtension["SQL"] = QStringList() << "sql";
    mapHighlighternameToExtension["TCL"] = QStringList() << "tcl";
    mapHighlighternameToExtension["TeX"] = QStringList() << "tex";
    mapHighlighternameToExtension["VHDL"] = QStringList() << "vhdl";
    mapHighlighternameToExtension["XML"] = QStringList() << "xml";
    mapHighlighternameToExtension["YAML"] = QStringList() << "yaml";

    lexer = 0;

    // This code is only for testing.
    /*
    foreach(QStringList extensionList, mapHighlighternameToExtension.values() ) {
        setLexerForExtension( extensionList.at(0) );
    }
    */

    // Set default highlighter to C++ highlighter.
    setLexerForExtension( "cpp" );
}


/*!
    \brief Returns the available highlighters as QStringList.
 */
QStringList UiguiHighlighter::getAvailableHighlighters() {
    return mapHighlighternameToExtension.keys();
}


/*!
    \brief This slot handles signals coming from selecting another syntax highlighter.
 */
void UiguiHighlighter::setHighlighterByAction(QAction* highlighterAction) {
	QString highlighterName = highlighterAction->text();
    setLexerForExtension( mapHighlighternameToExtension[highlighterName].first() );
    //TODO: This is really no nice way. How do it better?
    // Need to do this "text update" to update the syntax highlighting. Otherwise highlighting is wrong.
    int scrollPos = parent->verticalScrollBar()->value();
    parent->setText( parent->text() );
    parent->verticalScrollBar()->setValue(scrollPos);
}


/*!
    \brief Turns the syntax parser on.
*/
void UiguiHighlighter::turnHighlightOn() {
    highlightningIsOn = true;
	parent->setLexer(lexer);
    readCurrentSettings("");
}

/*!
    \brief Turns the syntax parser off.
*/
void UiguiHighlighter::turnHighlightOff() {
    highlightningIsOn = false;
	parent->setLexer();
    parent->setFont( QFont("Courier", 10, QFont::Normal) );
    parent->setMarginsFont( QFont("Courier", 10, QFont::Normal) );
}


/*!
    \brief Read the settings for the current lexer from the settings file.
 */
bool UiguiHighlighter::readCurrentSettings( const char *prefix )
{
    bool ok, flag, rc = true;
    int num;
    QString key;

    // Reset lists containing fonts and colors for each style
    fontForStyles.clear();
    colorForStyles.clear();

    // Read the styles.
    for (int i = 0; i < 128; ++i)
    {
        // Ignore invalid styles.
        if ( lexer->description(i).isEmpty() )
            continue;

        key.sprintf( "%s/%s/style%d/", prefix, lexer->language(), i );
		key.replace("+", "p");

        // Read the foreground color.
        ok = settings->contains(key + "color");
        num = settings->value(key + "color", 0).toInt();

        if (ok)
            setColor( QColor((num >> 16) & 0xff, (num >> 8) & 0xff, num & 0xff), i );
        else
            rc = false;

        // Read the end-of-line fill.
        ok = settings->contains(key + "eolfill");
        flag = settings->value(key + "eolfill", false).toBool();

        if (ok)
            lexer->setEolFill( flag, i );
        else
            rc = false;

        // Read the font
        QStringList fdesc;

        ok = settings->contains(key + "font");
        fdesc = settings->value(key + "font").toStringList();

        if (ok && fdesc.count() == 5)
        {
            QFont f;

            f.setFamily(fdesc[0]);
            f.setPointSize(fdesc[1].toInt());
            f.setBold(fdesc[2].toInt());
            f.setItalic(fdesc[3].toInt());
            f.setUnderline(fdesc[4].toInt());

            setFont(f, i);
        }
        else
            rc = false;

        // Read the background color.
        ok = settings->contains(key + "paper");
        num = settings->value(key + "paper", 0).toInt();

        if (ok)
            lexer->setPaper( QColor((num >> 16) & 0xff, (num >> 8) & 0xff, num & 0xff), i );
        else
            rc = false;
    }

    // Read the properties.
    key.sprintf( "%s/%s/properties/", prefix, lexer->language() );

    lexer->refreshProperties();

    return rc;
}


/*!
    \brief Write the settings for the current lexer to the settings file.
 */
void UiguiHighlighter::writeCurrentSettings( const char *prefix )
{
    QString key;

    // Write the styles.
    for (int i = 0; i < 128; ++i) {
        // Ignore invalid styles.
        if ( lexer->description(i).isEmpty() )
            continue;

        int num;
        QColor c;

        key.sprintf( "%s/%s/style%d/", prefix, lexer->language(), i );
		key.replace("+", "p");

		// Write style name
		settings->setValue( key + "", lexer->description(i) );

        // Write the foreground color.
		if ( colorForStyles.contains(i) ) {
			c = colorForStyles[i];
		}
		else {
			c = lexer->color(i);
		}
        num = (c.red() << 16) | (c.green() << 8) | c.blue();

        settings->setValue(key + "color", num);

        // Write the end-of-line fill.
        settings->setValue( key + "eolfill", lexer->eolFill(i) );

        // Write the font
        QStringList fdesc;
        QString fmt("%1");
        QFont f;

		if ( fontForStyles.contains(i) ) {
			f = fontForStyles[i];
		}
		else {
			f = lexer->font(i);
		}

        fdesc += f.family();
        fdesc += fmt.arg( f.pointSize() );

        // The casts are for Borland.
        fdesc += fmt.arg( (int)f.bold() );
        fdesc += fmt.arg( (int)f.italic() );
        fdesc += fmt.arg( (int)f.underline() );

        settings->setValue(key + "font", fdesc);

        // Write the background color.
        c = lexer->paper(i);
        num = (c.red() << 16) | (c.green() << 8) | c.blue();

        settings->setValue(key + "paper", num);
    }
}


/*!
    \brief Sets the \a color for the given \a style.
 */
void UiguiHighlighter::setColor(const QColor &color, int style) {
    colorForStyles[style] = color;
    lexer->setColor( color, style );
}


/*!
    \brief Sets the \a font for the given \a style.
 */
void UiguiHighlighter::setFont(const QFont &font, int style) {
    fontForStyles[style] = font;
    lexer->setFont( font, style );
}


/*!
    \brief Sets the to be used lexer by giving his name.
 */
void UiguiHighlighter::setLexerByName( QString lexerName ) {
    setLexerForExtension( mapHighlighternameToExtension[lexerName].first() );
}


/*!
    \brief Sets the proper highlighter / lexer for the given file \a extension. Returns the index of the used lexer in the list.
 */
int UiguiHighlighter::setLexerForExtension( QString extension ) {
    int indexOfHighlighter = 0;
	extension = extension.toLower();

	if ( lexer ) {
		writeCurrentSettings("");
		delete lexer;
	}

	if ( extension == "cpp" || extension == "hpp" || extension == "c" || extension == "h" || extension == "cxx" || extension == "hxx" ) {
		lexer = new QsciLexerCPP();
	} 
	else if ( extension == "sh" ) {
		lexer = new QsciLexerBash();
	}
	else if ( extension == "bat" ) {
		lexer = new QsciLexerBatch();
	}
    else if ( extension == "cmake" ) {
        lexer = new QsciLexerCMake();
    }
	else if ( extension == "cs" ) {
		lexer = new QsciLexerCSharp();
	}
	else if ( extension == "css" ) {
		lexer = new QsciLexerCSS();
	}
	else if ( extension == "d" ) {
		lexer = new QsciLexerD();
	}
	else if ( extension == "diff" ) {
		lexer = new QsciLexerDiff();
	}
    else if ( extension == "f" || extension == "for" ) {
        lexer = new QsciLexerFortran();
    }
    else if ( extension == "f77" ) {
        lexer = new QsciLexerFortran77();
    }
	else if ( extension == "html" ) {
		lexer = new QsciLexerHTML();
	}
	else if ( extension == "idl" ) {
		lexer = new QsciLexerIDL();
	}
	else if ( extension == "java" ) {
		lexer = new QsciLexerJava();
	}
	else if ( extension == "js" ) {
		lexer = new QsciLexerJavaScript();
	}
	else if ( extension == "lua" ) {
		lexer = new QsciLexerLua();
	}
    else if ( extension == "makefile" ) {
		lexer = new QsciLexerMakefile();
	}
    else if ( extension == "pas" ) {
        lexer = new QsciLexerPascal();
    }
	else if ( extension == "perl" || extension == "pl" || extension == "pm" ) {
		lexer = new QsciLexerPerl();
	}
    else if ( extension == "php" ) {
		lexer = new QsciLexerHTML();
	}
    else if ( extension == "ps" || extension == "eps" || extension == "pdf" || extension == "ai" || extension == "fh") {
        lexer = new QsciLexerPostScript();
    }
	else if ( extension == "pov" ) {
		lexer = new QsciLexerPOV();
	}
	else if ( extension == "ini" ) {
		lexer = new QsciLexerProperties();
	}
	else if ( extension == "py" ) {
		lexer = new QsciLexerPython();
	}
	else if ( extension == "rub" || extension == "rb" ) {
		lexer = new QsciLexerRuby();
	}
	else if ( extension == "sql" ) {
		lexer = new QsciLexerSQL();
	}
    else if ( extension == "tcl" ) {
        lexer = new QsciLexerTCL();
    }
	else if ( extension == "tex" ) {
		lexer = new QsciLexerTeX();
	}
    else if ( extension == "vhdl" ) {
        lexer = new QsciLexerVHDL();
    }
    else if ( extension == "xml" ) {
		lexer = new QsciLexerXML();
	}
    else if ( extension == "yaml" ) {
        lexer = new QsciLexerYAML();
    }
	else {
		lexer = new QsciLexerCPP();
        extension = "cpp";
	}

    // Find the index of the selected lexer.
    indexOfHighlighter = 0;
    while ( !mapHighlighternameToExtension.values().at(indexOfHighlighter).contains(extension) ) {
        indexOfHighlighter++;
    }

    // Set the lexer for the QScintilla widget.
    if ( highlightningIsOn ) {
	    parent->setLexer(lexer);
    }

    // Read the settings for the lexer properties from file.
	readCurrentSettings("");

    return indexOfHighlighter;
}
