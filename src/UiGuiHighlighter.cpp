/***************************************************************************
 *   Copyright (C) 2006-2012 by Thomas Schweitzer                          *
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

#include "UiGuiHighlighter.h"

#include "SettingsPaths.h"

#include <QSettings>
#include <QMenu>
#include <QScrollBar>
#include <QCoreApplication>

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>
#include <Qsci/qscilexerbash.h>
#include <Qsci/qscilexerbatch.h>
#include <Qsci/qscilexercmake.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexercsharp.h>
#include <Qsci/qscilexercss.h>
#include <Qsci/qscilexerd.h>
#include <Qsci/qscilexerdiff.h>
#if ( QSCINTILLA_VERSION >= 0x020300 )
#include <Qsci/qscilexerfortran.h>
#include <Qsci/qscilexerfortran77.h>
#endif
#include <Qsci/qscilexerhtml.h>
#include <Qsci/qscilexeridl.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qscilexerlua.h>
#include <Qsci/qscilexermakefile.h>
#if ( QSCINTILLA_VERSION >= 0x020300 )
#include <Qsci/qscilexerpascal.h>
#endif
#include <Qsci/qscilexerperl.h>
#if ( QSCINTILLA_VERSION >= 0x020300 )
#include <Qsci/qscilexerpostscript.h>
#endif
#include <Qsci/qscilexerpov.h>
#include <Qsci/qscilexerproperties.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerruby.h>
#if ( QSCINTILLA_VERSION >= 0x020400 )
#include <Qsci/qscilexerspice.h>
#endif
#include <Qsci/qscilexersql.h>
#if ( QSCINTILLA_VERSION >= 0x020300 )
#include <Qsci/qscilexertcl.h>
#endif
#include <Qsci/qscilexertex.h>
#if ( QSCINTILLA_VERSION >= 0x020400 )
#include <Qsci/qscilexerverilog.h>
#endif
#include <Qsci/qscilexervhdl.h>
#if ( QSCINTILLA_VERSION >= 0x020300 )
#include <Qsci/qscilexerxml.h>
#include <Qsci/qscilexeryaml.h>
#endif

//! \defgroup grp_EditorComponent All concerning editor widget.

/*!
    \class UiGuiHighlighter
    \ingroup grp_EditorComponent
    \brief UiGuiHighlighter used for selecting the syntax highlighter/lexer for the QsciScintilla component.
*/

/*!
    \brief The constructor initializes some regular expressions and keywords to identify cpp tokens
 */
