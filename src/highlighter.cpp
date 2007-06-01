/***************************************************************************
 *   Copyright (C) 2006-2007 by Thomas Schweitzer                          *
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

#include "highlighter.h"

/*!
    \class Highlighter
    \brief Highlighter used by QTextEdit fields to syntax highlight cpp source code
*/

/*!
    The constructor initializes some regular expressions and keywords to identify cpp tokens
 */
Highlighter::Highlighter(QsciScintilla *parent, QSettings *settings)
: QObject(parent)
{
    this->parent = parent;
    // If a settings file/object is given along with the constructor parameters, use it...
    if (settings) {
	    this->settings = settings;
    }
    // ... else create a new own one.
    else {
        this->settings = new QSettings("./data/highlighter.ini", QSettings::IniFormat, this);
    }
    highlightningIsOn = true;

    mapHighlighternameToExtension["Bash"] = QStringList() << "sh";
    mapHighlighternameToExtension["Batch"] = QStringList() << "bat";
    mapHighlighternameToExtension["C++"] = QStringList() << "c" << "h" << "cpp" << "hpp" << "cxx" << "hxx";
    mapHighlighternameToExtension["C#"] = QStringList() << "cs";
    mapHighlighternameToExtension["CSS"] = QStringList() << "css";
    mapHighlighternameToExtension["D"] = QStringList() << "d";
    mapHighlighternameToExtension["Diff"] = QStringList() << "diff";
    mapHighlighternameToExtension["HTML"] = QStringList() << "html";
    mapHighlighternameToExtension["IDL"] = QStringList() << "idl";
    mapHighlighternameToExtension["Java"] = QStringList() << "java";
    mapHighlighternameToExtension["JavaScript"] = QStringList() << "js";
    mapHighlighternameToExtension["LUA"] = QStringList() << "lua";
    mapHighlighternameToExtension["Makefile"] = QStringList() << "makefile";
    mapHighlighternameToExtension["Perl"] = QStringList() << "perl";
    mapHighlighternameToExtension["POV"] = QStringList() << "pov";
    mapHighlighternameToExtension["Ini"] = QStringList() << "ini";
    mapHighlighternameToExtension["Python"] = QStringList() << "py";
    mapHighlighternameToExtension["Ruby"] = QStringList() << "rub";
    mapHighlighternameToExtension["SQL"] = QStringList() << "sql";
    mapHighlighternameToExtension["TeX"] = QStringList() << "tex";

    createHighlighterMenu();

	lexer = 0;
    // Set default highlighter to C++ highlighter.
    setLexerForExtension( "cpp" );
}


/*!
	Creates a menu entry under the settings menu for all available text encodings.
*/
void Highlighter::createHighlighterMenu() {
	QAction *highlighterAction;
	QString highlighterName;

	highlighterActionGroup = new QActionGroup(this);

	// Loop for each known highlighter
	foreach ( highlighterName, mapHighlighternameToExtension.keys() ) {
		highlighterAction = new QAction(highlighterName, highlighterActionGroup);
		highlighterAction->setStatusTip( tr("Set the syntax highlightning to ") + highlighterName );
		highlighterAction->setCheckable(true);
	}
	//encodingActionGroup->actions().first()->setChecked(true);
	highlighterMenu = new QMenu( tr("Set Syntax Highlighter") );
	highlighterMenu->addActions( highlighterActionGroup->actions() );

	connect( highlighterActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(highlighterChanged(QAction*)) );
}


/*!
    Returns the menu for selecting the highlighter.
 */
QMenu *Highlighter::getHighlighterMenu() {
    return highlighterMenu;
}


/*!
    This slot handles signals coming from selecting another syntax highlighter.
 */
void Highlighter::highlighterChanged(QAction* highlighterAction) {
	QString highlighterName = highlighterAction->text();
    setLexerForExtension( mapHighlighternameToExtension[highlighterName].first() );
    // Need to do this "text update" to update the syntax highlighting. Otherwise highlighting is wrong.
    parent->setText( parent->text() );
}


/*!
    Retranslates the highlighter menu.
 */
