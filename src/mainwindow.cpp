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
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // set the program version, revision and date, which is shown in the main window title and in the about dialog.
    version = "0.5.1 Beta";
    revision = "300";
    QDate buildDate(2007, 3, 21);
    buildDateStr = buildDate.toString("d. MMMM yyyy");

    // Create the settings object, which loads all UiGui settings from a file.
	settings = new UiGuiSettings();

    // Initialize the language of the application.
    initApplicationLanguage();

    // Creates the main window and initializes it.
    initMainWindow();

    // Create toolbar and insert it into the main window.
    initToolBar();

    // Create the text edit component using the QScintilla widget.
    initTextEditor();

    // Create and init the syntax highlighter.
    initSyntaxHighlighter();

    // Create and init the indenter.
    initIndenter();

    // Create some menus.
    createLanguageMenu();
    createEncodingMenu();
    createHighlighterMenu();
	

    // generate about dialog box
    aboutDialog = new AboutDialog(this, version, revision, buildDateStr);
    connect( actionAbout_UniversalIndentGUI, SIGNAL(activated()), aboutDialog, SLOT(exec()) );
    connect( toolBarWidget->pbAbout, SIGNAL(clicked()), aboutDialog, SLOT(exec()) );

	// generate settings dialog box
	settingsDialog = new UiGuiSettingsDialog(this, settings);
    connect( actionShowSettings, SIGNAL(activated()), settingsDialog, SLOT(showDialog()) );

    // Loads the last opened file, if this is enabled in the settings.
    loadLastOpenedFile();

    updateSourceView();
    txtedSourceCode->setModified(false);
}


/*!
    Initializes the main window by creating the main gui and make some settings.
 */
void MainWindow::initMainWindow() {
    // Generate gui as it is build in the file "indentgui.ui"
    setupUi(this);

	// Handle last opened window size
	// ------------------------------
	bool maximized = settings->getValueByName("WindowIsMaximized").toBool();
	QPoint pos = settings->getValueByName("WindowPosition").toPoint();
	QSize size = settings->getValueByName("WindowSize").toSize();
	resize(size);
	move(pos);
	if ( maximized ) {
		showMaximized();
	}

    // Handle if first run of this version
    // -----------------------------------
    QString readVersion = settings->getValueByName("VersionInSettingsFile").toString();
	// If version strings are not equal set first run true.
	if ( readVersion != version ) {
		isFirstRunOfThisVersion = true;
	}
	else {
		isFirstRunOfThisVersion = false;
	}

    // Get last selected file encoding
	// -------------------------------
	currentEncoding = settings->getValueByName("FileEncoding").toString();


    // Register the syntax highlightning setting in the menu to the settings object.
    connect( uiGuiSyntaxHighlightningEnabled, SIGNAL(toggled(bool)), settings, SLOT(handleValueChangeFromExtern(bool)) );
    connect( settings, SIGNAL(syntaxHighlightningEnabled(bool)), uiGuiSyntaxHighlightningEnabled, SLOT(setChecked(bool)) );
    uiGuiSyntaxHighlightningEnabled->setChecked( settings->getValueByName("SyntaxHighlightningEnabled").toBool() );
    // Tell the highlighter if it has to be enabled or disabled.
    connect( settings, SIGNAL(syntaxHighlightningEnabled(bool)), this, SLOT(turnHighlightOnOff(bool)) );

    // Register the load last file setting in the menu to the settings object.
    connect( uiGuiLoadLastOpenedFileOnStartup, SIGNAL(toggled(bool)), settings, SLOT(handleValueChangeFromExtern(bool)) );
    connect( settings, SIGNAL(loadLastOpenedFileOnStartup(bool)), uiGuiLoadLastOpenedFileOnStartup, SLOT(setChecked(bool)) );
    uiGuiLoadLastOpenedFileOnStartup->setChecked( settings->getValueByName("LoadLastOpenedFileOnStartup").toBool() );

    // Register the white space setting in the menu to the settings object.
    connect( uiGuiWhiteSpaceIsVisible, SIGNAL(toggled(bool)), settings, SLOT(handleValueChangeFromExtern(bool)) );
    connect( settings, SIGNAL(whiteSpaceIsVisible(bool)), uiGuiWhiteSpaceIsVisible, SLOT(setChecked(bool)) );
    uiGuiWhiteSpaceIsVisible->setChecked( settings->getValueByName("WhiteSpaceIsVisible").toBool() );
    // Tell the QScintilla editor if it has to show white space.
    connect( settings, SIGNAL(whiteSpaceIsVisible(bool)), this, SLOT(setWhiteSpaceVisibility(bool)) );

    // Connect the remaining menue items.
    connect( actionOpen_Source_File, SIGNAL(activated()), this, SLOT(openSourceFileDialog()) );
    connect( actionSave_Source_File_As, SIGNAL(activated()), this, SLOT(saveasSourceFileDialog()) );
    connect( actionSave_Source_File, SIGNAL(activated()), this, SLOT(saveSourceFile()) );
    connect( actionExportPDF, SIGNAL(activated()), this, SLOT(exportToPDF()) );
    connect( actionExportHTML, SIGNAL(activated()), this, SLOT(exportToHTML()) );
    connect( actionLoad_Indenter_Config_File, SIGNAL(activated()), this, SLOT(openConfigFileDialog()) );
    connect( actionSave_Indenter_Config_File, SIGNAL(activated()), this, SLOT(saveasIndentCfgFileDialog()) );

    // Init of some variables.
    dataDirctoryStr = "./data/";
    sourceCodeChanged = false;
    scrollPositionChanged = false;
}