UiGuiHighlighter::UiGuiHighlighter(QsciScintilla *parent) : QObject(parent) {
    _qsciEditorParent = parent;

    // Create the highlighter _settings object from the UiGuiSyntaxHighlightConfig.ini file.
    _settings = new QSettings(SettingsPaths::getSettingsPath() + "/UiGuiSyntaxHighlightConfig.ini", QSettings::IniFormat, this);

    _highlightningIsOn = true;

    _mapHighlighternameToExtension["Bash"] = QStringList() << "sh";
    _mapHighlighternameToExtension["Batch"] = QStringList() << "bat";
    _mapHighlighternameToExtension["CMake"] = QStringList() << "cmake";
    _mapHighlighternameToExtension["C++"] = QStringList() << "c" << "h" << "cpp" << "hpp" << "cxx" << "hxx";
    _mapHighlighternameToExtension["C#"] = QStringList() << "cs";
    _mapHighlighternameToExtension["CSS"] = QStringList() << "css";
    _mapHighlighternameToExtension["D"] = QStringList() << "d";
    _mapHighlighternameToExtension["Diff"] = QStringList() << "diff";
#if ( QSCINTILLA_VERSION >= 0x020300 )
    _mapHighlighternameToExtension["Fortran"] = QStringList() << "f" << "for" << "f90";
    _mapHighlighternameToExtension["Fortran77"] = QStringList() << "f77";
#endif
    _mapHighlighternameToExtension["HTML"] = QStringList() << "html" << "htm";
    _mapHighlighternameToExtension["IDL"] = QStringList() << "idl";
    _mapHighlighternameToExtension["Java"] = QStringList() << "java";
    _mapHighlighternameToExtension["JavaScript"] = QStringList() << "js";
    _mapHighlighternameToExtension["LUA"] = QStringList() << "lua";
    _mapHighlighternameToExtension["Makefile"] = QStringList() << "makefile";
#if ( QSCINTILLA_VERSION >= 0x020300 )
    _mapHighlighternameToExtension["Pascal"] = QStringList() << "pas";
#endif
    _mapHighlighternameToExtension["Perl"] = QStringList() << "perl" << "pl" << "pm";
    _mapHighlighternameToExtension["PHP"] = QStringList() << "php";
#if ( QSCINTILLA_VERSION >= 0x020300 )
    _mapHighlighternameToExtension["PostScript"] = QStringList() << "ps" << "eps" << "pdf" << "ai" << "fh";
#endif
    _mapHighlighternameToExtension["POV"] = QStringList() << "pov";
    _mapHighlighternameToExtension["Ini"] = QStringList() << "ini";
    _mapHighlighternameToExtension["Python"] = QStringList() << "py";
    _mapHighlighternameToExtension["Ruby"] = QStringList() << "rub" << "rb";
#if ( QSCINTILLA_VERSION >= 0x020400 )
    _mapHighlighternameToExtension["Spice"] = QStringList() << "cir";
#endif
    _mapHighlighternameToExtension["SQL"] = QStringList() << "sql";
#if ( QSCINTILLA_VERSION >= 0x020300 )
    _mapHighlighternameToExtension["TCL"] = QStringList() << "tcl";
#endif
    _mapHighlighternameToExtension["TeX"] = QStringList() << "tex";
#if ( QSCINTILLA_VERSION >= 0x020400 )
    _mapHighlighternameToExtension["Verilog"] = QStringList() << "v" << "vh";
#endif
    _mapHighlighternameToExtension["VHDL"] = QStringList() << "vhdl";
    _mapHighlighternameToExtension["XML"] = QStringList() << "xml";
#if ( QSCINTILLA_VERSION >= 0x020300 )
    _mapHighlighternameToExtension["YAML"] = QStringList() << "yaml";
#endif

    _lexer = NULL;

    // This code is only for testing.
    /*
    foreach(QStringList extensionList, _mapHighlighternameToExtension.values() ) {
        setLexerForExtension( extensionList.at(0) );
    }
    */

    // Set default highlighter to C++ highlighter.
    setLexerForExtension( "cpp" );
}


/*!
    \brief Returns the available highlighters as QStringList.
 */
QStringList UiGuiHighlighter::getAvailableHighlighters() {
    return _mapHighlighternameToExtension.keys();
}


/*!
    \brief This slot handles signals coming from selecting another syntax highlighter.
 */
void UiGuiHighlighter::setHighlighterByAction(QAction* highlighterAction) {
    QString highlighterName = highlighterAction->text();
    setLexerForExtension( _mapHighlighternameToExtension[highlighterName].first() );
    //TODO: This is really no nice way. How do it better?
    // Need to do this "text update" to update the syntax highlighting. Otherwise highlighting is wrong.
    int scrollPos = _qsciEditorParent->verticalScrollBar()->value();
    _qsciEditorParent->setText( _qsciEditorParent->text() );
    _qsciEditorParent->verticalScrollBar()->setValue(scrollPos);
}


/*!
    \brief Turns the syntax parser on.
*/
void UiGuiHighlighter::turnHighlightOn() {
    _highlightningIsOn = true;
    _qsciEditorParent->setLexer(_lexer);
    readCurrentSettings("");
}

/*!
    \brief Turns the syntax parser off.
*/
void UiGuiHighlighter::turnHighlightOff() {
    _highlightningIsOn = false;
    _qsciEditorParent->setLexer();
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    _qsciEditorParent->setFont( QFont("Courier", 10, QFont::Normal) );
    _qsciEditorParent->setMarginsFont( QFont("Courier", 10, QFont::Normal) );
#else
    _qsciEditorParent->setFont( QFont("Monospace", 10, QFont::Normal) );
    _qsciEditorParent->setMarginsFont( QFont("Monospace", 10, QFont::Normal) );
#endif
}


/*!
    \brief Read the settings for the current lexer from the settings file.
 */