void Highlighter::retranslate() {
    if ( highlighterMenu ) {
        highlighterMenu->setTitle( tr("Set Syntax Highlighter") );
        QList<QAction *> actionList = highlighterMenu->actions();
        int i = 0;
        foreach ( QString highlighterName, mapHighlighternameToExtension.keys() ) {
            QAction *highlighterAction = actionList.at(i);
            highlighterAction->setStatusTip( tr("Set the syntax highlightning to ") + highlighterName );
            i++;
        }
    }
}


/*!
    Turns the syntax parser on.
*/
void Highlighter::turnHighlightOn() {
    highlightningIsOn = true;
	parent->setLexer(lexer);
}

/*!
    Turns the syntax parser off.
*/
void Highlighter::turnHighlightOff() {
    highlightningIsOn = false;
	parent->setLexer();
}


/*!
    Read the settings for the current lexer from the settings file.
 */
bool Highlighter::readCurrentSettings( const char *prefix )
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
    Write the settings for the current lexer to the settings file.
 */
void Highlighter::writeCurrentSettings( const char *prefix )
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


void Highlighter::setColor(const QColor &color, int style) {
    colorForStyles[style] = color;
    lexer->setColor( color, style );
}


void Highlighter::setFont(const QFont &font, int style) {
    fontForStyles[style] = font;
    lexer->setFont( font, style );
}


void Highlighter::setLexerForExtension( QString extension ) {
    int indexOfHighlighter = 0;
	extension = extension.toLower();

	if ( lexer ) {
		writeCurrentSettings("");
		delete lexer;
	}

	if ( extension == "cpp" || extension == "hpp" || extension == "c" || extension == "h" || extension == "cxx" || extension == "hxx" ) {
		lexer = new QsciLexerCPP();
        indexOfHighlighter =  mapHighlighternameToExtension.keys().indexOf("C++");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	} 
	else if ( extension == "sh" ) {
		lexer = new QsciLexerBash();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("Bash");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else if ( extension == "bat" ) {
		lexer = new QsciLexerBatch();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("Batch");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else if ( extension == "cs" ) {
		lexer = new QsciLexerCSharp();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("C#");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else if ( extension == "css" ) {
		lexer = new QsciLexerCSS();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("CSS");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else if ( extension == "d" ) {
		lexer = new QsciLexerD();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("D");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else if ( extension == "diff" ) {
		lexer = new QsciLexerDiff();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("Diff");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else if ( extension == "html" ) {
		lexer = new QsciLexerHTML();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("HTML");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else if ( extension == "php" ) {
		lexer = new QsciLexerHTML();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("HTML");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else if ( extension == "idl" ) {
		lexer = new QsciLexerIDL();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("IDL");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else if ( extension == "java" ) {
		lexer = new QsciLexerJava();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("Java");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else if ( extension == "js" ) {
		lexer = new QsciLexerJavaScript();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("JavaScript");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else if ( extension == "lua" ) {
		lexer = new QsciLexerLua();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("LUA");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
    else if ( extension == "makefile" ) {
		lexer = new QsciLexerMakefile();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("Makefile");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else if ( extension == "perl" ) {
		lexer = new QsciLexerPerl();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("Perl");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else if ( extension == "pov" ) {
		lexer = new QsciLexerPOV();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("POV");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else if ( extension == "ini" ) {
		lexer = new QsciLexerProperties();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("Ini");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else if ( extension == "py" ) {
		lexer = new QsciLexerPython();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("Python");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else if ( extension == "rub" ) {
		lexer = new QsciLexerRuby();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("Ruby");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else if ( extension == "sql" ) {
		lexer = new QsciLexerSQL();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("SQL");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else if ( extension == "tex" ) {
		lexer = new QsciLexerTeX();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("TeX");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}
	else {
		lexer = new QsciLexerCPP();
        indexOfHighlighter = mapHighlighternameToExtension.keys().indexOf("C++");
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
	}

    if ( highlightningIsOn ) {
	    parent->setLexer(lexer);
    }
	readCurrentSettings("");
}
