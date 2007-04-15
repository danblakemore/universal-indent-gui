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

#include "mainwindow.h"

/*!
    \class MainWindow
    \brief Is the main window of UniversalIndentGUI

    The MainWindow class is responsible for generating and displaying most of the gui elements.
    Its look is set in the file "indentgui.ui". An object for the indent handler is generated here
    and user actions are being controlled. Is responsible for file open dialogs and indenter selection.
 */

/*!
    Constructs the main window.
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // generate gui as it is build in the file "indentgui.ui"
    setupUi(this);

	txtedSourceCode = new QsciScintilla(this);
	hboxLayout1->addWidget(txtedSourceCode);

    txtedSourceCode->setUtf8(true);
    txtedSourceCode->setMarginLineNumbers(1, true);
	txtedSourceCode->setMarginWidth(1, QString("10000") );
	txtedSourceCode->setBraceMatching(txtedSourceCode->SloppyBraceMatch);
	txtedSourceCode->setMatchedBraceForegroundColor( QColor("red") );
	txtedSourceCode->setFolding(QsciScintilla::BoxedTreeFoldStyle);
	txtedSourceCode->setWhitespaceVisibility(QsciScintilla::WsVisible);

    // set the program version, revision and date, which is shown in the main window title and in the about dialog.
    version = "0.5.1 Beta";
    revision = "300";
    QDate buildDate(2007, 3, 21);
    buildDateStr = buildDate.toString("d. MMMM yyyy");

    toolBarWidget = new Ui::toolBarWidget();
    QWidget* helpWidget = new QWidget();
    toolBarWidget->setupUi(helpWidget);
    toolBar->addWidget(helpWidget);
    toolBar->setAllowedAreas( Qt::TopToolBarArea | Qt::BottomToolBarArea );

	// Create or open the settings file.
	settings = new QSettings("./UniversalIndentGUI.ini", QSettings::IniFormat, this);

	highlighter = new CppHighlighter(txtedSourceCode);
	menuSettings->insertMenu(actionSyntax_Highlight, highlighter->getHighlighterMenu() );

    indentHandler = 0;

    isFirstRunOfThisVersion = false;
    loadSettings();

    // Because the language settings are being loaded after creating the highlighter menu
    // the menu has to be retranslated after loading the language settings
    highlighter->retranslate();

    createLanguageMenu();
    createEncodingMenu();

    updateWindowTitle();

    textEditVScrollBar = txtedSourceCode->verticalScrollBar();

    sourceCodeChanged = false;
    scrollPositionChanged = false;
    // Set this true, so the indenter is called at first program start
    indentSettingsChanged = true;
    previewToggled = true;

    // generate about dialog box
    aboutDialog = new AboutDialog(this, version, revision, buildDateStr);

	// generate settings dialog box
	settingsDialog = new SettingsDialog(this);

    updateSourceView();
    txtedSourceCode->setModified(false);

    connect( toolBarWidget->pbOpen_Source_File, SIGNAL(clicked()), this, SLOT(openSourceFileDialog()) );
    connect( actionOpen_Source_File, SIGNAL(activated()), this, SLOT(openSourceFileDialog()) );
    connect( actionSave_Source_File_As, SIGNAL(activated()), this, SLOT(saveasSourceFileDialog()) );
    connect( actionSave_Source_File, SIGNAL(activated()), this, SLOT(saveSourceFile()) );
    connect( actionExportPDF, SIGNAL(activated()), this, SLOT(exportToPDF()) );
    connect( actionExportHTML, SIGNAL(activated()), this, SLOT(exportToHTML()) );

    connect( actionLoad_Indenter_Config_File, SIGNAL(activated()), this, SLOT(openConfigFileDialog()) );
    connect( actionSave_Indenter_Config_File, SIGNAL(activated()), this, SLOT(saveasIndentCfgFileDialog()) );

	connect( actionShowSettings, SIGNAL(activated()), settingsDialog, SLOT(exec()) );

    connect( toolBarWidget->cbLivePreview, SIGNAL(toggled(bool)), this, SLOT(previewTurnedOnOff(bool)) );
    connect( toolBarWidget->cbLivePreview, SIGNAL(toggled(bool)), actionLive_Indent_Preview, SLOT(setChecked(bool)) );
    connect( actionLive_Indent_Preview, SIGNAL(toggled(bool)), toolBarWidget->cbLivePreview, SLOT(setChecked(bool)) );
    connect( toolBarWidget->cbHighlight, SIGNAL(toggled(bool)), this, SLOT(turnHighlightOnOff(bool)) );
    connect( toolBarWidget->cbHighlight, SIGNAL(toggled(bool)), actionSyntax_Highlight, SLOT(setChecked(bool)) );
    connect( actionSyntax_Highlight, SIGNAL(toggled(bool)), toolBarWidget->cbHighlight, SLOT(setChecked(bool)) );
	connect( actionWhiteSpaceVisible, SIGNAL(toggled(bool)), this, SLOT(setWhiteSpaceVisibility(bool)) );

    connect( toolBarWidget->pbExit, SIGNAL(clicked()), this, SLOT(close()));
    connect( actionAbout_UniversalIndentGUI, SIGNAL(activated()), aboutDialog, SLOT(exec()) );
    connect( toolBarWidget->pbAbout, SIGNAL(clicked()), aboutDialog, SLOT(exec()) );

    connect( languageActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(languageChanged(QAction*)) );
    connect( encodingActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(encodingChanged(QAction*)) );

    connect( toolBarWidget->cmbBoxIndenters, SIGNAL(activated(int)), this, SLOT(selectIndenter(int)) );

    connect( txtedSourceCode, SIGNAL(textChanged()), this, SLOT(sourceCodeChangedHelperSlot()) );
}


/*!
    Creates the by \a indenterID selected indent handler object and adds the indent widget to its layout
 */