//TODO: Refactor this function so that the coding style and variable names suit better.
bool UiGuiHighlighter::readCurrentSettings( const char *prefix ) {
    bool ok, flag, rc = true;
    int num;
    QString key;

    // Reset lists containing fonts and colors for each style
    _fontForStyles.clear();
    _colorForStyles.clear();

    // Read the styles.
    for (int i = 0; i < 128; ++i) {
        // Ignore invalid styles.
        if ( _lexer->description(i).isEmpty() )
            continue;

        key.sprintf( "%s/%s/style%d/", prefix, _lexer->language(), i );
        key.replace("+", "p");

        // Read the foreground color.
        ok = _settings->contains(key + "color");
        num = _settings->value(key + "color", 0).toInt();

        if (ok)
            setColor( QColor((num >> 16) & 0xff, (num >> 8) & 0xff, num & 0xff), i );
        else
            rc = false;

        // Read the end-of-line fill.
        ok = _settings->contains(key + "eolfill");
        flag = _settings->value(key + "eolfill", false).toBool();

        if (ok)
            _lexer->setEolFill( flag, i );
        else
            rc = false;

        // Read the font
        QStringList fdesc;

        ok = _settings->contains(key + "font");
        fdesc = _settings->value(key + "font").toStringList();

        if (ok && fdesc.count() == 5) {
            QFont f;

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
            f.setFamily(fdesc[0]);
#else
            if ( fdesc[0].contains("courier", Qt::CaseInsensitive) )
                f.setFamily("Monospace");
            else
                f.setFamily(fdesc[0]);
#endif
            f.setPointSize(fdesc[1].toInt());
            f.setBold(fdesc[2].toInt());
            f.setItalic(fdesc[3].toInt());
            f.setUnderline(fdesc[4].toInt());

            setFont(f, i);
        }
        else
            rc = false;

        // Read the background color.
        ok = _settings->contains(key + "paper");
        num = _settings->value(key + "paper", 0).toInt();

        if (ok)
            _lexer->setPaper( QColor((num >> 16) & 0xff, (num >> 8) & 0xff, num & 0xff), i );
        else
            rc = false;
    }

    // Read the properties.
    key.sprintf( "%s/%s/properties/", prefix, _lexer->language() );

    _lexer->refreshProperties();

    return rc;
}


/*!
    \brief Write the settings for the current lexer to the settings file.
 */
void UiGuiHighlighter::writeCurrentSettings( const char *prefix ) {
    QString key;

    // Write the styles.
    for (int i = 0; i < 128; ++i) {
        // Ignore invalid styles.
        if ( _lexer->description(i).isEmpty() )
            continue;

        int num;
        QColor c;

        key.sprintf( "%s/%s/style%d/", prefix, _lexer->language(), i );
        key.replace("+", "p");

        // Write style name
        _settings->setValue( key + "", _lexer->description(i) );

        // Write the foreground color.
        if ( _colorForStyles.contains(i) ) {
            c = _colorForStyles[i];
        }
        else {
            c = _lexer->color(i);
        }
        num = (c.red() << 16) | (c.green() << 8) | c.blue();

        _settings->setValue(key + "color", num);

        // Write the end-of-line fill.
        _settings->setValue( key + "eolfill", _lexer->eolFill(i) );

        // Write the font
        QStringList fdesc;
        QString fmt("%1");
        QFont f;

        if ( _fontForStyles.contains(i) ) {
            f = _fontForStyles[i];
        }
        else {
            f = _lexer->font(i);
        }

        fdesc += f.family();
        fdesc += fmt.arg( f.pointSize() );

        // The casts are for Borland.
        fdesc += fmt.arg( (int)f.bold() );
        fdesc += fmt.arg( (int)f.italic() );
        fdesc += fmt.arg( (int)f.underline() );

        _settings->setValue(key + "font", fdesc);

        // Write the background color.
        c = _lexer->paper(i);
        num = (c.red() << 16) | (c.green() << 8) | c.blue();

        _settings->setValue(key + "paper", num);
    }
}


/*!
    \brief Sets the \a color for the given \a style.
 */
void UiGuiHighlighter::setColor(const QColor &color, int style) {
    _colorForStyles[style] = color;
    _lexer->setColor( color, style );
}


/*!
    \brief Sets the \a font for the given \a style.
 */
void UiGuiHighlighter::setFont(const QFont &font, int style) {
    _fontForStyles[style] = font;
    _lexer->setFont( font, style );
}


/*!
    \brief Sets the to be used lexer by giving his name.
 */
void UiGuiHighlighter::setLexerByName( QString lexerName ) {
    setLexerForExtension( _mapHighlighternameToExtension[lexerName].first() );
}


/*!
    \brief Sets the proper highlighter / lexer for the given file \a extension. Returns the index of the used lexer in the list.
 */
