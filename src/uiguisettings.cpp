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

#include "uiguisettings.h"

#include "SettingsPaths.h"

//! \defgroup grp_Settings All concerning the settings.

/*!
	\class UiguiSettings
    \ingroup grp_Settings
	\brief Handles the settings of the program. Reads them on startup and saves them on exit.
    Is a singleton class and can only be accessed via getInstance().
*/

// Inits the single class instance pointer.
 UiguiSettings* UiguiSettings::instance = NULL;

/*!
	\brief The constructor for the settings.
*/
UiguiSettings::UiguiSettings() : QObject() {
    // Create the main application settings object from the UniversalIndentGUI.ini file.
    qsettings = new QSettings(SettingsPaths::getSettingsPath() + "/UniversalIndentGUI.ini", QSettings::IniFormat, this);

    indenterDirctoryStr = SettingsPaths::getGlobalFilesPath() + "/indenters";
	readAvailableTranslations();
	loadSettings();
}


/*!
    \brief Returns the instance of the settings class. If no instance exists, ONE will be created.
 */
UiguiSettings* UiguiSettings::getInstance() {
    if ( instance == NULL ) {
        // Create the settings object, which loads all UiGui settings from a file.
    	instance = new UiguiSettings();
    }

    return instance;
}


void UiguiSettings::deleteInstance() {
    if ( instance != NULL ) {
        delete instance;
        instance = NULL;
    }
}


/*!
    \brief The destructor saves the settings to a file.
 */
UiguiSettings::~UiguiSettings() {
    saveSettings();
}


/*!
	\brief Scans the translations directory for available translation files and 
    stores them in the QList \a availableTranslations.
 */
void UiguiSettings::readAvailableTranslations() {
	QString languageShort;
	QStringList languageFileList;

	// English is the default language. A translation file does not exist but to have a menu entry, added here.
	languageFileList << "universalindent_en.qm";

	// Find all translation files in the "translations" directory.
	QDir translationDirectory = QDir( SettingsPaths::getGlobalFilesPath() + "/translations" );
	languageFileList << translationDirectory.entryList( QStringList("universalindent_*.qm") );

	// Loop for each found translation file
	foreach ( languageShort, languageFileList ) {
		// Remove the leading string "universalindent_" from the filename.
		languageShort.remove(0,16);
		// Remove trailing file extension ".qm".
		languageShort.chop(3);

		availableTranslations.append(languageShort);
	}
}


/*!
	\brief Returns a list of the mnemonics of the available translations.
 */
QStringList UiguiSettings::getAvailableTranslations() {
	return availableTranslations;
}


/*!
    \brief Extern widgets can connect to this slot to change settings.

    According to the objects property "connectedSettingName" the corresponding
    setting is known and set.
 */
void UiguiSettings::handleValueChangeFromExtern(int value) {
    if ( sender() ) {
        // Get the corresponding setting name from the sender objects property and remove "DONOTTRANSLATE:" from its beginning..
        QString settingName = sender()->property("connectedSettingName").toString().remove(0, 15);
        // If the property is not set, try using the objects name for convenience.
        if ( settingName.isEmpty() ) {
            // Get the objects name and remove "uiGui" from its beginning and use that as setting name.
            settingName = sender()->objectName();
            settingName.remove(0,5);
        }

        // Set the value of the setting to the objects value.
        setValueByName( settingName, value );
    }
}


/*!
    \brief Extern widgets can connect to this slot to change settings.

    According to the objects property "connectedSettingName" the corresponding
    setting is known and set.
 */
void UiguiSettings::handleValueChangeFromExtern(bool value) {
    if ( sender() ) {
        // Get the corresponding setting name from the sender objects property and remove "DONOTTRANSLATE:" from its beginning.
        QString settingName = sender()->property("connectedSettingName").toString().remove(0, 15);
        // If the property is not set, try using the objects name for convenience.
        if ( settingName.isEmpty() ) {
            // Get the objects name and remove "uiGui" from its beginning and use that as setting name.
            settingName = sender()->objectName();
            settingName.remove(0,5);
        }

        // Set the value of the setting to the objects value.
        setValueByName( settingName, value );
    }
}