void MainWindow::selectIndenter(int indenterID) {
    IndentHandler *oldIndentHandler = indentHandler;

    // prevent unnecessary updates if same indenter as current has been selected
    if ( indenterID == currentIndenterID ) {
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    indentHandler = new IndentHandler("./data/", indenterID, this, centralwidget);
    indentHandler->hide();
    vboxLayout->insertWidget(0, indentHandler);
    oldIndentHandler->hide();
    indentHandler->show();
    vboxLayout->removeWidget(oldIndentHandler);
    delete oldIndentHandler;

    // Take care if the selected indenterID is smaller or greater than the number of existing indenters
    if ( indenterID < 0 ) {
        indenterID = 0;
    }
    if ( indenterID >= indentHandler->getAvailableIndenters().count() ) {
        indenterID = indentHandler->getAvailableIndenters().count() - 1;
    }

    toolBarWidget->cmbBoxIndenters->setCurrentIndex(indenterID);
    QObject::connect(indentHandler, SIGNAL(settingsCodeChanged()), this, SLOT(indentSettingsChangedSlot()));

    currentIndenterID = indenterID;
    if ( toolBarWidget->cbLivePreview->isChecked() ) {
        callIndenter();
    }
    previewToggled = true;
    indentSettingsChanged = true;
    updateSourceView();
    QApplication::restoreOverrideCursor();
}

/*!
    Tries to load the by \a filePath defined file and returns its content as QString.
    If the file could not be loaded a error dialog will be shown.
 */
QString MainWindow::loadFile(QString filePath) {

    QFile inSrcFile(filePath);
    QString fileContent = "";

    if ( !inSrcFile.open(QFile::ReadOnly | QFile::Text) ) {
        QMessageBox::warning(NULL, tr("Error opening file"), tr("Cannot read the file ")+"\""+filePath+"\"." );
    }
    else {
        QTextStream inSrcStrm(&inSrcFile);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        inSrcStrm.setCodec( QTextCodec::codecForName(currentEncoding.toAscii()) );
        fileContent = inSrcStrm.readAll();
        QApplication::restoreOverrideCursor();
        inSrcFile.close();

		QFileInfo fileInfo(filePath);
		currentSourceFileExtension = fileInfo.suffix();
		highlighter->setLexerForExtension( currentSourceFileExtension );
    }
    return fileContent;
}

/*!
    Calls the source file open dialog to load a source file for the formatting preview.
    If the file was successfully loaded the indenter will be called to generate the formatted source code.
 */
void MainWindow::openSourceFileDialog() {
    // If the source code file is changed and the shown dialog for saving the file
    // is canceled, also stop opening another source file.
    if ( !maybeSave() ) {
        return;
    }
    QString openedSourceFileContent = "";
    QString fileExtensions = tr("Supported by indenter")+" ("+indentHandler->getPossibleIndenterFileExtensions()+
                             ");;"+tr("All files")+" (*.*)";

    //QString openedSourceFileContent = openFileDialog( tr("Choose source code file"), "./", fileExtensions );
    QString fileName = QFileDialog::getOpenFileName( this, tr("Choose source code file"), currentSourceFile, fileExtensions);

    if (fileName != "") {
        currentSourceFile = fileName;
        QFileInfo fileInfo(fileName);
        currentSourceFileExtension = fileInfo.suffix();

        openedSourceFileContent = loadFile(fileName);
        sourceFileContent = openedSourceFileContent;
        if ( toolBarWidget->cbLivePreview->isChecked() ) {
            callIndenter();
        }
        sourceCodeChanged = true;
        previewToggled = true;
        updateSourceView();
        updateWindowTitle();
        textEditLastScrollPos = 0;
        textEditVScrollBar->setValue( textEditLastScrollPos );

        savedSourceContent = openedSourceFileContent;
        txtedSourceCode->setModified( false );
        setWindowModified( txtedSourceCode->isModified() );
    }
}


/*!
    Calls the source file save as dialog to save a source file under a chosen name.
    If the file already exists and it should be overwritten, a warning is shown before.
 */
bool MainWindow::saveasSourceFileDialog() {
    QString fileExtensions = tr("Supported by indenter")+" ("+indentHandler->getPossibleIndenterFileExtensions()+
                             ");;"+tr("All files")+" (*.*)";

    //QString openedSourceFileContent = openFileDialog( tr("Choose source code file"), "./", fileExtensions );
    QString fileName = QFileDialog::getSaveFileName( this, tr("Save source code file"), currentSourceFile, fileExtensions);

    // Saving has been canceled if the filename is empty
    if ( fileName.isEmpty() ) {
        return false;
    }

    savedSourceContent = txtedSourceCode->text();

    currentSourceFile = fileName;
    QFile::remove(fileName);
    QFile outSrcFile(fileName);
    outSrcFile.open( QFile::ReadWrite | QFile::Text );
    outSrcFile.write( savedSourceContent.toUtf8() );
    outSrcFile.close();

    QFileInfo fileInfo(fileName);
    currentSourceFileExtension = fileInfo.suffix();

    txtedSourceCode->setModified( false );
    setWindowModified( txtedSourceCode->isModified() );

    updateWindowTitle();
    return true;
}


/*!
    Saves the currently shown source code to the last save or opened source file.
    If no source file has been opened, because only the static example has been loaded,
    the save as file dialog will be shown.
 */
bool MainWindow::saveSourceFile() {
    if ( currentSourceFile.isEmpty() ) {
        return saveasSourceFileDialog();
    }
    else {
        QFile::remove(currentSourceFile);
        QFile outSrcFile(currentSourceFile);
        savedSourceContent = txtedSourceCode->text();
        outSrcFile.open( QFile::ReadWrite | QFile::Text );
        outSrcFile.write( savedSourceContent.toUtf8() );
        outSrcFile.close();

        txtedSourceCode->setModified( false );
        setWindowModified( txtedSourceCode->isModified() );
    }
    return true;
}


/*!
    Calls the indenter config file save as dialog to save the config file under a chosen name.
    If the file already exists and it should be overwritten, a warning is shown before.
 */
void MainWindow::saveasIndentCfgFileDialog() {
    QString fileExtensions = tr("All files")+" (*.*)";

    //QString openedSourceFileContent = openFileDialog( tr("Choose source code file"), "./", fileExtensions );
    QString fileName = QFileDialog::getSaveFileName( this, tr("Save indent config file"), indentHandler->getIndenterCfgFile(), fileExtensions);

    if (fileName != "") {
        QFile::remove(fileName);
        QFile outCfgFile(fileName);
        outCfgFile.open( QFile::ReadWrite | QFile::Text );
        outCfgFile.write( indentHandler->getParameterString().toAscii() );
        outCfgFile.close();
    }
}


/*!
    Shows a file open dialog to open an existing config file for the currently selected indenter.
    If the file was successfully opened the indent handler is called to load the settings and update itself.
 */
void MainWindow::openConfigFileDialog() {
    QString configFilePath;

    configFilePath = QFileDialog::getOpenFileName( NULL, tr("Choose indenter config file"), indentHandler->getIndenterCfgFile(), "All files (*.*)" );

    if (configFilePath != "") {
        indentHandler->loadConfigFile(configFilePath);
    }
}

/*!
    Shows a file open dialog with the title \a dialogHeaderStr starting in the directory \a startPath
    and with a file mask defined by \a fileMaskStr. Returns the contents of the file as QString.
 */
QString MainWindow::openFileDialog(QString dialogHeaderStr, QString startPath, QString fileMaskStr) {

    QString fileContent = "";

    QString fileName = QFileDialog::getOpenFileName( NULL, dialogHeaderStr, startPath, fileMaskStr);

    if (fileName != "") {
        fileContent = loadFile(fileName);
    }

    return fileContent;
}

/*!
    Updates the text edit field, which is showing the loaded, and if preview is enabled formatted, source code.
    Reassigns the line numbers and in case of switch between preview and none preview keeps the text field
    at the same line number.
 */
void MainWindow::updateSourceView()
{
    textEditLastScrollPos = textEditVScrollBar->value();

    if ( toolBarWidget->cbLivePreview->isChecked() ) {
        sourceViewContent = sourceFormattedContent;
    }
    else {
        sourceViewContent = sourceFileContent;
    }

    if (previewToggled) {
        disconnect( txtedSourceCode, SIGNAL(textChanged ()), this, SLOT(sourceCodeChangedHelperSlot()) );
		bool textIsModified = txtedSourceCode->isModified();
        txtedSourceCode->setText(sourceViewContent);
		txtedSourceCode->setModified(textIsModified);
        previewToggled = false;
        connect( txtedSourceCode, SIGNAL(textChanged ()), this, SLOT(sourceCodeChangedHelperSlot()) );
    }

    textEditVScrollBar->setValue( textEditLastScrollPos );
}

/*!
    Calls the selected indenter with the currently loaded source code to retrieve the formatted source code.
    The original loaded source code file will not be changed.
 */
void MainWindow::callIndenter() {
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    sourceFormattedContent = indentHandler->callIndenter(sourceFileContent, currentSourceFileExtension);
    //updateSourceView();
    QApplication::restoreOverrideCursor();
}

/*!
    Switches the syntax highlighting corresponding to the value \a turnOn either on or off.
 */
void MainWindow::turnHighlightOnOff(bool turnOn) {
    if ( turnOn ) {
        highlighter->turnHighlightOn();
    }
    else {
        highlighter->turnHighlightOff();
    }
    previewToggled = true;
    updateSourceView();
}


void MainWindow::sourceCodeChangedHelperSlot() {
	QTimer::singleShot(0, this, SLOT(sourceCodeChangedSlot()));
}

/*!
    Is emitted whenever the text inside the source view window changes. Calls the indenter
    to format the changed source code.
 */
void MainWindow::sourceCodeChangedSlot() {
    QChar enteredCharacter;
	int cursorPos,cursorPosAbsolut, cursorLine;
	QString text;

    sourceCodeChanged = true;
    if ( scrollPositionChanged ) {
        scrollPositionChanged = false;
    }

    sourceFileContent = txtedSourceCode->text();
	
    if ( sourceFileContent.count() == 0 ) {
        sourceFileContent += "\n";
    }
    else {
        txtedSourceCode->getCursorPosition(&cursorLine, &cursorPos);
        cursorPosAbsolut = txtedSourceCode->SendScintilla(QsciScintillaBase::SCI_GETCURRENTPOS);
	    text = txtedSourceCode->text(cursorLine);
        if ( cursorPosAbsolut > 0 ) {
            cursorPosAbsolut--;
        }
        if ( cursorPos > 0 ) {
            cursorPos--;
        }
        enteredCharacter = sourceFileContent.at(cursorPosAbsolut);
    }

    if ( toolBarWidget->cbLivePreview->isChecked() ) {
        callIndenter();
        previewToggled = true;
    }
    updateSourceView();

    if ( !enteredCharacter.isNull() ) {
        //const char ch = enteredCharacter.toAscii();

        int saveCursorLine = cursorLine;
        int saveCursorPos = cursorPos;

        bool charFound = false;

        // Search forward
        int lineBreakCounter = 0;
        for ( cursorLine = saveCursorLine; cursorLine-saveCursorLine < 6 && cursorLine < txtedSourceCode->lines(); cursorLine++ ) {
            text = txtedSourceCode->text(cursorLine);
            while ( cursorPos < text.count() && enteredCharacter != text.at(cursorPos)) {
                cursorPos++;
            }
            if ( cursorPos >= text.count() ) {
                cursorPos = 0;
            } 
            else {
                charFound = true;
                break;
            }
        }

        // If foward search did not find the character, search backward
        if ( !charFound ) {
            text = txtedSourceCode->text(saveCursorLine);
            cursorPos = saveCursorPos;
            if ( cursorPos >= text.count() ) {
                cursorPos = text.count() - 1;
            }
            int lineBreakCounter = 0;
            for ( cursorLine = saveCursorLine; saveCursorLine-cursorLine < 6 && cursorLine >= 0; cursorLine-- ) {
                text = txtedSourceCode->text(cursorLine);
                while ( cursorPos >= 0 && enteredCharacter != text.at(cursorPos)) {
                    cursorPos--;
                }
                if ( cursorPos < 0 ) {
                    cursorPos = txtedSourceCode->lineLength(cursorLine-1) - 1;
                } 
                else {
                    charFound = true;
                    break;
                }
            }
        }

        // If the character was found set its new cursor position...
        if ( charFound ) {
            txtedSourceCode->setCursorPosition( cursorLine, cursorPos+1 );
        }
        // ...if it was not found, set the previous cursor position.
        else {
            txtedSourceCode->setCursorPosition( saveCursorLine, saveCursorPos+1 );
        }
    }


    if ( toolBarWidget->cbLivePreview->isChecked() ) {
        sourceCodeChanged = false;
    }

    if ( savedSourceContent == txtedSourceCode->text() ) {
        txtedSourceCode->setModified( false );
        setWindowModified( txtedSourceCode->isModified() );
    }
    else {
        txtedSourceCode->setModified( true );
        setWindowModified( txtedSourceCode->isModified() );
    }
}


/*!
    This slot is called whenever one of the indenter settings are changed.
    It calls the selected indenter if the preview is turned on. If preview
    is not active a flag is set, that the settings have changed.
 */
void MainWindow::indentSettingsChangedSlot() {
    indentSettingsChanged = true;

	int cursorLine, cursorPos;
	txtedSourceCode->getCursorPosition(&cursorLine, &cursorPos);

    if ( toolBarWidget->cbLivePreview->isChecked() ) {
        callIndenter();
        previewToggled = true;

        updateSourceView();
        if (sourceCodeChanged) {
/*            savedCursor = txtedSourceCode->textCursor();
            if ( cursorPos >= txtedSourceCode->text().count() ) {
                cursorPos = txtedSourceCode->text().count() - 1;
            }
            savedCursor.setPosition( cursorPos );
            txtedSourceCode->setTextCursor( savedCursor );
*/
            sourceCodeChanged = false;
        }
        indentSettingsChanged = false;
    }
    else {
        updateSourceView();
    }

    if ( savedSourceContent == txtedSourceCode->text() ) {
        txtedSourceCode->setModified( false );
        setWindowModified( txtedSourceCode->isModified() );
    }
    else {
        txtedSourceCode->setModified( true );
        setWindowModified( txtedSourceCode->isModified() );
    }
}


/*!
    This slot is called whenever the preview button is turned on or off.
    It calls the selected indenter to format the current source code if
    the code has been changed since the last indenter call.
 */
void MainWindow::previewTurnedOnOff(bool turnOn) {
    previewToggled = true;

	int cursorLine, cursorPos;
	txtedSourceCode->getCursorPosition(&cursorLine, &cursorPos);

    if ( turnOn && (indentSettingsChanged || sourceCodeChanged) ) {
        callIndenter();
    }
    updateSourceView();
    if (sourceCodeChanged) {
/*        savedCursor = txtedSourceCode->textCursor();
        if ( cursorPos >= txtedSourceCode->text().count() ) {
            cursorPos = txtedSourceCode->text().count() - 1;
        }
        savedCursor.setPosition( cursorPos );
        txtedSourceCode->setTextCursor( savedCursor );
*/
        sourceCodeChanged = false;
    }
    indentSettingsChanged = false;

    if ( savedSourceContent == txtedSourceCode->text() ) {
        txtedSourceCode->setModified( false );
        setWindowModified( txtedSourceCode->isModified() );
    }
    else {
        txtedSourceCode->setModified( true );
        setWindowModified( txtedSourceCode->isModified() );
    }
}


/*!
    This slot updates the main window title to show the currently opened
    source code filename.
 */
void MainWindow::updateWindowTitle() {
    this->setWindowTitle( "UniversalIndentGUI " + version +" [*]"+ currentSourceFile);
}


/*!
    Opens a dialog to save the current source code as a PDF document.
 */
void MainWindow::exportToPDF() {
    QString fileExtensions = tr("PDF Document")+" (*.pdf)";

    QString fileName = currentSourceFile;
    QFileInfo fileInfo(fileName);
    QString fileExtension = fileInfo.suffix();

    fileName.replace( fileName.length()-fileExtension.length(), fileExtension.length(), "pdf" );
    fileName = QFileDialog::getSaveFileName( this, tr("Export source code file"), fileName, fileExtensions);

    if ( !fileName.isEmpty() ) {
        QsciPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
		printer.printRange(txtedSourceCode);
    }
}


/*!
    Opens a dialog to save the current source code as a HTML document.
 */
void MainWindow::exportToHTML() {
	QString fileExtensions = tr("HTML Document")+" (*.html)";

    QString fileName = currentSourceFile;
    QFileInfo fileInfo(fileName);
    QString fileExtension = fileInfo.suffix();

    fileName.replace( fileName.length()-fileExtension.length(), fileExtension.length(), "html" );
    fileName = QFileDialog::getSaveFileName( this, tr("Export source code file"), fileName, fileExtensions);

    if ( !fileName.isEmpty() ) {
/*        QFile::remove(fileName);
        QFile outSrcFile(fileName);
        outSrcFile.open( QFile::ReadWrite | QFile::Text );
        syntaxHighlightCPP(txtedSourceCode);
        outSrcFile.write( txtedSourceCode->toHtml().toAscii() );
        outSrcFile.close(); */
    }
}


/*!
    Loads the settings for the main application out of the file "UniversalIndentGUI.ini",
    which should exists in the call directory. Settings are for example last selected indenter,
    last loaded config file and so on.
*/
void MainWindow::loadSettings() {
    bool settingsFileExists = true;
    int indenterID;


    // Handle if first run of this version
    // -----------------------------------

    // read the version string from settings if the settings file exists
    if ( settingsFileExists ) {
        QString readVersion = settings->value("UniversalIndentGUI/version", "").toString();
        // if version strings are not equal set first run true.
        if ( readVersion != version ) {
            isFirstRunOfThisVersion = true;
        }
        else {
            isFirstRunOfThisVersion = false;
        }
    }
    else {
        currentSourceFile = "./data/example.cpp";
    }


	// Handle last opened window size
	// ------------------------------

	// read windows last size and position from settings if the settings file exists
	if ( settingsFileExists ) {
		bool maximized = settings->value( "UniversalIndentGUI/maximized", false ).toBool();
		QPoint pos = settings->value( "UniversalIndentGUI/position", QPoint(0, 0) ).toPoint();
		QSize size = settings->value( "UniversalIndentGUI/size", QSize(800, 600) ).toSize();
		resize(size);
		move(pos);
		if ( maximized ) {
			showMaximized();
		}
	}
	else {
		QPoint pos = QPoint(0, 0);
		QSize size = QSize(800, 600);
		resize(size);
		move(pos);
	}


	// Handle last selected file encoding
	// ----------------------------------

	// read last selected encoding for the opened source code file.
	if ( settingsFileExists ) {
		currentEncoding = settings->value( "UniversalIndentGUI/encoding", "UTF-8" ).toString();
	}
	else {
		currentEncoding = "UTF-8";
	}


    // Handle last opened source code file
    // -----------------------------------

	// read if last opened source code file should be loaded on startup
	if ( settingsFileExists ) {
		loadLastSourceCodeFileOnStartup = settings->value("UniversalIndentGUI/loadLastSourceCodeFileOnStartup", true).toBool();
	}
	else {
		loadLastSourceCodeFileOnStartup = true;
	}
	actionAuto_Open_Last_file->setChecked( loadLastSourceCodeFileOnStartup );

	// Only load last source code file if set to do so
	if ( loadLastSourceCodeFileOnStartup ) {
		// read last opened source code file from settings if the settings file exists
		if ( settingsFileExists ) {
			currentSourceFile = settings->value("UniversalIndentGUI/lastSourceCodeFile", "./data/example.cpp").toString();
		}
		else {
			currentSourceFile = "./data/example.cpp";
		}

		// if source file exist load it
		if ( QFile::exists(currentSourceFile) ) {
			QFileInfo fileInfo(currentSourceFile);
			currentSourceFile = fileInfo.absoluteFilePath();
			sourceFileContent = loadFile(currentSourceFile);
		}
		// if no source code file exists make some default settings.
		else {
			QFileInfo fileInfo(currentSourceFile);
			currentSourceFile = fileInfo.absolutePath();
			currentSourceFileExtension = "";
			sourceFileContent = "if(x==\"y\"){x=z;}";
		}
	}
	// if last opened source file should not be loaded make some default settings.
	else {
		QFileInfo fileInfo("./data/example.cpp");
		currentSourceFile = fileInfo.absolutePath();
		currentSourceFileExtension = "";
		sourceFileContent = "";
	}
    savedSourceContent = sourceFileContent;


    // Handle last selected indenter
    // -----------------------------

    // read last selected indenter from settings if the settings file exists
    if ( settingsFileExists ) {
        indenterID = settings->value("UniversalIndentGUI/lastSelectedIndenter", 0).toInt();
    }
    else {
        indenterID = 0;
    }

    indentHandler = new IndentHandler("./data/", indenterID, this, centralwidget);
    vboxLayout->addWidget(indentHandler);

    toolBarWidget->cmbBoxIndenters->addItems( indentHandler->getAvailableIndenters() );

    // Take care if the selected indenterID is smaller or greater than the number of existing indenters
    if ( indenterID < 0 ) {
        indenterID = 0;
    }
    if ( indenterID >= indentHandler->getAvailableIndenters().count() && indenterID != 0 ) {
        indenterID = indentHandler->getAvailableIndenters().count() - 1;
    }

    toolBarWidget->cmbBoxIndenters->setCurrentIndex(indenterID);
    QObject::connect(indentHandler, SIGNAL(settingsCodeChanged()), this, SLOT(indentSettingsChangedSlot()));
    currentIndenterID = indenterID;


	// Handle if white space is set to be visible
	// ------------------------------------------

	// read if indenter parameter tool tips are enabled
	if ( settingsFileExists ) {
		bool whiteSpaceIsVisible = settings->value( "UniversalIndentGUI/whiteSpaceIsVisible", false ).toBool();
		actionWhiteSpaceVisible->setChecked( whiteSpaceIsVisible );
		if ( whiteSpaceIsVisible ) {
			txtedSourceCode->setWhitespaceVisibility(QsciScintilla::WsVisible);
		}
		else {
			txtedSourceCode->setWhitespaceVisibility(QsciScintilla::WsInvisible);
		}
	}
	else {
		actionWhiteSpaceVisible->setChecked( false );
		txtedSourceCode->setWhitespaceVisibility(QsciScintilla::WsInvisible);
	}


    // Handle if indenter parameter tool tips are enabled
    // --------------------------------------------------

    // read if indenter parameter tool tips are enabled
    if ( settingsFileExists ) {
        bool indenterParameterTooltipsEnabled = settings->value("UniversalIndentGUI/indenterParameterTooltipsEnabled", true).toBool();
        actionParameter_Tooltips->setChecked( indenterParameterTooltipsEnabled );
    }
    else {
        actionParameter_Tooltips->setChecked( true );
    }


    // Handle the width of tabs in spaces
    // ----------------------------------

    // read the tab width
    if ( settingsFileExists ) {
        int tabWidth = settings->value("UniversalIndentGUI/tabWidth", 4).toInt();
        txtedSourceCode->setTabWidth(tabWidth);
    }
    else {
        txtedSourceCode->setTabWidth(4);
    }


    // Handle selected language
    // ------------------------

    // read the selected language
    if ( settingsFileExists ) {
        language = settings->value("UniversalIndentGUI/language", "").toString();
    }
    else {
        language = "";
    }

    // if no language was set use the system language
    if ( language.isEmpty() ) {
        language = QLocale::system().name();
        language.truncate(2);
    }

	highlighter->readCurrentSettings("");

    // load the translation file and set it for the application
    translator = new QTranslator();
    translator->load( QString("./translations/universalindent_") + language );
    qApp->installTranslator(translator);
    retranslateUi(this);
    toolBarWidget->retranslateUi(toolBar);
}


/*!
    Saves the settings for the main application to the file "UniversalIndentGUI.ini".
    Settings are for example last selected indenter, last loaded config file and so on.
*/
void MainWindow::saveSettings() {
    QFileInfo fileInfo(currentSourceFile);
    if ( fileInfo.isFile() ) {
        settings->setValue( "UniversalIndentGUI/lastSourceCodeFile", currentSourceFile );
    }
	settings->setValue( "UniversalIndentGUI/loadLastSourceCodeFileOnStartup", actionAuto_Open_Last_file->isChecked() );
    settings->setValue( "UniversalIndentGUI/lastSelectedIndenter", currentIndenterID );
    settings->setValue( "UniversalIndentGUI/indenterParameterTooltipsEnabled", actionParameter_Tooltips->isChecked() );
    settings->setValue( "UniversalIndentGUI/language", language );
	settings->setValue( "UniversalIndentGUI/encoding", currentEncoding );
    settings->setValue( "UniversalIndentGUI/version", version );
	settings->setValue( "UniversalIndentGUI/maximized", isMaximized() );
	if ( !isMaximized() ) {
		settings->setValue( "UniversalIndentGUI/position", pos() );
		settings->setValue( "UniversalIndentGUI/size", size() );
	}
    settings->setValue( "UniversalIndentGUI/whiteSpaceIsVisible", actionWhiteSpaceVisible->isChecked() );
    settings->setValue( "UniversalIndentGUI/tabWidth", txtedSourceCode->tabWidth() );
    highlighter->writeCurrentSettings("");
}


/*!
    Is always called when the program is quit. Calls the saveSettings function before really quits.
*/
void MainWindow::closeEvent( QCloseEvent *event ) {
    if ( maybeSave() ) {
        saveSettings();
        event->accept();
    }
    else {
        event->ignore();
    }
}


/*!
    This function is setup to capture tooltip events. All widgets that are created by the
    indentHandler object and are responsible for indenter parameters are connected with
    this event filter. So depending on the settings the tooltips can be enabled and
    disabled for these widgets.
 */
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if ( event->type() == QEvent::ToolTip) {
        if ( actionParameter_Tooltips->isChecked() ) {
            return QMainWindow::eventFilter(obj, event);
        }
        else {
            //QToolTip::showText( QPoint(100,100) , "Test1");
            return true;
        }
    } else {
        // pass the event on to the parent class
        return QMainWindow::eventFilter(obj, event);
    }
}


