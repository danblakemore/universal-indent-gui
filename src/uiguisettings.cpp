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
	The constructor for the settings.
*/
UiGuiSettings::UiGuiSettings(QString settingFilePath) : QObject() {
    qsettings = new QSettings(settingFilePath, QSettings::IniFormat, this);
	loadSettings();
}


/*!
    The destructor saves the settings to a file.
 */
UiGuiSettings::~UiGuiSettings() {
	//FIXME: Is never called!!
    saveSettings();
}


/*!
    Extern widgets can connect to this slot to change settings. 
    According to the objects name the corresponding setting is known and set.
 */
void UiGuiSettings::handleValueChangeFromExtern(int value) {
    if ( sender() ) {
        // Get the objects name and remove "uiGui" from its beginning.
        QString objectName = sender()->objectName();
        objectName.remove(0,5);

        // Set the value of the setting to the objects value.
        setValueByName( objectName, value );
    }
}


/*!
    Extern widgets can connect to this slot to change settings. 
    According to the objects name the corresponding setting is known and set.
 */
void UiGuiSettings::handleValueChangeFromExtern(bool value) {
    if ( sender() ) {
        // Get the objects name and remove "uiGui" from its beginning.
        QString objectName = sender()->objectName();
        objectName.remove(0,5);

        // Set the value of the setting to the objects value.
        setValueByName( objectName, value );
    }
}


/*!
	Sets the value of the by \a settingsName defined setting to the value \a value.
    The to \a settingsName corresponding signal is emitted, if the value has changed.
 */
