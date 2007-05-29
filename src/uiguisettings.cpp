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
    Extern widgets can connect to this slot to change settings. The type of the
    object by whom the connected signal was emitted is identified and the
    name and value of the object acquired. By the name the corresponding
    setting is set.
 */
void UiGuiSettings::handleValueChangeFromExtern() {
    if ( sender() ) {
        // Get the objects name and remove "uiGui" from its beginning.
        QString objectName = sender()->objectName();
        objectName.remove(0,5);

        // Set the value of the setting to the objects value.
        setValueByName( objectName, getValueOfQObject(sender()) );
    }
}


/*!
	Registers the \a qobject to be updated if the setting with the name \a settingName
	changes its value. Also it immediatly sets the value of \a qobject to the current
    value of \a settingName.
 */
void UiGuiSettings::registerForUpdateOnValueChange( QObject* qobject, QString settingName ) {
	// Test if the named setting really exists.
	if ( settings.contains(settingName) ) {
        // Add the object to the list for the setting to be updated on value change.
		forOnValueChangeRegisteredObjects[settingName].append(qobject);
        // Set the objects value to the current value of the setting.
        setValueOfQObject( qobject, settings[settingName] );
	}
}


/*!
    Gets the value of the \a qobject, if it is either a QCheckBox, QSpinBox or QAction,
    and returns it as QVariant.
 */
QVariant UiGuiSettings::getValueOfQObject(QObject* qobject) {
    // Get the objects class name.
    QString className( qobject->metaObject()->className() );

    if ( className == "QCheckBox" ) {
        QCheckBox* checkBox = dynamic_cast<QCheckBox *>(qobject);
        if ( checkBox != NULL ) {
            return checkBox->isChecked();
        }
    }
    else if ( className == "QSpinBox" ) {
        QSpinBox* spinBox = dynamic_cast<QSpinBox *>(qobject);
        if ( spinBox != NULL ) {
            return spinBox->value();
        }
    }
    else if ( className == "QAction" ) {
        QAction* action = dynamic_cast<QAction *>(qobject);
        if ( action != NULL ) {
            return action->isChecked();
        }
    }

    return -1;
}


/*!
    Sets the value of the \a qobject, if it is either a QCheckBox, QSpinBox or QAction,
    to \a value.
 */
bool UiGuiSettings::setValueOfQObject(QObject* qobject, QVariant value) {
    bool couldSetValue = true;

    // Get the objects class name.
    QString className( qobject->metaObject()->className() );

    if ( className == "QCheckBox" ) {
        QCheckBox* checkBox = dynamic_cast<QCheckBox *>(qobject);
        if ( checkBox != NULL ) {
            checkBox->setChecked( value.toBool() );
        }
    }
    else if ( className == "QSpinBox" ) {
        QSpinBox* spinBox = dynamic_cast<QSpinBox *>(qobject);
        if ( spinBox != NULL ) {
            spinBox->setValue( value.toInt() );
        }
    }
    else if ( className == "QAction" ) {
        QAction* action = dynamic_cast<QAction *>(qobject);
        if ( action != NULL ) {
            action->setChecked( value.toBool() );
        }
    }
    // The object type was none of ones this function can handle.
    else {
        couldSetValue = false;
    }

    return couldSetValue;
}

/*!
	Sets the value of the by \a settingsName defined setting to the value \a value.
    Also behaves like a signal, because if a value is changed to a value other than
    it has been before, all registered objects for the setting \a settingName
    in the map \sa forOnValueChangeRegisteredObjects will get their values updated.
 */
bool UiGuiSettings::setValueByName(QString settingName, QVariant value) {
	// Test if the named setting really exists.
	if ( settings.contains(settingName) ) {
        // Test if the new value is different to the one before.
        if ( settings[settingName] != value ) {
            // Set the new value.
		    settings[settingName] = value;

            // Update all objects that are registered for an update on this settings value change.
            foreach( QObject* qobject, forOnValueChangeRegisteredObjects[settingName] ) {
                setValueOfQObject(qobject, value);
            }
        }
		return true;
	}
    return false;
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
