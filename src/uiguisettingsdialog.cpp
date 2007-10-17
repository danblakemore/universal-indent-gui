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

#include "uiguisettingsdialog.h"

/*!
	\class UiGuiSettingsDialog
    \ingroup grp_Settings
	\brief Displays a dialog window with settings for UniversalIndentGUI
*/

/*!
	\brief The constructor calls the setup function for the ui created by uic. and adds
*/
UiGuiSettingsDialog::UiGuiSettingsDialog(QWidget* parent, UiGuiSettings* settings) : QDialog(parent)
{
    // Remember pointer to the UiGuiSettings object.
    this->settings = settings;

    // Init the user interface created by the UIC.
	setupUi(this);
	//TODO: This has to be removed when the properties for the highlighters can be set.
	groupBoxSyntaxHighlighterProperties->setToolTip( "(Will be implemented soon)" + groupBoxSyntaxHighlighterProperties->toolTip() );

    // Get all check boxes that are used for settings.
    checkBoxes = findChildren<QCheckBox*>( QRegExp("uiGui*") );

    // Get all spin boxes that are used for settings.
    spinBoxes = findChildren<QSpinBox*>( QRegExp("uiGui*") );

    // Get all combo boxes that are used for settings.
    comboBoxes = findChildren<QComboBox*>( QRegExp("uiGui*") );

    // Connect the accepted signal to own function, to write values back to the UiGuiSettings object.
    connect(this, SIGNAL(accepted()), this, SLOT(handleAccepted()) );

	// Init the language selection combobox.
	initTranslationSelection();
}

/*!
	\brief By calling this function the combobox for selecting the application language will
	be initialized. 
    
    Also the translation itself will be reinitialized.
 */
void UiGuiSettingsDialog::initTranslationSelection() {
	// First empty the combo box.
	uiGuiLanguage->clear();
	
	// Now add an entry into the box for every language short.
	foreach (QString languageShort, settings->getAvailableTranslations() ) {
		// Identify the language mnemonic and set the full name.
		if ( languageShort == "en" ) {
			uiGuiLanguage->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("English") );
		}
		else if ( languageShort == "de" ) {
			uiGuiLanguage->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("German") );
		}
		else if ( languageShort == "zh_TW" ) {
			uiGuiLanguage->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("Chinese (Taiwan)") );
		}
		else if ( languageShort == "ja_JP" ) {
			uiGuiLanguage->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("Japanese") );
		}
		else {
			uiGuiLanguage->addItem( tr("Unknown language mnemonic ") + languageShort );
		}
	}
}


/*!
    \brief Displays the dialog by calling the dialogs exec function. 
    
    Before it gets all the values needed from the UiGuiSettings object.
 */
int UiGuiSettingsDialog::showDialog() {
    // Get the values for the check boxes from the settings object.
    foreach (QCheckBox* checkBox, checkBoxes) {
        // Get the objects name and remove "uiGui" from its beginning.
        QString objectName = checkBox->objectName();
        objectName.remove(0,5);

        // Get value from settings and assign it to the checkbox.
        bool value = settings->getValueByName( objectName ).toBool();
        checkBox->setChecked(value);
    }

    // Get the values for the spin boxes from the settings object.
    foreach (QSpinBox* spinBox, spinBoxes) {
        // Get the objects name and remove "uiGui" from its beginning.
        QString objectName = spinBox->objectName();
        objectName.remove(0,5);

        // Get value from settings and assign it to the checkbox.
        int value = settings->getValueByName( objectName ).toInt();
        spinBox->setValue(value);
    }

    // Get the values for the combo boxes from the settings object.
    foreach (QComboBox* comboBox, comboBoxes) {
        // Get the objects name and remove "uiGui" from its beginning.
        QString objectName = comboBox->objectName();
        objectName.remove(0,5);

        // Get value from settings and assign it to the checkbox.
        int value = settings->getValueByName( objectName ).toInt();
        comboBox->setCurrentIndex(value);
    }

    // Execute the dialog.
    return exec();
}


/*!
    \brief This slot is called when the dialog box is closed by pressing the Ok button.

    Writes all settings to the UiGuiSettings object.
 */
void UiGuiSettingsDialog::handleAccepted() {
    // Write the values of the check boxes to the settings object.
    foreach (QCheckBox* checkBox, checkBoxes) {
        // Get the objects name and remove "uiGui" from its beginning.
        QString objectName = checkBox->objectName();
        objectName.remove(0,5);

        // Write the check box value to the settings.
        settings->setValueByName( objectName, checkBox->isChecked() );
    }

    // Write the values for the spin boxes to the settings object.
    foreach (QSpinBox* spinBox, spinBoxes) {
        // Get the objects name and remove "uiGui" from its beginning.
        QString objectName = spinBox->objectName();
        objectName.remove(0,5);

        // Write the spin box value to the settings.
        settings->setValueByName( objectName, spinBox->value() );
    }

    // Write the values for the spin boxes to the settings object.
    foreach (QComboBox* comboBox, comboBoxes) {
        // Get the objects name and remove "uiGui" from its beginning.
        QString objectName = comboBox->objectName();
        objectName.remove(0,5);

        // Write the spin box value to the settings.
        settings->setValueByName( objectName, comboBox->currentIndex() );
    }
}


/*!
    \brief Catches language change events and retranslates all needed widgets.
 */
void UiGuiSettingsDialog::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi(this);
        // If this is not explicit set here, Qt < 4.3.0 does not translate the buttons.
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

		//TODO: This has to be removed when the properties for the highlighters can be set.
		groupBoxSyntaxHighlighterProperties->setToolTip( "(Will be implemented soon)" + groupBoxSyntaxHighlighterProperties->toolTip() );
		uiGuiRecentlyOpenedListSize->setToolTip( "(Will be implemented soon)" + uiGuiRecentlyOpenedListSize->toolTip() );

        QStringList languageShortList = settings->getAvailableTranslations();

        // Now retranslate every entry in the language selection box.
        for (int i = 0; i < languageShortList.size(); i++ ) {
            QString languageShort = languageShortList.at(i);

            // Identify the language mnemonic and set the full name.
            if ( languageShort == "en" ) {
                uiGuiLanguage->setItemText( i, tr("English") );
            }
            else if ( languageShort == "de" ) {
                uiGuiLanguage->setItemText( i, tr("German") );
            }
            else if ( languageShort == "zh_TW" ) {
                uiGuiLanguage->setItemText( i, tr("Chinese (Taiwan)") );
            }
            else if ( languageShort == "ja_JP" ) {
                uiGuiLanguage->setItemText( i, tr("Japanese") );
            }
            else {
                uiGuiLanguage->setItemText( i, tr("Unknown language mnemonic ") + languageShort );
            }
        }
    } 
    else {
        QWidget::changeEvent(event);
    }
}