/*!
    Creates and inits the tool bar. It is added to the main window.
 */
void MainWindow::initToolBar() {
    // Create the tool bar and add it to the main window.
    toolBarWidget = new Ui::toolBarWidget();
    QWidget* helpWidget = new QWidget();
    toolBarWidget->setupUi(helpWidget);
    toolBar->addWidget(helpWidget);
    toolBar->setAllowedAreas( Qt::TopToolBarArea | Qt::BottomToolBarArea );

    // Connect the tool bar widgets to their functions.
    connect( toolBarWidget->uiGuiSyntaxHighlightningEnabled, SIGNAL(toggled(bool)), settings, SLOT(handleValueChangeFromExtern(bool)) );
    connect( settings, SIGNAL(syntaxHighlightningEnabled(bool)), toolBarWidget->uiGuiSyntaxHighlightningEnabled, SLOT(setChecked(bool)) );
    toolBarWidget->uiGuiSyntaxHighlightningEnabled->setChecked( settings->getValueByName("SyntaxHighlightningEnabled").toBool() );
    toolBarWidget->uiGuiSyntaxHighlightningEnabled->hide();
    connect( toolBarWidget->pbOpen_Source_File, SIGNAL(clicked()), this, SLOT(openSourceFileDialog()) );
    connect( toolBarWidget->pbExit, SIGNAL(clicked()), this, SLOT(close()));
    connect( toolBarWidget->cmbBoxIndenters, SIGNAL(activated(int)), this, SLOT(selectIndenter(int)) );
    connect( toolBarWidget->cbLivePreview, SIGNAL(toggled(bool)), this, SLOT(previewTurnedOnOff(bool)) );
    connect( toolBarWidget->cbLivePreview, SIGNAL(toggled(bool)), actionLive_Indent_Preview, SLOT(setChecked(bool)) );
    connect( actionLive_Indent_Preview, SIGNAL(toggled(bool)), toolBarWidget->cbLivePreview, SLOT(setChecked(bool)) );
}


/*!
   Create and initialize the text editor component. It uses the QScintilla widget.
 */
