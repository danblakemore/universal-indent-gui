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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "arthurwidgets.h"

#include "ui_indentgui.h"
#include "ui_toolBarWidget.h"
#include "aboutdialog.h"
#include "cpphighlighter.h"
#include "indenthandler.h"
#include <QWidget>
#include <QString>
#include <QScrollBar>
#include <QTextCursor>
#include <QFileDialog>
#include <QTextStream>
#include <QPrinter>
#include <QPrintDialog>
#include <QCloseEvent>
#include <QHelpEvent>
#include <QToolTip>
#include <QTranslator>
#include <QLocale>
#include <QTextCodec>
#include <QDate>

class MainWindow : public QMainWindow, private Ui::IndentGui
{
    Q_OBJECT

public:
    //! Constructor
    MainWindow(QWidget *parent = 0);

private:
    QString version;
    QString revision;
    QString buildDateStr;
    QString language;
	QString currentEncoding;
    QString sourceFileContent;
    QString sourceFormattedContent;
    QString sourceViewContent;
    CppHighlighter *highlighter;
    QScrollBar *textEditVScrollBar;
    QScrollBar *textEdit2VScrollBar;
    AboutDialog *aboutDialog;
    int textEditLastScrollPos;
    int currentIndenterID;
    QString currentSourceFile;
    QString currentSourceFileExtension;
    QString savedSourceContent;
    QActionGroup *languageActionGroup;
    QActionGroup *encodingActionGroup;
    QTranslator *translator;
    QMenu *languageMenu;
    QMenu *encodingMenu;
    bool isFirstRunOfThisVersion;

    bool sourceCodeChanged;
    bool scrollPositionChanged;
    bool indentSettingsChanged;
    bool previewToggled;

    Ui::toolBarWidget *toolBarWidget;
    IndentHandler *indentHandler;

    // Stores info about a language, like the full name, mnemonic and a reference to its menu action
    struct LanguageInfo
    {
        QString languageName;
        QString languageShort;
        QAction *languageAction;
    };
    QVector<LanguageInfo> languageInfos;

    //! Tries to load a file and returns its content as QString
    QString loadFile(QString filePath);

    QString openFileDialog(QString dialogHeaderStr, QString startPath, QString fileMaskStr);

    void updateWindowTitle();

    //! Loads the settings for the main program
    void loadSettings();

    //! Saves the settings for the main program
    void saveSettings();

    bool maybeSave();

    void createLanguageMenu();
    void createEncodingMenu();
    void syntaxHighlightCPP( QTextEdit *textEdit );

protected:
    void closeEvent( QCloseEvent *event );
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void openConfigFileDialog();
    void openSourceFileDialog();
    bool saveasSourceFileDialog();
    bool saveSourceFile();
    void saveasIndentCfgFileDialog();
    void callIndenter();
    void updateSourceView();
    void turnHighlightOnOff(bool turnOn);
    void selectIndenter(int indenterID);
    void sourceCodeChangedSlot();
    void indentSettingsChangedSlot();
    void previewTurnedOnOff(bool turnOn);
    void exportToPDF();
    void exportToHTML();
    void languageChanged(QAction *languageAction);
    void encodingChanged(QAction *encodingAction);
};

#endif // MAINWINDOW_H