bool UiGuiSettings::setValueByName(QString settingName, QVariant value) {
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
    Emits the correct signal for the given \a settingName. If \a settingName
    equals "all", all signals are emitted. This can be used to update all
    dependend widgets. \a value is the new value that is emitted along with the signal.
 */
void UiGuiSettings::emitSignalForSetting(QString settingName) {
    // Emit the signal for the changed value.
    if ( settingName == "VersionInSettingsFile" ) emit versionInSettingsFile( settings[settingName].toString() );
    else if ( settingName == "WindowIsMaximized" ) emit windowIsMaximized( settings[settingName].toBool() );
    else if ( settingName == "WindowPosition" ) emit windowPosition( settings[settingName].toPoint() );
    else if ( settingName == "WindowSize" ) emit windowSize( settings[settingName].toSize() );
    else if ( settingName == "FileEncoding" ) emit fileEncoding( settings[settingName].toString() );
    else if ( settingName == "LoadLastOpenedFileOnStartup" ) emit loadLastOpenedFileOnStartup( settings[settingName].toBool() );
    else if ( settingName == "LastOpenedFile" ) emit lastOpenedFile( settings[settingName].toString() );
    else if ( settingName == "LastSelectedIndenterID" ) emit lastSelectedIndenterID( settings[settingName].toInt() );
    else if ( settingName == "SyntaxHighlightningEnabled" ) emit syntaxHighlightningEnabled( settings[settingName].toBool() );
    else if ( settingName == "WhiteSpaceIsVisible" ) emit whiteSpaceIsVisible( settings[settingName].toBool() );
    else if ( settingName == "IndenterParameterTooltipsEnabled" ) emit indenterParameterTooltipsEnabled( settings[settingName].toBool() );
    else if ( settingName == "TabWidth" ) emit tabWidth( settings[settingName].toInt() );
    else if ( settingName == "Language" ) emit language( settings[settingName].toString() );
    else if ( settingName == "all" ) {
        emit versionInSettingsFile( settings["VersionInSettingsFile"].toString() );
        emit windowIsMaximized( settings["WindowIsMaximized"].toBool() );
        emit windowPosition( settings["WindowPosition"].toPoint() );
        emit windowSize( settings["WindowSize"].toSize() );
        emit fileEncoding( settings["FileEncoding"].toString() );
        emit loadLastOpenedFileOnStartup( settings["LoadLastOpenedFileOnStartup"].toBool() );
        emit lastOpenedFile( settings["LastOpenedFile"].toString() );
        emit lastSelectedIndenterID( settings["LastSelectedIndenterID"].toInt() );
        emit syntaxHighlightningEnabled( settings["SyntaxHighlightningEnabled"].toBool() );
        emit whiteSpaceIsVisible( settings["WhiteSpaceIsVisible"].toBool() );
        emit indenterParameterTooltipsEnabled( settings["IndenterParameterTooltipsEnabled"].toBool() );
        emit tabWidth( settings["TabWidth"].toInt() );
        emit language( settings["Language"].toString() );
    }
}


/*!
    Calls \sa emitSignalForSetting with settingName "all" to update all widgets or whatever
    is connceted to each setting.
 */
void UiGuiSettings::updatedAllDependend() {
    emitSignalForSetting("all");
}


/*!
	Returns the value of the by \a settingsName defined setting as QVariant. 
	If the named setting does not exist, 0 is being returned.
*/
QVariant UiGuiSettings::getValueByName(QString settingName) {
	// Test if the named setting really exists.
	if ( settings.contains(settingName) ) {
		return settings[settingName];
	}
	return QVariant(0);
}


/*!
    Loads the settings for the main application.
	Settings are for example last selected indenter, last loaded source code file and so on.
*/
bool UiGuiSettings::loadSettings() {
	// Read the version string saved in the settings file.
	settings["VersionInSettingsFile"] = qsettings->value("UniversalIndentGUI/version", "").toString();

	// Read windows last size and position from the settings file.
	settings["WindowIsMaximized"] = qsettings->value( "UniversalIndentGUI/maximized", false ).toBool();
	settings["WindowPosition"] = qsettings->value( "UniversalIndentGUI/position", QPoint(0, 0) ).toPoint();
	settings["WindowSize"] = qsettings->value( "UniversalIndentGUI/size", QSize(800, 600) ).toSize();

	// Read last selected encoding for the opened source code file.
	settings["FileEncoding"] = qsettings->value( "UniversalIndentGUI/encoding", "UTF-8" ).toString();

	// Read if last opened source code file should be loaded on startup.
	settings["LoadLastOpenedFileOnStartup"] = qsettings->value("UniversalIndentGUI/loadLastSourceCodeFileOnStartup", true).toBool();

	// Read last opened source code file from the settings file.
	settings["LastOpenedFile"] = qsettings->value("UniversalIndentGUI/lastSourceCodeFile", settings["IndenterExecutableDir"].toString()+"example.cpp").toString();

	// Read last selected indenter from the settings file.
	int LastSelectedIndenterID = qsettings->value("UniversalIndentGUI/lastSelectedIndenter", 0).toInt();
	if ( LastSelectedIndenterID < 0 ) {
		LastSelectedIndenterID = 0;
	}
	settings["LastSelectedIndenterID"] = LastSelectedIndenterID;

    // Read if syntax highlightning is enabled.
	settings["SyntaxHighlightningEnabled"] = qsettings->value( "UniversalIndentGUI/SyntaxHighlightningEnabled", true ).toBool();

	// Read if white space characters should be displayed.
	settings["WhiteSpaceIsVisible"] = qsettings->value( "UniversalIndentGUI/whiteSpaceIsVisible", false ).toBool();

	// Read if indenter parameter tool tips are enabled.
	settings["IndenterParameterTooltipsEnabled"] = qsettings->value("UniversalIndentGUI/indenterParameterTooltipsEnabled", true).toBool();

	// Read the tab width from the settings file.
	settings["TabWidth"] = qsettings->value("UniversalIndentGUI/tabWidth", 4).toInt();

	// Read the last selected language.
	settings["Language"] = qsettings->value("UniversalIndentGUI/language", "").toString();

	return true;
}


/*!
    Saves the settings for the main application.
    Settings are for example last selected indenter, last loaded source code file and so on.
*/
bool UiGuiSettings::saveSettings() {
    qsettings->setValue( "UniversalIndentGUI/lastSourceCodeFile", settings["LastOpenedFile"] );
	qsettings->setValue( "UniversalIndentGUI/loadLastSourceCodeFileOnStartup", settings["LoadLastOpenedFileOnStartup"] );
    qsettings->setValue( "UniversalIndentGUI/lastSelectedIndenter", settings["LastSelectedIndenterID"] );
    qsettings->setValue( "UniversalIndentGUI/indenterParameterTooltipsEnabled", settings["IndenterParameterTooltipsEnabled"] );
    qsettings->setValue( "UniversalIndentGUI/language", settings["Language"] );
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

    return true;
}