/*!
    \brief Extern widgets can connect to this slot to change settings.

    According to the objects property "connectedSettingName" the corresponding
    setting is known and set.
 */
void UiguiSettings::handleValueChangeFromExtern(QDate value) {
    if ( sender() ) {
        // Get the corresponding setting name from the sender objects property and remove "DONOTTRANSLATE:" from its beginning.
        QString settingName = sender()->property("connectedSettingName").toString().remove(0, 15);
        // If the property is not set, try using the objects name for convenience.
        if ( settingName.isEmpty() ) {
            // Get the objects name and remove "uiGui" from its beginning and use that as setting name.
            settingName = sender()->objectName();
            settingName.remove(0,5);
        }

        // Set the value of the setting to the objects value.
        setValueByName( settingName, value );
    }
}


/*!
    \brief Extern widgets can connect to this slot to change settings.

    According to the objects property "connectedSettingName" the corresponding
    setting is known and set.
 */
void UiguiSettings::handleValueChangeFromExtern(QByteArray value) {
    if ( sender() ) {
        // Get the corresponding setting name from the sender objects property.
        QString settingName = sender()->property("connectedSettingName").toString();
        // If the property is not set, try using the objects name for convenience.
        if ( settingName.isEmpty() ) {
            // Get the objects name and remove "uiGui" from its beginning and use that as setting name.
            settingName = sender()->objectName();
            settingName.remove(0,5);
        }

        // Set the value of the setting to the objects value.
        setValueByName( settingName, value );
    }
}


/*!
	\brief Sets the value of the by \a settingsName defined setting to the value \a value.

    The to \a settingsName corresponding signal is emitted, if the value has changed.
 */
bool UiguiSettings::setValueByName(QString settingName, QVariant value) {
	// Test if the named setting really exists.
	if ( settings.contains(settingName) ) {
        // Test if the new value is different to the one before.
        if ( settings[settingName] != value ) {
            // Set the new value.
		    settings[settingName] = value;
            // Emit the signal for the changed setting.
            emitSignalForSetting(settingName);
        }
		return true;
	}
    return false;
}


/*!
    \brief Emits the correct signal for the given \a settingName. 
    
    If \a settingName equals "all", all signals are emitted. This can be used to update all
    dependent widgets. \a value is the new value that is emitted along with the signal.
 */