void MainWindow::initTextEditor() {
    // Create the QScintilla widget and add it to the layout.
    txtedSourceCode = new QsciScintilla(this);
	hboxLayout1->addWidget(txtedSourceCode);

    // Make some settings for the QScintilla widget.
    txtedSourceCode->setUtf8(true);
    txtedSourceCode->setMarginLineNumbers(1, true);
	txtedSourceCode->setMarginWidth(1, QString("10000") );
	txtedSourceCode->setBraceMatching(txtedSourceCode->SloppyBraceMatch);
	txtedSourceCode->setMatchedBraceForegroundColor( QColor("red") );
	txtedSourceCode->setFolding(QsciScintilla::BoxedTreeFoldStyle);
	txtedSourceCode->setAutoCompletionSource(QsciScintilla::AcsAll);
	txtedSourceCode->setAutoCompletionThreshold(3);

    // Handle if white space is set to be visible
	bool whiteSpaceIsVisible = settings->getValueByName("WhiteSpaceIsVisible").toBool();
    setWhiteSpaceVisibility( whiteSpaceIsVisible );

    // Handle the width of tabs in spaces
    int tabWidth = settings->getValueByName("TabWidth").toInt();
    txtedSourceCode->setTabWidth(tabWidth);

    // Remember a pointer to the scrollbar of the QScintilla widget used to keep
    // on the same line as before when turning preview on/off.
    textEditVScrollBar = txtedSourceCode->verticalScrollBar();

    // Connect the text editor to dependent functions.
    connect( txtedSourceCode, SIGNAL(textChanged()), this, SLOT(sourceCodeChangedHelperSlot()) );
	connect( txtedSourceCode, SIGNAL(linesChanged()), this, SLOT(numberOfLinesChanged()) );
    connect( settings, SIGNAL(tabWidth(int)), txtedSourceCode, SLOT(setTabWidth(int)) );
    txtedSourceCode->setTabWidth( settings->getValueByName("TabWidth").toInt() );
}


/*!
    Create and init the syntax highlighter and set it to use the QScintilla edit component.
 */
void MainWindow::initSyntaxHighlighter() {
    // Create the highlighter.
    highlighter = new Highlighter(txtedSourceCode);

    // Handle if syntax highlightning is enabled
	bool syntaxHighlightningEnabled = settings->getValueByName("SyntaxHighlightningEnabled").toBool();
	if ( syntaxHighlightningEnabled ) {
        highlighter->turnHighlightOn();
    }
    else {
        highlighter->turnHighlightOff();
    }
}


/*!
    If the program language is defined in the settings, the corresponding language
    file will be loaded and set for the application. If not set there, the system
    default language will be set, if a translation file for that language exists.
    Returns true, if the translation file could be loaded. Otherwise it returns
    false and uses the default language, which is English.
 */
bool MainWindow::initApplicationLanguage() {
    QString languageShort;
    // Create a translator
    translator = new QTranslator();

    // Get the language settings from the settings object.
	int languageIndex = settings->getValueByName("Language").toInt();

    // If no language was set, indicated by a negative index, use the system language.
    if ( languageIndex < 0 ) {
        languageShort = QLocale::system().name();
        languageShort.truncate(2);

		// In case of Chinese language choose the country code to differ more specific whether traditional or simplified Chinese.
		if ( languageShort == "zh" ) {
			 languageShort = QLocale::system().name().right(2).toLower();
		}

        // If no translation file for the systems local language exist, fall back to English.
        if ( settings->getAvailableTranslations().indexOf(languageShort) < 0 ) {
            languageShort = "en";
        }
        // Set the language setting to the new language.
        settings->setValueByName("Language", settings->getAvailableTranslations().indexOf(languageShort) );
    }
    // If a language was defined in the settings, get this language mnemonic.
    else {
        languageShort = settings->getAvailableTranslations().at(languageIndex);
    }

    // Load the translation file and set it for the application.
    translator = new QTranslator();
    bool translationFileLoaded = translator->load( QString("./translations/universalindent_") + languageShort );
    if ( translationFileLoaded ) {
        qApp->installTranslator(translator);
    }

    return translationFileLoaded;
}


/*!
    Creates and initializes the indenter.
 */
