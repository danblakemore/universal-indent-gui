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

#include "mainwindow.h"

//! \defgroup grp_MainWindow All concerning main window functionality.

/*!
    \class MainWindow
    \ingroup grp_MainWindow
    \brief Is the main window of UniversalIndentGUI

    The MainWindow class is responsible for generating and displaying most of the gui elements.
    Its look is set in the file "mainwindow.ui". An object for the indent handler is generated here
    and user actions are being controlled. Is responsible for file open dialogs and indenter selection.
 */

/*!
    \brief Constructs the main window.
 */
MainWindow::MainWindow(QString file2OpenOnStart, QWidget *parent) : QMainWindow(parent) {
    // set the program version, revision and date, which is shown in the main window title and in the about dialog.
    version = "1.0.1";
    revision = "854";
    QDate buildDate(2008, 10, 20);
    buildDateStr = buildDate.toString("d. MMMM yyyy");

    // Init of some variables.
    sourceCodeChanged = false;
    scrollPositionChanged = false;

    // Create the settings object, which loads all UiGui settings from a file.
	settings = UiguiSettings::getInstance();

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
    createEncodingMenu();
    createHighlighterMenu();
	

    // generate about dialog box
    aboutDialog = new AboutDialog(this, Qt::SplashScreen, version, revision, buildDateStr);
    aboutDialogGraphicsView = new AboutDialogGraphicsView(aboutDialog, this);
    connect( toolBarWidget->pbAbout, SIGNAL(clicked()), this, SLOT(showAboutDialog()) );
    connect( actionAbout_UniversalIndentGUI, SIGNAL(triggered()), this, SLOT(showAboutDialog()) );

	// generate settings dialog box
	settingsDialog = new UiguiSettingsDialog(this, settings);
    connect( actionShowSettings, SIGNAL(triggered()), settingsDialog, SLOT(showDialog()) );

    // If a file that should be opened on start has been handed over to the constructor exists, load it
    if ( QFile::exists(file2OpenOnStart) ) {
        openSourceFileDialog(file2OpenOnStart);
    }
    // Otherwise load the last opened file, if this is enabled in the settings.
    else {
        loadLastOpenedFile();
    }

    updateSourceView();

    // Check if a newer version is available but only if the setting for that is enabled and today not already a check has been done.
    if ( settings->getValueByName("CheckForUpdate").toBool() && QDate::currentDate() != settings->getValueByName("LastUpdateCheck").toDate() ) {
        updateCheckDialog->checkForUpdate();
    }

    // Enable accept dropping of files.
    setAcceptDrops(true);
}


/*!
    \brief Initializes the main window by creating the main gui and make some settings.
 */
void MainWindow::initMainWindow() {
	   // Generate gui as it is build in the file "mainwindow.ui"
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
#ifndef Q_OS_MAC // On Mac restoring the window state causes the screenshot no longer to work.
    restoreState( settings->getValueByName("MainWindowState").toByteArray() );
#endif

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

    updateCheckDialog = new UpdateCheckDialog(version, settings, this);

    // Register the syntax highlightning setting in the menu to the settings object.
    connect( enableSyntaxHighlightningAction, SIGNAL(toggled(bool)), settings, SLOT(handleValueChangeFromExtern(bool)) );
    connect( settings, SIGNAL(syntaxHighlightningEnabled(bool)), enableSyntaxHighlightningAction, SLOT(setChecked(bool)) );
    enableSyntaxHighlightningAction->setChecked( settings->getValueByName("SyntaxHighlightningEnabled").toBool() );
    // Tell the highlighter if it has to be enabled or disabled.
    connect( settings, SIGNAL(syntaxHighlightningEnabled(bool)), this, SLOT(turnHighlightOnOff(bool)) );

    // Register the load last file setting in the menu to the settings object.
    connect( loadLastOpenedFileOnStartupAction, SIGNAL(toggled(bool)), settings, SLOT(handleValueChangeFromExtern(bool)) );
    connect( settings, SIGNAL(loadLastOpenedFileOnStartup(bool)), loadLastOpenedFileOnStartupAction, SLOT(setChecked(bool)) );
    loadLastOpenedFileOnStartupAction->setChecked( settings->getValueByName("LoadLastOpenedFileOnStartup").toBool() );

    // Register the white space setting in the menu to the settings object.
    connect( whiteSpaceIsVisibleAction, SIGNAL(toggled(bool)), settings, SLOT(handleValueChangeFromExtern(bool)) );
    connect( settings, SIGNAL(whiteSpaceIsVisible(bool)), whiteSpaceIsVisibleAction, SLOT(setChecked(bool)) );
    whiteSpaceIsVisibleAction->setChecked( settings->getValueByName("WhiteSpaceIsVisible").toBool() );
    // Tell the QScintilla editor if it has to show white space.
    connect( settings, SIGNAL(whiteSpaceIsVisible(bool)), this, SLOT(setWhiteSpaceVisibility(bool)) );

    // Connect the remaining menu items.
    connect( actionOpen_Source_File, SIGNAL(triggered()), this, SLOT(openSourceFileDialog()) );
    connect( actionSave_Source_File_As, SIGNAL(triggered()), this, SLOT(saveasSourceFileDialog()) );
    connect( actionSave_Source_File, SIGNAL(triggered()), this, SLOT(saveSourceFile()) );
    connect( actionExportPDF, SIGNAL(triggered()), this, SLOT(exportToPDF()) );
    connect( actionExportHTML, SIGNAL(triggered()), this, SLOT(exportToHTML()) );
    connect( actionCheck_for_update, SIGNAL(triggered()), updateCheckDialog, SLOT(checkForUpdateAndShowDialog()) );

    // Init the menu for selecting one of the recently opened files.
    updateRecentlyOpenedList();
    connect( menuRecently_Opened_Files, SIGNAL(triggered(QAction*)), this, SLOT(openFileFromRecentlyOpenedList(QAction*)) );
    connect( settings, SIGNAL(recentlyOpenedListSize(int)), this, SLOT(updateRecentlyOpenedList()) );
}


