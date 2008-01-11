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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "arthurwidgets.h"

#include "ui_indentgui.h"
#include "ui_toolBarWidget.h"
#include "aboutdialog.h"
//#include "AboutDialogGraphicsView.h"
#include "uiguisettings.h"
#include "uiguisettingsdialog.h"
#include "highlighter.h"
#include "indenthandler.h"
#include "updatecheckdialog.h"

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

#include <Qsci/qsciscintilla.h>
#include <Qsci/qsciprinter.h>

class MainWindow : public QMainWindow, private Ui::IndentGui
{
    Q_OBJECT

public:
    //! Constructor
    MainWindow(QWidget *parent = 0);

private:
    QString loadFile(QString filePath);
    QString openFileDialog(QString dialogHeaderStr, QString startPath, QString fileMaskStr);
    void updateWindowTitle();
    void loadLastOpenedFile();
    void saveSettings();
    bool maybeSave();
    void createEncodingMenu();
    void createHighlighterMenu();
    bool initApplicationLanguage();
    void initMainWindow();
    void initToolBar();
    void initTextEditor();
    void initSyntaxHighlighter();
    void initIndenter();
    void changeEvent(QEvent *event);

	QsciScintilla *txtedSourceCode;
	UiGuiSettings *settings;

    QString version;
    QString revision;
    QString buildDateStr;
	QString indenterDirctoryStr;
    QString tempDirctoryStr;
    QString settingsDirctoryStr;
	QString currentEncoding;
    QString sourceFileContent;
    QString sourceFormattedContent;
    QString sourceViewContent;
    Highlighter *highlighter;
    QScrollBar *textEditVScrollBar;
    AboutDialog *aboutDialog;
//    AboutDialogGraphicsView *aboutDialogGraphicsView;
	UiGuiSettingsDialog *settingsDialog;
    int textEditLastScrollPos;
    int currentIndenterID;
	bool loadLastSourceCodeFileOnStartup;
    QString currentSourceFile;
    QString currentSourceFileExtension;
    QString savedSourceContent;
    QActionGroup *encodingActionGroup;
    QActionGroup *highlighterActionGroup;
    QTranslator *uiGuiTranslator;
    QTranslator *qTTranslator;
    bool isFirstRunOfThisVersion;

    bool sourceCodeChanged;
    bool scrollPositionChanged;
    bool indentSettingsChanged;
    bool previewToggled;
	QStringList encodingsList;

    Ui::toolBarWidget *toolBarWidget;
    IndentHandler *indentHandler;
    UpdateCheckDialog *updateCheckDialog;

protected:
    void closeEvent( QCloseEvent *event );
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void openConfigFileDialog();
    void openSourceFileDialog(QString fileName = "");
    bool saveasSourceFileDialog();
    bool saveSourceFile();
    void saveasIndentCfgFileDialog();
    void callIndenter();
    void updateSourceView();
    void turnHighlightOnOff(bool turnOn);
	void setWhiteSpaceVisibility(bool visible);
    void selectIndenter(int indenterID);
	void sourceCodeChangedHelperSlot();
    void sourceCodeChangedSlot();
    void indentSettingsChangedSlot();
    void previewTurnedOnOff(bool turnOn);
    void exportToPDF();
    void exportToHTML();
	void languageChanged(int languageIndex);
    void encodingChanged(QAction *encodingAction);
    void highlighterChanged(QAction* highlighterAction);
	void numberOfLinesChanged();
    void createIndenterCallShellScript();
    void updateRecentlyOpenedList();
    void openFileFromRecentlyOpenedList(QAction* recentlyOpenedAction);
    void clearRecentlyOpenedList();
    void showIndenterManual();
};

#endif // MAINWINDOW_H