void MainWindow::initIndenter() {
    // Get Id of last selected indenter.
	currentIndenterID = settings->getValueByName("LastSelectedIndenterID").toInt();

    // Create the indenter widget with the ID and add it to the layout.
    indentHandler = new IndentHandler(dataDirctoryStr, currentIndenterID, this, centralwidget);
    vboxLayout->addWidget(indentHandler);

    // Check whether indenters are available.
	if ( !indentHandler->getAvailableIndenters().isEmpty() ) {
		toolBarWidget->cmbBoxIndenters->addItems( indentHandler->getAvailableIndenters() );
		// Take care if the selected indenterID is greater than the number of existing indenters
		if ( currentIndenterID >= indentHandler->getAvailableIndenters().count() ) {
			currentIndenterID = indentHandler->getAvailableIndenters().count() - 1;
		}
	}
    // If no indenter are found, show a warning message.
	else {
		currentIndenterID = 0;
		QMessageBox::warning(NULL, tr("No indenter ini files"), tr("There exists no indenter ini files in the directory \"") + QDir(dataDirctoryStr).absolutePath() + "\".");
	}

    // Set the combobox in the toolbar to show the selected indenter.
    toolBarWidget->cmbBoxIndenters->setCurrentIndex( currentIndenterID );

    // If settings for the indenter have changed, let the main window know aboud it.
    connect(indentHandler, SIGNAL(indenterSettingsChanged()), this, SLOT(indentSettingsChangedSlot()));

    // Set this true, so the indenter is called at first program start
    indentSettingsChanged = true;
    previewToggled = true;

    // Handle if indenter parameter tool tips are enabled
    connect( uiGuiIndenterParameterTooltipsEnabled, SIGNAL(toggled(bool)), settings, SLOT(handleValueChangeFromExtern(bool)) );
    connect( settings, SIGNAL(indenterParameterTooltipsEnabled(bool)), uiGuiIndenterParameterTooltipsEnabled, SLOT(setChecked(bool)) );
    uiGuiIndenterParameterTooltipsEnabled->setChecked( settings->getValueByName("IndenterParameterTooltipsEnabled").toBool() );
}


/*!
    Creates the by \a indenterID selected indent handler object and adds the indent widget to its layout.
 */
