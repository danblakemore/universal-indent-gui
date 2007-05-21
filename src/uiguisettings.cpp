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

#include "uiguisettings.h"

/*!
	\class UiGuiSettings
	\brief Handles the settings of the program. Reads them on startup and saves them on exit.
*/

/*!
	The constructor ...
*/
UiGuiSettings::UiGuiSettings(QMainWindow *parent) : QObject(parent) {
    this->parent = parent;
    settings = new QSettings("./UniversalIndentGUI.ini", QSettings::IniFormat, this);
}


/*!
    The destructor saves the settings to a file.
 */
UiGuiSettings::~UiGuiSettings() {
}


/*!
    Loads the settings for the main application out of the file "UniversalIndentGUI.ini",
    which should exists in the call directory. Settings are for example last selected indenter,
    last loaded config file and so on.
*/
bool UiGuiSettings::loadSettings() {
    bool settingsFileExists = true;

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
        currentSourceFile = dataDirctoryStr + "example.cpp";
    }


	// Handle last opened window size
	// ------------------------------

	// read windows last size and position from settings if the settings file exists
	if ( settingsFileExists ) {
		bool maximized = settings->value( "UniversalIndentGUI/maximized", false ).toBool();
		QPoint pos = settings->value( "UniversalIndentGUI/position", QPoint(0, 0) ).toPoint();
		QSize size = settings->value( "UniversalIndentGUI/size", QSize(800, 600) ).toSize();
		parent->resize(size);
		parent->move(pos);
		if ( maximized ) {
			parent->showMaximized();
		}
	}
	else {
		QPoint pos = QPoint(0, 0);
		QSize size = QSize(800, 600);
		parent->resize(size);
		parent->move(pos);
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
	//actionAuto_Open_Last_file->setChecked( loadLastSourceCodeFileOnStartup );

	// Only load last source code file if set to do so
	if ( loadLastSourceCodeFileOnStartup ) {
		// read last opened source code file from settings if the settings file exists
		if ( settingsFileExists ) {
			currentSourceFile = settings->value("UniversalIndentGUI/lastSourceCodeFile", dataDirctoryStr+"example.cpp").toString();
		}
		else {
			currentSourceFile = dataDirctoryStr+"example.cpp";
		}

		// if source file exist load it
		if ( QFile::exists(currentSourceFile) ) {
			QFileInfo fileInfo(currentSourceFile);
			currentSourceFile = fileInfo.absoluteFilePath();
			//sourceFileContent = loadFile(currentSourceFile);
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
		QFileInfo fileInfo(dataDirctoryStr+"example.cpp");
		currentSourceFile = fileInfo.absolutePath();
		currentSourceFileExtension = "";
		sourceFileContent = "";
	}
    //savedSourceContent = sourceFileContent;


    // Handle last selected indenter
    // -----------------------------

    // read last selected indenter from settings if the settings file exists
    if ( settingsFileExists ) {
        currentIndenterID = settings->value("UniversalIndentGUI/lastSelectedIndenter", 0).toInt();
		if ( currentIndenterID < 0 ) {
			currentIndenterID = 0;
		}
    }
    else {
        currentIndenterID = 0;
    }

    /*
    indentHandler = new IndentHandler(dataDirctoryStr, currentIndenterID, this, centralwidget);
    vboxLayout->addWidget(indentHandler);

	if ( !indentHandler->getAvailableIndenters().isEmpty() ) {
		toolBarWidget->cmbBoxIndenters->addItems( indentHandler->getAvailableIndenters() );
		// Take care if the selected indenterID is greater than the number of existing indenters
		if ( currentIndenterID >= indentHandler->getAvailableIndenters().count() ) {
			currentIndenterID = indentHandler->getAvailableIndenters().count() - 1;
		}
	}
	else {
		currentIndenterID = 0;
		QMessageBox::warning(NULL, tr("No indenter ini files"), tr("There exists no indenter ini files in the directory \"") + QDir(dataDirctoryStr).absolutePath() + "\".");
	}

    toolBarWidget->cmbBoxIndenters->setCurrentIndex( currentIndenterID );
    QObject::connect(indentHandler, SIGNAL(settingsCodeChanged()), this, SLOT(indentSettingsChangedSlot()));
    */

	// Handle if white space is set to be visible
	// ------------------------------------------

	// read if indenter parameter tool tips are enabled
	if ( settingsFileExists ) {
		whiteSpaceIsVisible = settings->value( "UniversalIndentGUI/whiteSpaceIsVisible", false ).toBool();
		//actionWhiteSpaceVisible->setChecked( whiteSpaceIsVisible );
		if ( whiteSpaceIsVisible ) {
			//txtedSourceCode->setWhitespaceVisibility(QsciScintilla::WsVisible);
		}
		else {
			//txtedSourceCode->setWhitespaceVisibility(QsciScintilla::WsInvisible);
		}
	}
	else {
		//actionWhiteSpaceVisible->setChecked( false );
		//txtedSourceCode->setWhitespaceVisibility(QsciScintilla::WsInvisible);
	}


    // Handle if indenter parameter tool tips are enabled
    // --------------------------------------------------

    // read if indenter parameter tool tips are enabled
    if ( settingsFileExists ) {
        indenterParameterTooltipsEnabled = settings->value("UniversalIndentGUI/indenterParameterTooltipsEnabled", true).toBool();
        //actionParameter_Tooltips->setChecked( indenterParameterTooltipsEnabled );
    }
    else {
        //actionParameter_Tooltips->setChecked( true );
    }


    // Handle the width of tabs in spaces
    // ----------------------------------

    // read the tab width
    if ( settingsFileExists ) {
        tabWidth = settings->value("UniversalIndentGUI/tabWidth", 4).toInt();
        //txtedSourceCode->setTabWidth(tabWidth);
    }
    else {
        //txtedSourceCode->setTabWidth(4);
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

	//highlighter->readCurrentSettings("");

    // load the translation file and set it for the application
    /*
    translator = new QTranslator();
    translator->load( QString("./translations/universalindent_") + language );
    qApp->installTranslator(translator);
    retranslateUi(this);
    toolBarWidget->retranslateUi(toolBar);
    */
    return true;
}


/*!
    Saves the settings for the main application to the file "UniversalIndentGUI.ini".
    Settings are for example last selected indenter, last loaded config file and so on.
*/
bool UiGuiSettings::saveSettings() {
    QFileInfo fileInfo(currentSourceFile);
    if ( fileInfo.isFile() ) {
        settings->setValue( "UniversalIndentGUI/lastSourceCodeFile", currentSourceFile );
    }
	settings->setValue( "UniversalIndentGUI/loadLastSourceCodeFileOnStartup", loadLastSourceCodeFileOnStartup );
    settings->setValue( "UniversalIndentGUI/lastSelectedIndenter", currentIndenterID );
    settings->setValue( "UniversalIndentGUI/indenterParameterTooltipsEnabled", indenterParameterTooltipsEnabled );
    settings->setValue( "UniversalIndentGUI/language", language );
	settings->setValue( "UniversalIndentGUI/encoding", currentEncoding );
    settings->setValue( "UniversalIndentGUI/version", version );
	settings->setValue( "UniversalIndentGUI/maximized", parent->isMaximized() );
	if ( !parent->isMaximized() ) {
		settings->setValue( "UniversalIndentGUI/position", parent->pos() );
		settings->setValue( "UniversalIndentGUI/size", parent->size() );
	}
    settings->setValue( "UniversalIndentGUI/whiteSpaceIsVisible", whiteSpaceIsVisible );
    settings->setValue( "UniversalIndentGUI/tabWidth", tabWidth );
    //highlighter->writeCurrentSettings("");

    return true;
}