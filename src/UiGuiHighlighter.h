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

#ifndef UIGUIHIGHLIGHTER_H
#define UIGUIHIGHLIGHTER_H

#include <QObject>
#include <QMap>
#include <QFont>
#include <QColor>

class QAction;
class QSettings;

class QsciScintilla;
class QsciLexer;


class UiGuiHighlighter : public QObject
{
    Q_OBJECT

public:
    UiGuiHighlighter(QsciScintilla *parent);
    void turnHighlightOff();
    void turnHighlightOn();

    bool readCurrentSettings(const char *prefix);
    void writeCurrentSettings(const char *prefix);
    QStringList getAvailableHighlighters();

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

private:
    bool _highlightningIsOn;
    QsciScintilla *_qsciEditorParent;
    QMap<int, QFont> _fontForStyles;
    QMap<int, QColor> _colorForStyles;
    QsciLexer* _lexer;
    QSettings *_settings;
    QMap<QString, QStringList> _mapHighlighternameToExtension;
};

#endif  // UIGUIHIGHLIGHTER_H
