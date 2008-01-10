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

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QObject>
#include <QMap>
#include <QMenu>
#include <QCoreApplication>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>
#include <Qsci/qscilexerbash.h>
#include <Qsci/qscilexerbatch.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexercsharp.h>
#include <Qsci/qscilexercss.h>
#include <Qsci/qscilexerd.h>
#include <Qsci/qscilexerdiff.h>
#include <Qsci/qscilexerhtml.h>
#include <Qsci/qscilexeridl.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qscilexerlua.h>
#include <Qsci/qscilexermakefile.h>
#include <Qsci/qscilexerperl.h>
#include <Qsci/qscilexerpov.h>
#include <Qsci/qscilexerproperties.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerruby.h>
#include <Qsci/qscilexersql.h>
#include <Qsci/qscilexertex.h>


class Highlighter : public QObject
{
    Q_OBJECT

public:
    Highlighter(QsciScintilla *parent, QSettings *settings=0);
    void turnHighlightOff();
    void turnHighlightOn();
	
	bool readCurrentSettings(const char *prefix);
	void writeCurrentSettings(const char *prefix);
    QStringList getAvailableHighlighters();

private:
    bool highlightningIsOn;
    QsciScintilla *parent;
    QMap<int, QFont> fontForStyles;
    QMap<int, QColor> colorForStyles;
	QsciLexer* lexer;
	QSettings *settings;
    QMap<QString, QStringList> mapHighlighternameToExtension;

public slots:
    //! The foreground color for style number \a style is set to \a color.  If
    //! \a style is -1 then the color is set for all styles.
    void setColor(const QColor &color, int style = -1);

    //! The font for style number \a style is set to \a font.  If \a style is
    //! -1 then the font is set for all styles.
    void setFont(const QFont &font, int style = -1);

	//! Sets the lexer that is responsible for the given \a extension.
	int setLexerForExtension( QString extension );

    void setLexerByName( QString lexerName );

	void setHighlighterByAction(QAction* highlighterAction);
};

#endif  // HIGHLIGHTER_H