/*!
    Is called at application exit and asks whether to save the source code file, if it has been changed.
 */
bool MainWindow::maybeSave()
{
    if ( txtedSourceCode->isModified() ) {
        int ret = QMessageBox::warning(this, tr("Modified code"),
            tr("The source code has been modified.\n"
            "Do you want to save your changes?"),
            QMessageBox::Yes | QMessageBox::Default,
            QMessageBox::No,
            QMessageBox::Cancel | QMessageBox::Escape);
        if (ret == QMessageBox::Yes) {
            return saveSourceFile();
        }
        else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}


/*!
    Searches for available translation files and adds a menu entry for each.
 */
void MainWindow::createLanguageMenu() {
    QString languageShort;
    QStringList languageFileList;
    QAction *languageAction;
    LanguageInfo languageInfo;

    languageActionGroup = new QActionGroup(this);

    // English is the default language. A translation file does not exist but to have a menu entry, added here
    languageFileList << "universalindent_en.qm";

    // Find all translation files in the "translations" directory
    QDir dataDirctory = QDir("./translations");
    languageFileList << dataDirctory.entryList( QStringList("universalindent_*.qm") );

    // Loop for each found translation file
    foreach ( languageShort, languageFileList ) {
        // remove the leading string "universalindent_" from the filename
        languageShort.remove(0,16);
        // remove trailing file extension ".qm"
        languageShort.chop(3);
        languageShort = languageShort.toLower();

        languageInfo.languageShort = languageShort;

        // Identify the language mnemonic and set the full name
        if ( languageShort == "en" ) {
            languageInfo.languageName = tr("English");
        }
        else if ( languageShort == "de" ) {
            languageInfo.languageName = tr("German");
        }
		else if ( languageShort == "tw" ) {
			languageInfo.languageName = tr("Taiwan (Chinese)");
		}
        else {
            languageInfo.languageName = tr("Unknown language mnemonic ") + languageShort;
        }

        languageAction = new QAction(languageInfo.languageName, languageActionGroup);
        languageAction->setStatusTip(languageInfo.languageName + tr(" as user interface language."));
        languageAction->setCheckable(true);

        // if the language selected in the ini file or no ini exists the system locale is
        // equal to the current language mnemonic set this menu entry checked
        if ( languageShort == language ) {
            languageAction->setChecked(true);
        }

        languageInfo.languageAction = languageAction;
        languageInfos.append( languageInfo );
    }

    languageMenu = menuSettings->addMenu( tr("Language") );

    languageMenu->addActions( languageActionGroup->actions() );
}


/*!
    This slot is called whenever a language is selected in the menu. It tries to find the
    corresponding action in the languageInfoList and sets the language.
 */
void MainWindow::languageChanged(QAction* languageAction) {
    LanguageInfo languageInfo;

    // Search for the activated action
    foreach ( LanguageInfo languageInfo, languageInfos ) {
        // Set the new language if found in list
        if ( languageInfo.languageAction == languageAction ) {
            // remove the old translation
            qApp->removeTranslator( translator );

            language = languageInfo.languageShort;

            // load the new translation file and add it to the translation list
            translator->load( QString("./translations/universalindent_") + language );
            qApp->installTranslator( translator );
            retranslateUi(this);
            toolBarWidget->retranslateUi(toolBar);

            // translate the language menu
            languageMenu->setTitle( tr("Language") );
            foreach ( languageInfo, languageInfos ) {
                // Identify the language mnemonic and set the full name
                if ( languageInfo.languageShort == "en" ) {
                    languageInfo.languageName = tr("English");
                }
                else if ( languageInfo.languageShort == "de" ) {
                    languageInfo.languageName = tr("German");
                }
				else if ( languageInfo.languageShort == "tw" ) {
					languageInfo.languageName = tr("Taiwan (Chinese)");
				}
                else {
                    languageInfo.languageName = tr("Unknown language mnemonic ") + languageInfo.languageShort;
                }
                languageInfo.languageAction->setText( languageInfo.languageName );
                languageInfo.languageAction->setStatusTip( languageInfo.languageName + tr(" as user interface language.") );
            }

			// translate the encoding menu
			QStringList encodingsList = QStringList() << "UTF-8" << "UTF-16" << "UTF-16BE" << "UTF-16LE"
				<< "Apple Roman" << "Big5" << "Big5-HKSCS" << "EUC-JP" << "EUC-KR" << "GB18030-0"
				<< "IBM 850" << "IBM 866" << "IBM 874" << "ISO 2022-JP" << "ISO 8859-1" << "ISO 8859-13"
				<< "Iscii-Bng" << "JIS X 0201" << "JIS X 0208" << "KOI8-R" << "KOI8-U" << "MuleLao-1"
				<< "ROMAN8" << "Shift-JIS" << "TIS-620" << "TSCII" << "Windows-1250" << "WINSAMI2";

			encodingMenu->setTitle( tr("Reopen File with other Encoding") );
			QList<QAction *> encodingActionList = encodingActionGroup->actions();
			for ( int i = 0; i < encodingActionList.size(); i++ ) {
				encodingActionList.at(i)->setStatusTip( tr("Reopen the currently opened source code file by using the text encoding scheme ") + encodingsList.at(i) );
			}

			// Translate the about dialog.
            aboutDialog->retranslate();

            // Translate the highlighter menue.
            highlighter->retranslate();
        }
    }
}


/*!
    Creates a menu entry under the settings menu for all available text encodings.
*/
void MainWindow::createEncodingMenu() {
    QAction *encodingAction;
    QString encodingName;

    encodingActionGroup = new QActionGroup(this);

    QStringList encodingsList = QStringList() << "UTF-8" << "UTF-16" << "UTF-16BE" << "UTF-16LE"
            << "Apple Roman" << "Big5" << "Big5-HKSCS" << "EUC-JP" << "EUC-KR" << "GB18030-0"
            << "IBM 850" << "IBM 866" << "IBM 874" << "ISO 2022-JP" << "ISO 8859-1" << "ISO 8859-13"
            << "Iscii-Bng" << "JIS X 0201" << "JIS X 0208" << "KOI8-R" << "KOI8-U" << "MuleLao-1"
            << "ROMAN8" << "Shift-JIS" << "TIS-620" << "TSCII" << "Windows-1250" << "WINSAMI2";

    // Loop for each available encoding
    foreach ( encodingName, encodingsList ) {
            encodingAction = new QAction(encodingName, encodingActionGroup);
            encodingAction->setStatusTip( tr("Reopen the currently opened source code file by using the text encoding scheme ") + encodingName );
            encodingAction->setCheckable(true);
			if ( encodingName == currentEncoding ) {
				encodingAction->setChecked(true);
			}
    }
    encodingMenu = new QMenu( tr("Reopen File with other Encoding") );
    menuFile->insertMenu(actionSave_Source_File, encodingMenu);

    encodingMenu->addActions( encodingActionGroup->actions() );
}


/*!
    This slot is called whenever an encoding is selected in the settings menu.
*/
void MainWindow::encodingChanged(QAction* encodingAction) {
    if ( maybeSave() ) {
        QFile inSrcFile(currentSourceFile);
        QString fileContent = "";

        if ( !inSrcFile.open(QFile::ReadOnly | QFile::Text) ) {
            QMessageBox::warning(NULL, tr("Error opening file"), tr("Cannot read the file ")+"\""+currentSourceFile+"\"." );
        }
        else {
            QTextStream inSrcStrm(&inSrcFile);
            QApplication::setOverrideCursor(Qt::WaitCursor);
            QString encodingName = encodingAction->text();
			currentEncoding = encodingName;
            inSrcStrm.setCodec( QTextCodec::codecForName(encodingName.toAscii()) );
            fileContent = inSrcStrm.readAll();
            QApplication::restoreOverrideCursor();
            inSrcFile.close();
            txtedSourceCode->setText( fileContent );
			txtedSourceCode->setModified(false);
        }
    }
}


/*!
	Is called whenever the white space visibitlity is being changed in the menue.
 */
void MainWindow::setWhiteSpaceVisibility(bool visible) {
	if ( visible ) {
		txtedSourceCode->setWhitespaceVisibility(QsciScintilla::WsVisible);
	}
	else {
		txtedSourceCode->setWhitespaceVisibility(QsciScintilla::WsInvisible);
	}
}