void MainWindow::selectIndenter(int indenterID) {
    IndentHandler *oldIndentHandler = indentHandler;

    // Prevent unnecessary updates if same indenter as current has been selected
    if ( indenterID == currentIndenterID ) {
        return;
    }

    // Disconnect the old indent handler from the settings changed slot, because he will be deleted.
    disconnect(oldIndentHandler, SIGNAL(indenterSettingsChanged()), this, SLOT(indentSettingsChangedSlot()));

    QApplication::setOverrideCursor(Qt::WaitCursor);

    indentHandler = new IndentHandler(dataDirctoryStr, indenterID, this, centralwidget);
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

    // Set the combobox in the toolbar to show the selected indenter.
    toolBarWidget->cmbBoxIndenters->setCurrentIndex(indenterID);

    // If settings for the indenter have changed, let the main window know aboud it.
    connect(indentHandler, SIGNAL(indenterSettingsChanged()), this, SLOT(indentSettingsChangedSlot()));

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


/*!
    Added this slot to avoid multiple calls because of changed text.
 */
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

    // Could set cursor this way and use normal linear search in text instead of columns and rows.
    //txtedSourceCode->SendScintilla(QsciScintillaBase::SCI_SETCURRENTPOS, 50);
    //txtedSourceCode->SendScintilla(QsciScintillaBase::SCI_SETANCHOR, 50);
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
    Loads the last opened file if this option is enabled in the settings. If the file
    does not exist, the default example file is tried to be loaded. If even that
    fails a very small code example is shown.
    If the setting for opening the last file is disabled, the editor is empty on startup.
*/
void MainWindow::loadLastOpenedFile() {
    // Get setting for last opened source code file
	loadLastSourceCodeFileOnStartup = settings->getValueByName("LoadLastOpenedFileOnStartup").toBool();

	// Only load last source code file if set to do so
	if ( loadLastSourceCodeFileOnStartup ) {
		currentSourceFile = settings->getValueByName("LastOpenedFile").toString();

		// If source file exist load it.
		if ( QFile::exists(currentSourceFile) ) {
			QFileInfo fileInfo(currentSourceFile);
			currentSourceFile = fileInfo.absoluteFilePath();
			sourceFileContent = loadFile(currentSourceFile);
		}
        // If the last opened source code file does not exist, try to load the default example.cpp file.
        else if ( QFile::exists(dataDirctoryStr + "example.cpp") ) {
			QFileInfo fileInfo(dataDirctoryStr + "example.cpp");
			currentSourceFile = fileInfo.absoluteFilePath();
			sourceFileContent = loadFile(currentSourceFile);
		}
		// If neither the example source code file exists show some small code example.
		else {
			currentSourceFile = "untitled.cpp";
			currentSourceFileExtension = "cpp";
			sourceFileContent = "if(x==\"y\"){x=z;}";
		}
	}
	// if last opened source file should not be loaded make some default settings.
	else {
		currentSourceFile = "untitled.cpp";
		currentSourceFileExtension = "cpp";
		sourceFileContent = "";
	}
    savedSourceContent = sourceFileContent;

    // Update the mainwindow title to show the name of the loaded source code file.
    updateWindowTitle();
}


/*!
    Saves the settings for the main application to the file "UniversalIndentGUI.ini".
    Settings are for example last selected indenter, last loaded config file and so on.
*/
void MainWindow::saveSettings() {
    QFileInfo fileInfo(currentSourceFile);
    if ( fileInfo.isFile() ) {
        settings->setValueByName( "LastOpenedFile", currentSourceFile );
    }
	//settings->setValueByName( "LoadLastOpenedFileOnStartup", uiGuiLoadLastOpenedFileOnStartup->isChecked() );
    settings->setValueByName( "LastSelectedIndenterID", currentIndenterID );
    //settings->setValueByName( "IndenterParameterTooltipsEnabled", uiGuiIndenterParameterTooltipsEnabled->isChecked() );
    //settings->setValueByName( "Language", language );
	settings->setValueByName( "FileEncoding", currentEncoding );
    settings->setValueByName( "VersionInSettingsFile", version );
	settings->setValueByName( "WindowIsMaximized", isMaximized() );
	if ( !isMaximized() ) {
		settings->setValueByName( "WindowPosition", pos() );
		settings->setValueByName( "WindowSize", size() );
	}
    //settings->setValueByName( "SyntaxHighlightningEnabled", uiGuiSyntaxHighlightningEnabled->isChecked() );
    //settings->setValueByName( "WhiteSpaceIsVisible", uiGuiWhiteSpaceIsVisible->isChecked() );
    //settings->setValueByName( "TabWidth", txtedSourceCode->tabWidth() );

    //FIXME: Needs to be called explicit here, because the destructor of UiGuiSettings doesn't do it.
	settings->saveSettings();

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
        if ( uiGuiIndenterParameterTooltipsEnabled->isChecked() ) {
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
	QString languageName;
    QAction *languageAction;

    // Get the language settings from the settings object.
	int languageIndex = settings->getValueByName("Language").toInt();

    languageActionGroup = new QActionGroup(this);

    // Loop for each found translation file
    for ( int i = 0; i < settings->getAvailableTranslations().count(); i++ ) {
        languageShort = settings->getAvailableTranslations().at(i);

        // Identify the language mnemonic and set the full name.
        if ( languageShort == "en" ) {
            languageName = tr("English");
        }
        else if ( languageShort == "de" ) {
            languageName = tr("German");
        }
		else if ( languageShort == "tw" ) {
			languageName = tr("Chinese (Taiwan)");
		}
		else if ( languageShort == "ja" ) {
			languageName = tr("Japanese");
		}
        else {
            languageName = tr("Unknown language mnemonic ") + languageShort;
        }

        languageAction = new QAction(languageName, languageActionGroup);
        languageAction->setStatusTip(languageName + tr(" as user interface language."));
		languageAction->setIcon( QIcon(QString(":/language/language-"+languageShort+".png")) );
        languageAction->setCheckable(true);

        // If the language selected in the ini file or no ini exists the system locale is
        // equal to the current language mnemonic set this menu entry checked.
        if ( i == languageIndex ) {
            languageAction->setChecked(true);
        }
    }

    QString languageString =  tr("Language");
    //languageMenu = menuSettings->addMenu( languageString );

    //languageMenu->addActions( languageActionGroup->actions() );

	connect( settings, SIGNAL(language(int)), this, SLOT(languageChanged(int)) );
    connect( languageActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(languageChanged(QAction*)) );
}


/*!
	This slot is called whenever a language is selected in the menu. It tries to find the
	corresponding action in the languageInfoList and sets the language.
 */
void MainWindow::languageChanged(QAction* languageAction) {
	int languageIndex = languageActionGroup->actions().indexOf(languageAction);
	settings->setValueByName("Language", languageIndex);
}


/*!
    This slot is called whenever a language is selected in the menu. It tries to find the
    corresponding action in the languageInfoList and sets the language.
 */
void MainWindow::languageChanged(int languageIndex) {
    // Set the language in the menu to the new selected language.
    languageActionGroup->actions().at(languageIndex)->setChecked(true);

    // Get the mnemonic of the new selected language.
	QString languageShort = settings->getAvailableTranslations().at(languageIndex);

	// Remove the old translation.
	qApp->removeTranslator( translator );

	// Load the new translation file and add it to the translation list.
	translator->load( QString("./translations/universalindent_") + languageShort );
	qApp->installTranslator( translator );
}


/*!
    Creates a menu entry under the settings menu for all available text encodings.
*/
void MainWindow::createEncodingMenu() {
    QAction *encodingAction;
    QString encodingName;

    encodingActionGroup = new QActionGroup(this);

    encodingsList = QStringList() << "UTF-8" << "UTF-16" << "UTF-16BE" << "UTF-16LE"
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

    connect( encodingActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(encodingChanged(QAction*)) );
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
	Creates a menu entry under the settings menu for all available text encodings.
*/
void MainWindow::createHighlighterMenu() {
	QAction *highlighterAction;
    QMenu *highlighterMenu;
	QString highlighterName;

	highlighterActionGroup = new QActionGroup(this);

	// Loop for each known highlighter
    foreach ( highlighterName, highlighter->getAvailableHighlighters() ) {
		highlighterAction = new QAction(highlighterName, highlighterActionGroup);
		highlighterAction->setStatusTip( tr("Set the syntax highlightning to ") + highlighterName );
		highlighterAction->setCheckable(true);
	}
	//encodingActionGroup->actions().first()->setChecked(true);
	highlighterMenu = new QMenu( tr("Set Syntax Highlighter") );
	highlighterMenu->addActions( highlighterActionGroup->actions() );
    menuSettings->insertMenu(uiGuiIndenterParameterTooltipsEnabled, highlighterMenu );

	connect( highlighterActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(highlighterChanged(QAction*)) );
}


/*!
    This slot handles signals coming from selecting another syntax highlighter.
 */
void MainWindow::highlighterChanged(QAction* highlighterAction) {
	QString highlighterName = highlighterAction->text();
    highlighter->setLexerByName( highlighterName );
    //TODO: This is really no nice way. How do it better?
    // Need to do this "text update" to update the syntax highlighting. Otherwise highlighting is wrong.
    previewToggled = true;
    updateSourceView();
}


/*!
	Is called whenever the white space visibility is being changed in the menu.
 */
void MainWindow::setWhiteSpaceVisibility(bool visible) {
	if ( visible ) {
		txtedSourceCode->setWhitespaceVisibility(QsciScintilla::WsVisible);
	}
	else {
		txtedSourceCode->setWhitespaceVisibility(QsciScintilla::WsInvisible);
	}
}

/*!
	This slot is called whenever the number of lines in the editor changes
	and adapts the margin for the displayed line numbers.
*/
void MainWindow::numberOfLinesChanged() {
	QString lineNumbers;
	lineNumbers.setNum( txtedSourceCode->lines()*10 );
	txtedSourceCode->setMarginWidth(1, lineNumbers);
}


/*!
    Catches language change events and retranslates all needed widgets.
 */
void MainWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        QString languageName;

        // Translate the main window.
        retranslateUi(this);

        // Translate the toolbar.
        toolBarWidget->retranslateUi(toolBar);

        // Translate the language menu.
        //languageMenu->setTitle( tr("Language") );
        int i = 0;
        foreach ( QAction* languageAction, languageActionGroup->actions() ) {
            QString languageShort = settings->getAvailableTranslations().at(i);

            // Identify the language mnemonic and set the full name
            if ( languageShort == "en" ) {
                languageName = tr("English");
            }
            else if ( languageShort == "de" ) {
                languageName = tr("German");
            }
            else if ( languageShort == "tw" ) {
                languageName = tr("Chinese (Taiwan)");
            }
            else if ( languageShort == "ja" ) {
                languageName = tr("Japanese");
            }
            else {
                languageName = tr("Unknown language mnemonic ") + languageShort;
            }
            languageAction->setText( languageName );
            languageAction->setStatusTip( languageName + tr(" as user interface language.") );
            i++;
        }

        // Translate the encoding menu.
        encodingMenu->setTitle( tr("Reopen File with other Encoding") );
        QList<QAction *> encodingActionList = encodingActionGroup->actions();
        for ( int i = 0; i < encodingActionList.size(); i++ ) {
            encodingActionList.at(i)->setStatusTip( tr("Reopen the currently opened source code file by using the text encoding scheme ") + encodingsList.at(i) );
        }

        // Translate the highlighter menu.
        highlighter->retranslate();
    } 
    else {
        QWidget::changeEvent(event);
    }
}
