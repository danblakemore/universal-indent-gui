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

#include "ui_mainwindow.h"
#include "ui_toolBarWidget.h"
#include "aboutdialog.h"
#include "AboutDialogGraphicsView.h"
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

class MainWindow : public QMainWindow, private Ui::MainWindowUi
{
    Q_OBJECT

public:
    //! Constructor
    MainWindow(QString file2OpenOnStart = "", QWidget *parent = 0);

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
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

	QsciScintilla *txtedSourceCode;
	UiguiSettings *settings;

    QString version;
    QString revision;
    QString buildDateStr;

	QString currentEncoding;
    QString sourceFileContent;
    QString sourceFormattedContent;
    QString sourceViewContent;
    UiguiHighlighter *highlighter;
    QScrollBar *textEditVScrollBar;
    AboutDialog *aboutDialog;
    AboutDialogGraphicsView *aboutDialogGraphicsView;
	UiguiSettingsDialog *settingsDialog;
    int textEditLastScrollPos;
    int currentIndenterID;
	bool loadLastSourceCodeFileOnStartup;
    QString currentSourceFile;
    QString currentSourceFileExtension;
    QString savedSourceContent;
    QActionGroup *encodingActionGroup;
    QActionGroup *saveEncodedActionGroup;
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
    QLabel *textEditLineColumnInfoLabel;

protected:
    void closeEvent( QCloseEvent *event );
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void openSourceFileDialog(QString fileName = "");
    bool saveasSourceFileDialog(QAction *chosenEncodingAction = NULL);
    void saveAsOtherEncoding(QAction *chosenEncodingAction);
    bool saveSourceFile();
    void callIndenter();
    void updateSourceView();
    void turnHighlightOnOff(bool turnOn);
	void setWhiteSpaceVisibility(bool visible);
	void sourceCodeChangedHelperSlot();
    void sourceCodeChangedSlot();
    void indentSettingsChangedSlot();
    void previewTurnedOnOff(bool turnOn);
    void exportToPDF();
    void exportToHTML();
	void languageChanged(int languageIndex);
    void encodingChanged(QAction *encodingAction);
	void numberOfLinesChanged();
    void updateRecentlyOpenedList();
    void openFileFromRecentlyOpenedList(QAction* recentlyOpenedAction);
    void clearRecentlyOpenedList();
	void showAboutDialog();
    void setStatusBarCursorPosInfo(int line, int column);
};

#endif // MAINWINDOW_H