int UiGuiHighlighter::setLexerForExtension( QString extension ) {
    int indexOfHighlighter = 0;
    extension = extension.toLower();

    if ( _lexer != NULL ) {
        writeCurrentSettings("");
        delete _lexer;
    }

    if ( extension == "cpp" || extension == "hpp" || extension == "c" || extension == "h" || extension == "cxx" || extension == "hxx" ) {
        _lexer = new QsciLexerCPP();
    }
    else if ( extension == "sh" ) {
        _lexer = new QsciLexerBash();
    }
    else if ( extension == "bat" ) {
        _lexer = new QsciLexerBatch();
    }
    else if ( extension == "cmake" ) {
        _lexer = new QsciLexerCMake();
    }
    else if ( extension == "cs" ) {
        _lexer = new QsciLexerCSharp();
    }
    else if ( extension == "css" ) {
        _lexer = new QsciLexerCSS();
    }
    else if ( extension == "d" ) {
        _lexer = new QsciLexerD();
    }
    else if ( extension == "diff" ) {
        _lexer = new QsciLexerDiff();
    }
#if ( QSCINTILLA_VERSION >= 0x020300 )
    else if ( extension == "f" || extension == "for" || extension == "f90" ) {
        _lexer = new QsciLexerFortran();
    }
    else if ( extension == "f77" ) {
        _lexer = new QsciLexerFortran77();
    }
#endif
    else if ( extension == "html" || extension == "htm" ) {
        _lexer = new QsciLexerHTML();
    }
    else if ( extension == "idl" ) {
        _lexer = new QsciLexerIDL();
    }
    else if ( extension == "java" ) {
        _lexer = new QsciLexerJava();
    }
    else if ( extension == "js" ) {
        _lexer = new QsciLexerJavaScript();
    }
    else if ( extension == "lua" ) {
        _lexer = new QsciLexerLua();
    }
    else if ( extension == "makefile" ) {
        _lexer = new QsciLexerMakefile();
    }
#if ( QSCINTILLA_VERSION >= 0x020300 )
    else if ( extension == "pas" ) {
        _lexer = new QsciLexerPascal();
    }
#endif
    else if ( extension == "perl" || extension == "pl" || extension == "pm" ) {
        _lexer = new QsciLexerPerl();
    }
    else if ( extension == "php" ) {
        _lexer = new QsciLexerHTML();
    }
#if ( QSCINTILLA_VERSION >= 0x020300 )
    else if ( extension == "ps" || extension == "eps" || extension == "pdf" || extension == "ai" || extension == "fh") {
        _lexer = new QsciLexerPostScript();
    }
#endif
    else if ( extension == "pov" ) {
        _lexer = new QsciLexerPOV();
    }
    else if ( extension == "ini" ) {
        _lexer = new QsciLexerProperties();
    }
    else if ( extension == "py" ) {
        _lexer = new QsciLexerPython();
    }
    else if ( extension == "rub" || extension == "rb" ) {
        _lexer = new QsciLexerRuby();
    }
#if ( QSCINTILLA_VERSION >= 0x020400 )
    else if ( extension == "spice?" ) {
        _lexer = new QsciLexerSpice();
    }
#endif
    else if ( extension == "sql" ) {
        _lexer = new QsciLexerSQL();
    }
#if ( QSCINTILLA_VERSION >= 0x020300 )
    else if ( extension == "tcl" ) {
        _lexer = new QsciLexerTCL();
    }
#endif
    else if ( extension == "tex" ) {
        _lexer = new QsciLexerTeX();
    }
#if ( QSCINTILLA_VERSION >= 0x020400 )
    else if ( extension == "vlog?" ) {
        _lexer = new QsciLexerVerilog();
    }
#endif
    else if ( extension == "vhdl" ) {
        _lexer = new QsciLexerVHDL();
    }
    else if ( extension == "xml" ) {
#if ( QSCINTILLA_VERSION >= 0x020300 )
        _lexer = new QsciLexerXML();
#else
        _lexer = new QsciLexerHTML();
#endif
    }
#if ( QSCINTILLA_VERSION >= 0x020300 )
    else if ( extension == "yaml" ) {
        _lexer = new QsciLexerYAML();
    }
#endif
    else {
        _lexer = new QsciLexerCPP();
        extension = "cpp";
    }

    // Find the index of the selected _lexer.
    indexOfHighlighter = 0;
    while ( !_mapHighlighternameToExtension.values().at(indexOfHighlighter).contains(extension) ) {
        indexOfHighlighter++;
    }

    // Set the _lexer for the QScintilla widget.
    if ( _highlightningIsOn ) {
        _qsciEditorParent->setLexer(_lexer);
    }

    // Read the _settings for the _lexer properties from file.
    readCurrentSettings("");

    return indexOfHighlighter;
}