void UiguiSettings::emitSignalForSetting(QString settingName) {
    // Emit the signal for the changed value.
    if ( settingName == "VersionInSettingsFile" ) emit versionInSettingsFile( settings[settingName].toString() );
    else if ( settingName == "WindowIsMaximized" ) emit windowIsMaximized( settings[settingName].toBool() );
    else if ( settingName == "WindowPosition" ) emit windowPosition( settings[settingName].toPoint() );
    else if ( settingName == "WindowSize" ) emit windowSize( settings[settingName].toSize() );
    else if ( settingName == "FileEncoding" ) emit fileEncoding( settings[settingName].toString() );
    else if ( settingName == "RecentlyOpenedListSize" ) emit recentlyOpenedListSize( settings[settingName].toInt() );
    else if ( settingName == "LoadLastOpenedFileOnStartup" ) emit loadLastOpenedFileOnStartup( settings[settingName].toBool() );
    else if ( settingName == "LastOpenedFiles" ) emit lastOpenedFiles( settings[settingName].toString() );
    else if ( settingName == "SelectedIndenter" ) emit selectedIndenter( settings[settingName].toInt() );
    else if ( settingName == "SyntaxHighlightningEnabled" ) emit syntaxHighlightningEnabled( settings[settingName].toBool() );
    else if ( settingName == "WhiteSpaceIsVisible" ) emit whiteSpaceIsVisible( settings[settingName].toBool() );
    else if ( settingName == "IndenterParameterTooltipsEnabled" ) emit indenterParameterTooltipsEnabled( settings[settingName].toBool() );
    else if ( settingName == "TabWidth" ) emit tabWidth( settings[settingName].toInt() );
    else if ( settingName == "Language" ) emit language( settings[settingName].toInt() );
    else if ( settingName == "CheckForUpdate" ) emit checkForUpdate( settings[settingName].toBool() );
    else if ( settingName == "LastUpdateCheck" ) emit lastUpdateCheck( settings[settingName].toDate() );
    else if ( settingName == "MainWindowState" ) emit mainWindowState( settings[settingName].toByteArray() );
    else if ( settingName == "all" ) {
        emit versionInSettingsFile( settings["VersionInSettingsFile"].toString() );
        emit windowIsMaximized( settings["WindowIsMaximized"].toBool() );
        emit windowPosition( settings["WindowPosition"].toPoint() );
        emit windowSize( settings["WindowSize"].toSize() );
        emit fileEncoding( settings["FileEncoding"].toString() );
        emit recentlyOpenedListSize( settings["RecentlyOpenedListSize"].toInt() );
        emit loadLastOpenedFileOnStartup( settings["LoadLastOpenedFileOnStartup"].toBool() );
        emit lastOpenedFiles( settings["LastOpenedFiles"].toString() );
        emit selectedIndenter( settings["SelectedIndenter"].toInt() );
        emit syntaxHighlightningEnabled( settings["SyntaxHighlightningEnabled"].toBool() );
        emit whiteSpaceIsVisible( settings["WhiteSpaceIsVisible"].toBool() );
        emit indenterParameterTooltipsEnabled( settings["IndenterParameterTooltipsEnabled"].toBool() );
        emit tabWidth( settings["TabWidth"].toInt() );
        emit language( settings["Language"].toInt() );
        emit checkForUpdate( settings["CheckForUpdate"].toBool() );
        emit lastUpdateCheck( settings["LastUpdateCheck"].toDate() );
        emit mainWindowState( settings["MainWindowState"].toByteArray() );
    }
}


/*!
    \brief Calls \sa emitSignalForSetting with settingName "all" to update all widgets or whatever
    is connected to each setting.
 */
void UiguiSettings::updateAllDependend() {
    emitSignalForSetting("all");
}


/*!
	\brief Returns the value of the by \a settingsName defined setting as QVariant. 

	If the named setting does not exist, 0 is being returned.
*/
QVariant UiguiSettings::getValueByName(QString settingName) {
	// Test if the named setting really exists.
	if ( settings.contains(settingName) ) {
		return settings[settingName];
	}
	return QVariant(0);
}