/*!
    \brief Creates and inits the tool bar. It is added to the main window.
 */
void MainWindow::initToolBar() {
    // Create the tool bar and add it to the main window.
    toolBarWidget = new Ui::toolBarWidget();
    QWidget* helpWidget = new QWidget();
    toolBarWidget->setupUi(helpWidget);
    toolBar->addWidget(helpWidget);
    toolBar->setAllowedAreas( Qt::TopToolBarArea | Qt::BottomToolBarArea );

    // Connect the tool bar widgets to their functions.
    connect( toolBarWidget->enableSyntaxHighlightningCheckBox, SIGNAL(toggled(bool)), settings, SLOT(handleValueChangeFromExtern(bool)) );
    connect( settings, SIGNAL(syntaxHighlightningEnabled(bool)), toolBarWidget->enableSyntaxHighlightningCheckBox, SLOT(setChecked(bool)) );
    toolBarWidget->enableSyntaxHighlightningCheckBox->setChecked( settings->getValueByName("SyntaxHighlightningEnabled").toBool() );
    toolBarWidget->enableSyntaxHighlightningCheckBox->hide();
    connect( toolBarWidget->pbOpen_Source_File, SIGNAL(clicked()), this, SLOT(openSourceFileDialog()) );
    connect( toolBarWidget->pbExit, SIGNAL(clicked()), this, SLOT(close()));
    connect( toolBarWidget->cbLivePreview, SIGNAL(toggled(bool)), this, SLOT(previewTurnedOnOff(bool)) );
    connect( toolBarWidget->cbLivePreview, SIGNAL(toggled(bool)), actionLive_Indent_Preview, SLOT(setChecked(bool)) );
    connect( actionLive_Indent_Preview, SIGNAL(toggled(bool)), toolBarWidget->cbLivePreview, SLOT(setChecked(bool)) );
}


/*!
    \brief Create and initialize the text editor component. It uses the QScintilla widget.
 */
void MainWindow::initTextEditor() {

    QMessageBox infoQScintillaLoad(NULL);
    infoQScintillaLoad.setWindowTitle("Loading QScintilla...");
    infoQScintillaLoad.setText("Trying to load QScintilla library.\nIf anything fails during loading, it might be possible that the debug and release version of QScintilla are mixed.");
    infoQScintillaLoad.show();
    // Create the QScintilla widget and add it to the layout.
    // Try and catch doesn't seem to catch the runtime error when starting UiGUI release with QScintilla debug lib and the other way around.
    try {
        qSciSourceCodeEditor = new QsciScintilla(this);
    }
    catch (...) {
        QMessageBox::critical(this, "Error creating QScintilla text editor component!", 
            "During trying to create the text editor component, that is based on QScintilla, an error occurred. Please make sure that you have installed QScintilla and not mixed release and debug versions." );
        exit(1);
    }
    infoQScintillaLoad.hide();

	hboxLayout1->addWidget(qSciSourceCodeEditor);

    // Make some settings for the QScintilla widget.
    qSciSourceCodeEditor->setUtf8(true);
    qSciSourceCodeEditor->setMarginLineNumbers(1, true);
	qSciSourceCodeEditor->setMarginWidth(1, QString("10000") );
	qSciSourceCodeEditor->setBraceMatching(qSciSourceCodeEditor->SloppyBraceMatch);
	qSciSourceCodeEditor->setMatchedBraceForegroundColor( QColor("red") );
	qSciSourceCodeEditor->setFolding(QsciScintilla::BoxedTreeFoldStyle);
	qSciSourceCodeEditor->setAutoCompletionSource(QsciScintilla::AcsAll);
	qSciSourceCodeEditor->setAutoCompletionThreshold(3);

    // Handle if white space is set to be visible
	bool whiteSpaceIsVisible = settings->getValueByName("WhiteSpaceIsVisible").toBool();
    setWhiteSpaceVisibility( whiteSpaceIsVisible );

    // Handle the width of tabs in spaces
    int tabWidth = settings->getValueByName("TabWidth").toInt();
    qSciSourceCodeEditor->setTabWidth(tabWidth);

    // Remember a pointer to the scrollbar of the QScintilla widget used to keep
    // on the same line as before when turning preview on/off.
    textEditVScrollBar = qSciSourceCodeEditor->verticalScrollBar();

    // Add a column row indicator to the status bar.
    textEditLineColumnInfoLabel = new QLabel( tr("Line %1, Column %2").arg(1).arg(1) );
    statusbar->addPermanentWidget(textEditLineColumnInfoLabel);
    connect( qSciSourceCodeEditor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(setStatusBarCursorPosInfo(int, int)) );

    // Connect the text editor to dependent functions.
    connect( qSciSourceCodeEditor, SIGNAL(textChanged()), this, SLOT(sourceCodeChangedHelperSlot()) );
	connect( qSciSourceCodeEditor, SIGNAL(linesChanged()), this, SLOT(numberOfLinesChanged()) );
    connect( settings, SIGNAL(tabWidth(int)), qSciSourceCodeEditor, SLOT(setTabWidth(int)) );
    qSciSourceCodeEditor->setTabWidth( settings->getValueByName("TabWidth").toInt() );
}


/*!
    \brief Create and init the syntax highlighter and set it to use the QScintilla edit component.
 */
