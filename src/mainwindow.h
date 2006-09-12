/****************************************************************************
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "arthurwidgets.h"

#include "ui_indentgui.h"
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

class MainWindow : public QMainWindow, private Ui::IndentGui
{
    Q_OBJECT

public:
    //! Constructor
    MainWindow(QWidget *parent = 0);

private:
    QString version;
    QString language;
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
    QTranslator *translator;

    bool sourceCodeChanged;
    bool scrollPositionChanged;
    bool indentSettingsChanged;
    bool previewToggled;

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
};

#endif // MAINWINDOW_H