/*!
    \brief Loads the settings for the main application.

	Settings are for example last selected indenter, last loaded source code file and so on.
*/
bool UiguiSettings::loadSettings() {
	// Read the version string saved in the settings file.
	settings["VersionInSettingsFile"] = qsettings->value("UniversalIndentGUI/version", "").toString();

	// Read windows last size and position from the settings file.
	settings["WindowIsMaximized"] = qsettings->value("UniversalIndentGUI/maximized", false).toBool();
	settings["WindowPosition"] = qsettings->value("UniversalIndentGUI/position", QPoint(50, 50)).toPoint();
	settings["WindowSize"] = qsettings->value("UniversalIndentGUI/size", QSize(800, 600)).toSize();

	// Read last selected encoding for the opened source code file.
	settings["FileEncoding"] = qsettings->value("UniversalIndentGUI/encoding", "UTF-8").toString();

    // Read maximum length of list for recently opened files.
	settings["RecentlyOpenedListSize"] = qsettings->value("UniversalIndentGUI/recentlyOpenedListSize", 5).toInt();

	// Read if last opened source code file should be loaded on startup.
	settings["LoadLastOpenedFileOnStartup"] = qsettings->value("UniversalIndentGUI/loadLastSourceCodeFileOnStartup", true).toBool();

	// Read last opened source code file from the settings file.
	settings["LastOpenedFiles"] = qsettings->value("UniversalIndentGUI/lastSourceCodeFile", indenterDirctoryStr+"/example.cpp").toString();

	// Read last selected indenter from the settings file.
	int SelectedIndenter = qsettings->value("UniversalIndentGUI/selectedIndenter", 0).toInt();
	if ( SelectedIndenter < 0 ) {
		SelectedIndenter = 0;
	}
	settings["SelectedIndenter"] = SelectedIndenter;

    // Read if syntax highlighting is enabled.
	settings["SyntaxHighlightningEnabled"] = qsettings->value("UniversalIndentGUI/SyntaxHighlightningEnabled", true).toBool();

	// Read if white space characters should be displayed.
	settings["WhiteSpaceIsVisible"] = qsettings->value("UniversalIndentGUI/whiteSpaceIsVisible", false).toBool();

	// Read if indenter parameter tool tips are enabled.
	settings["IndenterParameterTooltipsEnabled"] = qsettings->value("UniversalIndentGUI/indenterParameterTooltipsEnabled", true).toBool();

	// Read the tab width from the settings file.
	settings["TabWidth"] = qsettings->value("UniversalIndentGUI/tabWidth", 4).toInt();

	// Read the last selected language and stores the index it has in the list of available translations.
	settings["Language"] = availableTranslations.indexOf( qsettings->value("UniversalIndentGUI/language", "").toString() );

    // Read the update check settings from the settings file.
    settings["CheckForUpdate"] = qsettings->value("UniversalIndentGUI/CheckForUpdate", true).toBool();
    settings["LastUpdateCheck"] = qsettings->value("UniversalIndentGUI/LastUpdateCheck", QDate(1900,1,1)).toDate();

    // Read the main window state.
    settings["MainWindowState"] = qsettings->value("UniversalIndentGUI/MainWindowState", QByteArray()).toByteArray();

	return true;
}


/*!
    \brief Saves the settings for the main application.

    Settings are for example last selected indenter, last loaded source code file and so on.
*/
bool UiguiSettings::saveSettings() {
	qsettings->setValue( "UniversalIndentGUI/recentlyOpenedListSize", settings["RecentlyOpenedListSize"] );
    qsettings->setValue( "UniversalIndentGUI/lastSourceCodeFile", settings["LastOpenedFiles"] );
	qsettings->setValue( "UniversalIndentGUI/loadLastSourceCodeFileOnStartup", settings["LoadLastOpenedFileOnStartup"] );
    qsettings->setValue( "UniversalIndentGUI/selectedIndenter", settings["SelectedIndenter"] );
    qsettings->setValue( "UniversalIndentGUI/indenterParameterTooltipsEnabled", settings["IndenterParameterTooltipsEnabled"] );
    qsettings->setValue( "UniversalIndentGUI/language", availableTranslations[ settings["Language"].toInt() ] );
	qsettings->setValue( "UniversalIndentGUI/encoding", settings["FileEncoding"] );
    qsettings->setValue( "UniversalIndentGUI/version", settings["VersionInSettingsFile"] );
	qsettings->setValue( "UniversalIndentGUI/maximized", settings["WindowIsMaximized"] );
	if ( !settings["WindowIsMaximized"].toBool() ) {
		qsettings->setValue( "UniversalIndentGUI/position", settings["WindowPosition"] );
		qsettings->setValue( "UniversalIndentGUI/size", settings["WindowSize"] );
	}
    qsettings->setValue( "UniversalIndentGUI/SyntaxHighlightningEnabled", settings["SyntaxHighlightningEnabled"] );
    qsettings->setValue( "UniversalIndentGUI/whiteSpaceIsVisible", settings["WhiteSpaceIsVisible"] );
    qsettings->setValue( "UniversalIndentGUI/tabWidth", settings["TabWidth"] );
    // Write the update check settings to the settings file.
    qsettings->setValue("UniversalIndentGUI/CheckForUpdate", settings["CheckForUpdate"].toBool() );
    qsettings->setValue("UniversalIndentGUI/LastUpdateCheck", settings["LastUpdateCheck"].toDate() );
    // Write the main window state.
    qsettings->setValue("UniversalIndentGUI/MainWindowState", settings["MainWindowState"].toByteArray() );

    return true;
}