void MainWindow::initSyntaxHighlighter() {
    // Create the highlighter.
    highlighter = new UiguiHighlighter(qSciSourceCodeEditor);

    // Handle if syntax highlighting is enabled
	bool syntaxHighlightningEnabled = settings->getValueByName("SyntaxHighlightningEnabled").toBool();
	if ( syntaxHighlightningEnabled ) {
        highlighter->turnHighlightOn();
    }
    else {
        highlighter->turnHighlightOff();
    }
}


/*!
    \brief Initializes the language of UniversalIndentGUI.

    If the program language is defined in the settings, the corresponding language
    file will be loaded and set for the application. If not set there, the system
    default language will be set, if a translation file for that language exists.
    Returns true, if the translation file could be loaded. Otherwise it returns
    false and uses the default language, which is English.
 */
bool MainWindow::initApplicationLanguage() {
    QString languageShort;

    // Get the language settings from the settings object.
	int languageIndex = settings->getValueByName("Language").toInt();

    // If no language was set, indicated by a negative index, use the system language.
    if ( languageIndex < 0 ) {
        languageShort = QLocale::system().name();

        // Chinese and Japanese language consist of country and language code.
        // For all others the language code will be cut off.
        if ( languageShort.left(2) != "zh" && languageShort.left(2) != "ja" ) {
            languageShort = languageShort.left(2);
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

    // Load the Qt own translation file and set it for the application.
    qTTranslator = new QTranslator();
    bool translationFileLoaded;
    translationFileLoaded = qTTranslator->load( SettingsPaths::getGlobalFilesPath() + "/translations/qt_" + languageShort );
    if ( translationFileLoaded ) {
        qApp->installTranslator(qTTranslator);
    }

    // Load the uigui translation file and set it for the application.
    uiGuiTranslator = new QTranslator();
    translationFileLoaded = uiGuiTranslator->load( SettingsPaths::getGlobalFilesPath() + "/translations/universalindent_" + languageShort );
    if ( translationFileLoaded ) {
        qApp->installTranslator(uiGuiTranslator);
    }

    connect( settings, SIGNAL(language(int)), this, SLOT(languageChanged(int)) );

    return translationFileLoaded;
}


/*!
    \brief Creates and initializes the indenter.
 */
void MainWindow::initIndenter() {
    // Get Id of last selected indenter.
	currentIndenterID = settings->getValueByName("SelectedIndenter").toInt();

    // Create the indenter widget with the ID and add it to the layout.
    indentHandler = new IndentHandler(currentIndenterID, this, centralwidget);
    vboxLayout->addWidget(indentHandler);

    // If settings for the indenter have changed, let the main window know aboud it.
    connect(indentHandler, SIGNAL(indenterSettingsChanged()), this, SLOT(indentSettingsChangedSlot()));

    // Set this true, so the indenter is called at first program start
    indentSettingsChanged = true;
    previewToggled = true;

    // Handle if indenter parameter tool tips are enabled
    connect( indenterParameterTooltipsEnabledAction, SIGNAL(toggled(bool)), settings, SLOT(handleValueChangeFromExtern(bool)) );
    connect( settings, SIGNAL(indenterParameterTooltipsEnabled(bool)), indenterParameterTooltipsEnabledAction, SLOT(setChecked(bool)) );
    indenterParameterTooltipsEnabledAction->setChecked( settings->getValueByName("IndenterParameterTooltipsEnabled").toBool() );

    // Add the indenters context menu to the mainwindows menu.
    menuIndenter->addActions( indentHandler->getIndenterMenuActions() );
    
}


/*!
    \brief Tries to load the by \a filePath defined file and returns its content as QString.

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
		int indexOfHighlighter = highlighter->setLexerForExtension( currentSourceFileExtension );
        highlighterActionGroup->actions().at(indexOfHighlighter)->setChecked(true);
    }
    return fileContent;
}


/*!
    \brief Calls the source file open dialog to load a source file for the formatting preview.

    If the file was successfully loaded the indenter will be called to generate the formatted source code.
 */
void MainWindow::openSourceFileDialog(QString fileName) {
    // If the source code file is changed and the shown dialog for saving the file
    // is canceled, also stop opening another source file.
    if ( !maybeSave() ) {
        return;
    }
    QString openedSourceFileContent = "";
    QString fileExtensions = tr("Supported by indenter")+" ("+indentHandler->getPossibleIndenterFileExtensions()+
                             ");;"+tr("All files")+" (*.*)";

    //QString openedSourceFileContent = openFileDialog( tr("Choose source code file"), "./", fileExtensions );
	if ( fileName.isEmpty() ) {
		fileName = QFileDialog::getOpenFileName( this, tr("Choose source code file"), currentSourceFile, fileExtensions);
	}

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
        updateRecentlyOpenedList();
        textEditLastScrollPos = 0;
        textEditVScrollBar->setValue( textEditLastScrollPos );

        savedSourceContent = openedSourceFileContent;
        qSciSourceCodeEditor->setModified( false );
        setWindowModified( false );
    }
}


/*!
    \brief Calls the source file save as dialog to save a source file under a chosen name.

    If the file already exists and it should be overwritten, a warning is shown before.
 */
bool MainWindow::saveasSourceFileDialog(QAction *chosenEncodingAction) {
    QString encoding;
    QString fileExtensions = tr("Supported by indenter")+" ("+indentHandler->getPossibleIndenterFileExtensions()+
                             ");;"+tr("All files")+" (*.*)";

    //QString openedSourceFileContent = openFileDialog( tr("Choose source code file"), "./", fileExtensions );
    QString fileName = QFileDialog::getSaveFileName( this, tr("Save source code file"), currentSourceFile, fileExtensions);

    // Saving has been canceled if the filename is empty
    if ( fileName.isEmpty() ) {
        return false;
    }

    savedSourceContent = qSciSourceCodeEditor->text();

    currentSourceFile = fileName;
    QFile::remove(fileName);
    QFile outSrcFile(fileName);
    outSrcFile.open( QFile::ReadWrite | QFile::Text );
    
    // Get current encoding.
    if ( chosenEncodingAction != NULL ) {
        encoding = chosenEncodingAction->text();
    }
    else {
        encoding = encodingActionGroup->checkedAction()->text();
    }
    QTextStream outSrcStrm(&outSrcFile);
    outSrcStrm.setCodec( QTextCodec::codecForName(encoding.toAscii()) );
    outSrcStrm << savedSourceContent;
    outSrcFile.close();

    QFileInfo fileInfo(fileName);
    currentSourceFileExtension = fileInfo.suffix();

    qSciSourceCodeEditor->setModified( false );
    setWindowModified( false );

    updateWindowTitle();
    return true;
}


/*!
    \brief Saves the currently shown source code to the last save or opened source file.

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
        savedSourceContent = qSciSourceCodeEditor->text();
        outSrcFile.open( QFile::ReadWrite | QFile::Text );

        // Get current encoding.
        QString currentEncoding = encodingActionGroup->checkedAction()->text();
        QTextStream outSrcStrm(&outSrcFile);
        outSrcStrm.setCodec( QTextCodec::codecForName(currentEncoding.toAscii()) );
        outSrcStrm << savedSourceContent;
        outSrcFile.close();

        qSciSourceCodeEditor->setModified( false );
        setWindowModified( false );
    }
    return true;
}


/*!
    \brief Shows a file open dialog.

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
    \brief Updates the displaying of the source code.

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
        disconnect( qSciSourceCodeEditor, SIGNAL(textChanged ()), this, SLOT(sourceCodeChangedHelperSlot()) );
		bool textIsModified = isWindowModified();
        qSciSourceCodeEditor->setText(sourceViewContent);
		setWindowModified(textIsModified);
        previewToggled = false;
        connect( qSciSourceCodeEditor, SIGNAL(textChanged ()), this, SLOT(sourceCodeChangedHelperSlot()) );
    }

    textEditVScrollBar->setValue( textEditLastScrollPos );
}


/*!
    \brief Calls the selected indenter with the currently loaded source code to retrieve the formatted source code.

    The original loaded source code file will not be changed.
 */
void MainWindow::callIndenter() {
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    sourceFormattedContent = indentHandler->callIndenter(sourceFileContent, currentSourceFileExtension);
    //updateSourceView();
    QApplication::restoreOverrideCursor();
}


/*!
    \brief Switches the syntax highlighting corresponding to the value \a turnOn either on or off.
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
    \brief Added this slot to avoid multiple calls because of changed text.
 */
void MainWindow::sourceCodeChangedHelperSlot() {
	QTimer::singleShot(0, this, SLOT(sourceCodeChangedSlot()));
}


/*!
    \brief Is emitted whenever the text inside the source view window changes. Calls the indenter
    to format the changed source code.
 */
void MainWindow::sourceCodeChangedSlot() {
    QChar enteredCharacter;
	int cursorPos, cursorPosAbsolut, cursorLine;
	QString text;

    sourceCodeChanged = true;
    if ( scrollPositionChanged ) {
        scrollPositionChanged = false;
    }

    // Get the content text of the text editor.
    sourceFileContent = qSciSourceCodeEditor->text();
	
    // Get the position of the cursor in the unindented text.
    if ( sourceFileContent.isEmpty() ) {
        // Add this line feed, because AStyle has problems with a totally emtpy file.
        sourceFileContent += "\n";
        cursorPosAbsolut = 0;
        cursorPos = 0;
        cursorLine = 0;
        enteredCharacter = sourceFileContent.at(cursorPosAbsolut);
    }
    else {
        qSciSourceCodeEditor->getCursorPosition(&cursorLine, &cursorPos);
        cursorPosAbsolut = qSciSourceCodeEditor->SendScintilla(QsciScintillaBase::SCI_GETCURRENTPOS);
	    text = qSciSourceCodeEditor->text(cursorLine);
        if ( cursorPosAbsolut > 0 ) {
            cursorPosAbsolut--;
        }
        if ( cursorPos > 0 ) {
            cursorPos--;
        }
        enteredCharacter = sourceFileContent.at(cursorPosAbsolut);
    }

    // Call the indenter to reformat the text.
    if ( toolBarWidget->cbLivePreview->isChecked() ) {
        callIndenter();
        previewToggled = true;
    }

    // Update the text editor.
    updateSourceView();

    if ( toolBarWidget->cbLivePreview->isChecked() && !enteredCharacter.isNull() && enteredCharacter != 10 ) {
        //const char ch = enteredCharacter.toAscii();

        int saveCursorLine = cursorLine;
        int saveCursorPos = cursorPos;

        bool charFound = false;

        // Search forward
        for ( cursorLine = saveCursorLine; cursorLine-saveCursorLine < 6 && cursorLine < qSciSourceCodeEditor->lines(); cursorLine++ ) {
            text = qSciSourceCodeEditor->text(cursorLine);
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
            text = qSciSourceCodeEditor->text(saveCursorLine);
            cursorPos = saveCursorPos;
            if ( cursorPos >= text.count() ) {
                cursorPos = text.count() - 1;
            }

            for ( cursorLine = saveCursorLine; saveCursorLine-cursorLine < 6 && cursorLine >= 0; cursorLine-- ) {
                text = qSciSourceCodeEditor->text(cursorLine);
                while ( cursorPos >= 0 && enteredCharacter != text.at(cursorPos)) {
                    cursorPos--;
                }
                if ( cursorPos < 0 ) {
                    cursorPos = qSciSourceCodeEditor->lineLength(cursorLine-1) - 1;
                } 
                else {
                    charFound = true;
                    break;
                }
            }
        }

        // If the character was found set its new cursor position...
        if ( charFound ) {
            qSciSourceCodeEditor->setCursorPosition( cursorLine, cursorPos+1 );
        }
        // ...if it was not found, set the previous cursor position.
        else {
            qSciSourceCodeEditor->setCursorPosition( saveCursorLine, saveCursorPos+1 );
        }
    }
    // set the previous cursor position.
    else if ( enteredCharacter == 10 ) {
        qSciSourceCodeEditor->setCursorPosition( cursorLine, cursorPos );
    }


    if ( toolBarWidget->cbLivePreview->isChecked() ) {
        sourceCodeChanged = false;
    }

    if ( savedSourceContent == qSciSourceCodeEditor->text() ) {
        qSciSourceCodeEditor->setModified( false );
        setWindowModified( false );
    }
    else {
        qSciSourceCodeEditor->setModified( true ); // Has no effect according to QScintilla docs.
        setWindowModified( true );
    }

    // Could set cursor this way and use normal linear search in text instead of columns and rows.
    //qSciSourceCodeEditor->SendScintilla(QsciScintillaBase::SCI_SETCURRENTPOS, 50);
    //qSciSourceCodeEditor->SendScintilla(QsciScintillaBase::SCI_SETANCHOR, 50);
}


/*!
    \brief This slot is called whenever one of the indenter settings are changed.

    It calls the selected indenter if the preview is turned on. If preview
    is not active a flag is set, that the settings have changed.
 */
void MainWindow::indentSettingsChangedSlot() {
    indentSettingsChanged = true;

	int cursorLine, cursorPos;
	qSciSourceCodeEditor->getCursorPosition(&cursorLine, &cursorPos);

    if ( toolBarWidget->cbLivePreview->isChecked() ) {
        callIndenter();
        previewToggled = true;

        updateSourceView();
        if (sourceCodeChanged) {
/*            savedCursor = qSciSourceCodeEditor->textCursor();
            if ( cursorPos >= qSciSourceCodeEditor->text().count() ) {
                cursorPos = qSciSourceCodeEditor->text().count() - 1;
            }
            savedCursor.setPosition( cursorPos );
            qSciSourceCodeEditor->setTextCursor( savedCursor );
*/
            sourceCodeChanged = false;
        }
        indentSettingsChanged = false;
    }
    else {
        updateSourceView();
    }

    if ( savedSourceContent == qSciSourceCodeEditor->text() ) {
        qSciSourceCodeEditor->setModified( false );
        setWindowModified( false );
    }
    else {
        qSciSourceCodeEditor->setModified( true ); // Has no effect according to QScintilla docs.
        setWindowModified( true );
    }
}


/*!
    \brief This slot is called whenever the preview button is turned on or off.

    It calls the selected indenter to format the current source code if
    the code has been changed since the last indenter call.
 */
void MainWindow::previewTurnedOnOff(bool turnOn) {
    previewToggled = true;

	int cursorLine, cursorPos;
	qSciSourceCodeEditor->getCursorPosition(&cursorLine, &cursorPos);

    if ( turnOn && (indentSettingsChanged || sourceCodeChanged) ) {
        callIndenter();
    }
    updateSourceView();
    if (sourceCodeChanged) {
/*        savedCursor = qSciSourceCodeEditor->textCursor();
        if ( cursorPos >= qSciSourceCodeEditor->text().count() ) {
            cursorPos = qSciSourceCodeEditor->text().count() - 1;
        }
        savedCursor.setPosition( cursorPos );
        qSciSourceCodeEditor->setTextCursor( savedCursor );
*/
        sourceCodeChanged = false;
    }
    indentSettingsChanged = false;

    if ( savedSourceContent == qSciSourceCodeEditor->text() ) {
        qSciSourceCodeEditor->setModified( false );
        setWindowModified( false );
    }
    else {
        qSciSourceCodeEditor->setModified( true );
        setWindowModified( true );
    }
}


/*!
    \brief This slot updates the main window title to show the currently opened
    source code filename.
 */
void MainWindow::updateWindowTitle() {
    this->setWindowTitle( "UniversalIndentGUI " + version +" [*]"+ currentSourceFile);
}


/*!
    \brief Opens a dialog to save the current source code as a PDF document.
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
		printer.printRange(qSciSourceCodeEditor);
    }
}


/*!
    \brief Opens a dialog to save the current source code as a HTML document.
 */
void MainWindow::exportToHTML() {
	QString fileExtensions = tr("HTML Document")+" (*.html)";

    QString fileName = currentSourceFile;
    QFileInfo fileInfo(fileName);
    QString fileExtension = fileInfo.suffix();

    fileName.replace( fileName.length()-fileExtension.length(), fileExtension.length(), "html" );
    fileName = QFileDialog::getSaveFileName( this, tr("Export source code file"), fileName, fileExtensions);

    if ( !fileName.isEmpty() ) {
        // Create a document from which HTML code can be generated.
        QTextDocument sourceCodeDocument( qSciSourceCodeEditor->text() );
        sourceCodeDocument.setDefaultFont( QFont("Courier", 12, QFont::Normal) );
        QString sourceCodeAsHTML = sourceCodeDocument.toHtml();
        // To ensure that empty lines are kept in the HTML code make this replacement.
        sourceCodeAsHTML.replace("\"></p>", "\"><br /></p>");

        // Write the HTML file.
        QFile::remove(fileName);
        QFile outSrcFile(fileName);
        outSrcFile.open( QFile::ReadWrite | QFile::Text );
        outSrcFile.write( sourceCodeAsHTML.toAscii() );
        outSrcFile.close();
    }
}


/*!
    \brief Loads the last opened file if this option is enabled in the settings. 
    
    If the file does not exist, the default example file is tried to be loaded. If even that
    fails a very small code example is shown.
    If the setting for opening the last file is disabled, the editor is empty on startup.
*/
void MainWindow::loadLastOpenedFile() {
    // Get setting for last opened source code file.
	loadLastSourceCodeFileOnStartup = settings->getValueByName("LoadLastOpenedFileOnStartup").toBool();

	// Only load last source code file if set to do so
	if ( loadLastSourceCodeFileOnStartup ) {
        // From the list of last opened files get the first one.
        currentSourceFile = settings->getValueByName("LastOpenedFiles").toString().split("|").first();

		// If source file exist load it.
		if ( QFile::exists(currentSourceFile) ) {
			QFileInfo fileInfo(currentSourceFile);
			currentSourceFile = fileInfo.absoluteFilePath();
			sourceFileContent = loadFile(currentSourceFile);
		}
        // If the last opened source code file does not exist, try to load the default example.cpp file.
        else if ( QFile::exists( SettingsPaths::getIndenterPath() + "/example.cpp" ) ) {
			QFileInfo fileInfo( SettingsPaths::getIndenterPath() + "/example.cpp" );
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
    \brief Saves the settings for the main application to the file "UniversalIndentGUI.ini".

    Settings are for example last selected indenter, last loaded config file and so on.
*/
void MainWindow::saveSettings() {
	settings->setValueByName( "FileEncoding", currentEncoding );
    settings->setValueByName( "VersionInSettingsFile", version );
	settings->setValueByName( "WindowIsMaximized", isMaximized() );
	if ( !isMaximized() ) {
		settings->setValueByName( "WindowPosition", pos() );
		settings->setValueByName( "WindowSize", size() );
	}
    settings->setValueByName( "MainWindowState", saveState() );

    // Also save the syntax highlight style for all lexers.
    highlighter->writeCurrentSettings("");
}


/*!
    \brief Is always called when the program is quit. Calls the saveSettings function before really quits.
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
    \brief This function is setup to capture tooltip events. 
    
    All widgets that are created by the indentHandler object and are responsible 
    for indenter parameters are connected with this event filter. 
    So depending on the settings the tooltips can be enabled and disabled for these widgets.
 */
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if ( event->type() == QEvent::ToolTip) {
        if ( indenterParameterTooltipsEnabledAction->isChecked() ) {
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
    \brief Is called at application exit and asks whether to save the source code file, if it has been changed.
 */
bool MainWindow::maybeSave()
{
    if ( isWindowModified() ) {
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
    \brief This slot is called whenever a language is selected in the menu. It tries to find the
    corresponding action in the languageInfoList and sets the language.
 */
void MainWindow::languageChanged(int languageIndex) {
    // Get the mnemonic of the new selected language.
	QString languageShort = settings->getAvailableTranslations().at(languageIndex);

	// Remove the old qt translation.
	qApp->removeTranslator( qTTranslator );

    // Remove the old uigui translation.
	qApp->removeTranslator( uiGuiTranslator );

    // Load the Qt own translation file and set it for the application.
    bool translationFileLoaded;
    translationFileLoaded = qTTranslator->load( SettingsPaths::getGlobalFilesPath() + "/translations/qt_" + languageShort );
    if ( translationFileLoaded ) {
        qApp->installTranslator(qTTranslator);
    }

    // Load the uigui translation file and set it for the application.
    translationFileLoaded = uiGuiTranslator->load( SettingsPaths::getGlobalFilesPath() + "/translations/universalindent_" + languageShort );
    if ( translationFileLoaded ) {
        qApp->installTranslator(uiGuiTranslator);
    }
}


/*!
    \brief Creates a menu entries in the file menu for opening and saving a file with different encodings.
*/
void MainWindow::createEncodingMenu() {
    QAction *encodingAction;
    QString encodingName;

    encodingsList = QStringList() << "UTF-8" << "UTF-16" << "UTF-16BE" << "UTF-16LE"
            << "Apple Roman" << "Big5" << "Big5-HKSCS" << "EUC-JP" << "EUC-KR" << "GB18030-0"
            << "IBM 850" << "IBM 866" << "IBM 874" << "ISO 2022-JP" << "ISO 8859-1" << "ISO 8859-13"
            << "Iscii-Bng" << "JIS X 0201" << "JIS X 0208" << "KOI8-R" << "KOI8-U" << "MuleLao-1"
            << "ROMAN8" << "Shift-JIS" << "TIS-620" << "TSCII" << "Windows-1250" << "WINSAMI2";

    encodingActionGroup = new QActionGroup(this);
    saveEncodedActionGroup = new QActionGroup(this);

    // Loop for each available encoding
    foreach ( encodingName, encodingsList ) {
        // Create actions for the "reopen" menu
        encodingAction = new QAction(encodingName, encodingActionGroup);
        encodingAction->setStatusTip( tr("Reopen the currently opened source code file by using the text encoding scheme ") + encodingName );
        encodingAction->setCheckable(true);
        if ( encodingName == currentEncoding ) {
            encodingAction->setChecked(true);
        }

        // Create actions for the "save as encoded" menu
        encodingAction = new QAction(encodingName, saveEncodedActionGroup);
        encodingAction->setStatusTip( tr("Save the currently opened source code file by using the text encoding scheme ") + encodingName );
    }

    encodingMenu->addActions( encodingActionGroup->actions() );
    connect( encodingActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(encodingChanged(QAction*)) );

    saveEncodedMenu->addActions( saveEncodedActionGroup->actions() );
    connect( saveEncodedActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(saveAsOtherEncoding(QAction*)) );
}


/*!
    \brief This slot calls the save dialog to save the current source file with another encoding.

    If the saving is successful and not aborted, the currently used encoding, visible in the
    "reopen" menu, is also changed to the new encoding.
*/
void MainWindow::saveAsOtherEncoding(QAction *chosenEncodingAction) {
    bool fileWasSaved = saveasSourceFileDialog(chosenEncodingAction);

    // If the file was save with another encoding, change the selected encoding in the reopen menu.
    if ( fileWasSaved ) {
        foreach ( QAction *action, encodingActionGroup->actions() ) {
            if ( action->text() == chosenEncodingAction->text() ) {
                action->setChecked(true);
                return;
            }
        }
    }
}


/*!
    \brief This slot is called whenever an encoding is selected in the settings menu.
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
            qSciSourceCodeEditor->setText( fileContent );
			qSciSourceCodeEditor->setModified(false);
        }
    }
}


/*!
	\brief Creates a menu entry under the settings menu for all available text encodings.
*/
void MainWindow::createHighlighterMenu() {
	QAction *highlighterAction;
	QString highlighterName;

	highlighterActionGroup = new QActionGroup(this);

	// Loop for each known highlighter
    foreach ( highlighterName, highlighter->getAvailableHighlighters() ) {
		highlighterAction = new QAction(highlighterName, highlighterActionGroup);
		highlighterAction->setStatusTip( tr("Set the syntax highlightning to ") + highlighterName );
		highlighterAction->setCheckable(true);
	}
	highlighterMenu->addActions( highlighterActionGroup->actions() );
    menuSettings->insertMenu(indenterParameterTooltipsEnabledAction, highlighterMenu );

	connect( highlighterActionGroup, SIGNAL(triggered(QAction*)), highlighter, SLOT(setHighlighterByAction(QAction*)) );
}


/*!
	\brief Is called whenever the white space visibility is being changed in the menu.
 */
void MainWindow::setWhiteSpaceVisibility(bool visible) {
	if ( visible ) {
		qSciSourceCodeEditor->setWhitespaceVisibility(QsciScintilla::WsVisible);
	}
	else {
		qSciSourceCodeEditor->setWhitespaceVisibility(QsciScintilla::WsInvisible);
	}
}

/*!
	\brief This slot is called whenever the number of lines in the editor changes
	and adapts the margin for the displayed line numbers.
*/
void MainWindow::numberOfLinesChanged() {
	QString lineNumbers;
	lineNumbers.setNum( qSciSourceCodeEditor->lines()*10 );
	qSciSourceCodeEditor->setMarginWidth(1, lineNumbers);
}


/*!
    \brief Catches language change events and retranslates all needed widgets.
 */
void MainWindow::changeEvent(QEvent *event) {
    int i = 0;

    if (event->type() == QEvent::LanguageChange) {
        QString languageName;

        // Translate the main window.
        retranslateUi(this);
        updateWindowTitle();

        // Translate the toolbar.
        toolBarWidget->retranslateUi(toolBar);

        // Translate the indent handler widget.
        indentHandler->retranslateUi();

         // Translate the load encoding menu.
        QList<QAction *> encodingActionList = encodingActionGroup->actions();
        for ( i = 0; i < encodingActionList.size(); i++ ) {
            encodingActionList.at(i)->setStatusTip( tr("Reopen the currently opened source code file by using the text encoding scheme ") + encodingsList.at(i) );
        }

        // Translate the save encoding menu.
        encodingActionList = saveEncodedActionGroup->actions();
        for ( i = 0; i < encodingActionList.size(); i++ ) {
            encodingActionList.at(i)->setStatusTip( tr("Save the currently opened source code file by using the text encoding scheme ") + encodingsList.at(i) );
        }

        // Translate the highlighter menu.
        QList<QAction *> actionList = highlighterMenu->actions();
        i = 0;
        foreach ( QString highlighterName, highlighter->getAvailableHighlighters() ) {
            QAction *highlighterAction = actionList.at(i);
            highlighterAction->setStatusTip( tr("Set the syntax highlightning to ") + highlighterName );
            i++;
        }

        // Translate the line and column indicators in the statusbar.
        int line, column;
        qSciSourceCodeEditor->getCursorPosition( &line, &column );
        setStatusBarCursorPosInfo( line, column );
    } 
    else {
        QWidget::changeEvent(event);
    }
}



/*!
    \brief Updates the list of recently opened files. 
    
    Therefore the currently open file is set at the lists first position 
    regarding the in the settings set maximum list length. Overheads of the 
    list will be cut off. The new list will be updated to the settings and 
    the recently opened menu will be updated too.
 */
void MainWindow::updateRecentlyOpenedList() {

	QString fileName;
    QString filePath;
    QStringList recentlyOpenedList = settings->getValueByName("LastOpenedFiles").toString().split("|");
    QList<QAction*> recentlyOpenedActionList = menuRecently_Opened_Files->actions();

    // Check if the currently open file is in the list of recently opened.
    int indexOfCurrentFile = recentlyOpenedList.indexOf( currentSourceFile );

    // If it is in the list of recently opened files and not at the first position, move it to the first pos.
    if ( indexOfCurrentFile > 0 ) {
        recentlyOpenedList.move(indexOfCurrentFile, 0);
        recentlyOpenedActionList.move(indexOfCurrentFile, 0);
    }
    // Put the current file at the first position if it not already is and is not empty.
    else if ( indexOfCurrentFile == -1 && !currentSourceFile.isEmpty() ) {
        recentlyOpenedList.insert(0, currentSourceFile);
		QAction *recentlyOpenedAction = new QAction(QFileInfo(currentSourceFile).fileName(), menuRecently_Opened_Files);
		recentlyOpenedAction->setStatusTip(currentSourceFile);
        recentlyOpenedActionList.insert(0, recentlyOpenedAction );
    }

	// Get the maximum recently opened list size.
	int recentlyOpenedListMaxSize = settings->getValueByName("RecentlyOpenedListSize").toInt();

	// Loop for each filepath in the recently opened list, remove non existing files and
	// loop only as long as maximum allowed list entries are set.
    for ( int i = 0; i < recentlyOpenedList.size() && i < recentlyOpenedListMaxSize; ) {
		filePath = recentlyOpenedList.at(i);
        QFileInfo fileInfo(filePath);

		// If the file does no longer exist, remove it from the list.
		if ( !fileInfo.exists() ) {
			recentlyOpenedList.takeAt(i);
            if ( i < recentlyOpenedActionList.size()-2 ) {
                QAction* action = recentlyOpenedActionList.takeAt(i);
                delete action;
            }
		}
        // else if its not already in the menu, add it to the menu.
        else {
            if ( i >= recentlyOpenedActionList.size()-2 ) {
                QAction *recentlyOpenedAction = new QAction(fileInfo.fileName(), menuRecently_Opened_Files);
                recentlyOpenedAction->setStatusTip(filePath);
                recentlyOpenedActionList.insert( recentlyOpenedActionList.size()-2, recentlyOpenedAction );
            }
            i++;
        }
	}

	// Trim the list to its in the settings allowed maximum size.
	while ( recentlyOpenedList.size() > recentlyOpenedListMaxSize ) {
		recentlyOpenedList.takeLast();
        QAction* action = recentlyOpenedActionList.takeAt( recentlyOpenedActionList.size()-3 );
        delete action;
	}

    // Add all actions to the menu.
    menuRecently_Opened_Files->addActions(recentlyOpenedActionList);

    // Write the new recently opened list to the settings.
    settings->setValueByName( "LastOpenedFiles", recentlyOpenedList.join("|") );

    // Enable or disable "actionClear_Recently_Opened_List" if list is [not] emtpy
    if ( recentlyOpenedList.isEmpty() ) {
        actionClear_Recently_Opened_List->setEnabled(false);
    }
    else {
        actionClear_Recently_Opened_List->setEnabled(true);
    }
}


/*!
    \brief This slot empties the list of recently opened files.
 */
void MainWindow::clearRecentlyOpenedList() {
    QStringList recentlyOpenedList = settings->getValueByName("LastOpenedFiles").toString().split("|");
    QList<QAction*> recentlyOpenedActionList = menuRecently_Opened_Files->actions();

    while ( recentlyOpenedList.size() > 0 ) {
		recentlyOpenedList.takeLast();
        QAction* action = recentlyOpenedActionList.takeAt( recentlyOpenedActionList.size()-3 );
        delete action;
	}

    // Write the new recently opened list to the settings.
    settings->setValueByName( "LastOpenedFiles", recentlyOpenedList.join("|") );

    // Disable "actionClear_Recently_Opened_List"
    actionClear_Recently_Opened_List->setEnabled(false);
}


/*!
    \brief This slot is called if an entry from the list of recently opened files is
    being selected.
 */
void MainWindow::openFileFromRecentlyOpenedList(QAction* recentlyOpenedAction) {
    // If the selected action from the recently opened list menu is the clear action
    // call the slot to clear the list and then leave.
    if ( recentlyOpenedAction == actionClear_Recently_Opened_List ) {
        clearRecentlyOpenedList();
        return;
    }

    QString fileName = recentlyOpenedAction->text();
    int indexOfSelectedFile = menuRecently_Opened_Files->actions().indexOf( recentlyOpenedAction );
    QStringList recentlyOpenedList = settings->getValueByName("LastOpenedFiles").toString().split("|");
    QString filePath = recentlyOpenedList.at(indexOfSelectedFile);
	QFileInfo fileInfo(filePath);

	// If the file exists, open it.
	if ( fileInfo.exists() ) {
		openSourceFileDialog(filePath);
	}
	// If it does not exist, show a warning message and update the list of recently opened files.
	else {
		QMessageBox::warning(NULL, tr("File no longer exists"), tr("The file %1 in the list of recently opened files does no longer exist.") );
        // The function updateRecentlyOpenedList() has to be called via a singleShot so it is executed after this
        // function (openFileFromRecentlyOpenedList) has already been left. This has to be done because
        // a Qt3Support function tries to emit a signal based on the existing actions and deleting
        // any of these actions in updateRecentlyOpenedList() causes an error.
		QTimer::singleShot(0, this, SLOT(updateRecentlyOpenedList()) );
	}
}


/*!
    \brief If the dragged in object contains urls/paths to a file, accept the drag.
 */
void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if ( event->mimeData()->hasUrls() ) {
        event->acceptProposedAction();
    }
}


/*!
    \brief If the dropped in object contains urls/paths to a file, open that file.
 */
void MainWindow::dropEvent(QDropEvent *event) {
    if ( event->mimeData()->hasUrls() ) {
        QString filePathName = event->mimeData()->urls().first().toLocalFile();
        openSourceFileDialog(filePathName);
    }

    event->acceptProposedAction();
}


/*!
    \brief If the dropped in object contains urls/paths to a file, open that file.
*/
void MainWindow::showAboutDialog() {
	QPixmap originalPixmap = QPixmap::grabWindow(QApplication::desktop()->screen()->winId());
	qDebug("in main pixmap width %d, numScreens = %d", originalPixmap.size().width(), QApplication::desktop()->availableGeometry().width());
	aboutDialogGraphicsView->setScreenshotPixmap( originalPixmap );
	aboutDialogGraphicsView->show();
}


/*!
    \brief Sets the label in the status bar to show the \a line and \a column number.
*/
void MainWindow::setStatusBarCursorPosInfo( int line, int column ) {
    textEditLineColumnInfoLabel->setText( tr("Line %1, Column %2").arg(line+1).arg(column+1) );
}
