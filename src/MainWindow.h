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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include  "UiGuiSettings.h"

class UiGuiSettingsDialog;
class AboutDialog;
class AboutDialogGraphicsView;
class UiGuiHighlighter;
class IndentHandler;
class UpdateCheckDialog;
namespace Ui {
	class ToolBarWidget;
	class MainWindowUi;
}

class QLabel;
class QScrollBar;
class QActionGroup;
class QTranslator;

class QsciScintilla;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //! Constructor
    MainWindow(QString file2OpenOnStart = "", QWidget *parent = NULL);
    ~MainWindow() {
        _settings.clear();
    }

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

private:
	Ui::MainWindowUi *_mainWindowForm;

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

    QsciScintilla *_qSciSourceCodeEditor;
    QSharedPointer<UiGuiSettings> _settings;

    QString _currentEncoding;
    QString _sourceFileContent;
    QString _sourceFormattedContent;
    QString _sourceViewContent;
    UiGuiHighlighter *_highlighter;
    QScrollBar *_textEditVScrollBar;
    AboutDialog *_aboutDialog;
    AboutDialogGraphicsView *_aboutDialogGraphicsView;
    UiGuiSettingsDialog *_settingsDialog;
    int _textEditLastScrollPos;
    int _currentIndenterID;
    bool _loadLastSourceCodeFileOnStartup;
    QString _currentSourceFile;
    QString _currentSourceFileExtension;
    QString _savedSourceContent;
    QActionGroup *_encodingActionGroup;
    QActionGroup *_saveEncodedActionGroup;
    QActionGroup *_highlighterActionGroup;
    QTranslator *_uiGuiTranslator;
    QTranslator *_qTTranslator;
    bool _isFirstRunOfThisVersion;

    bool _sourceCodeChanged;
    bool _scrollPositionChanged;
    bool _indentSettingsChanged;
    bool _previewToggled;
    QStringList _encodingsList;

    Ui::ToolBarWidget *_toolBarWidget;
    IndentHandler *_indentHandler;
    UpdateCheckDialog *_updateCheckDialog;
    QLabel *_textEditLineColumnInfoLabel;
};

#endif // MAINWINDOW_H
