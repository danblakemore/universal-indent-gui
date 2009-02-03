/***************************************************************************
*   Copyright (C) 2006-2009 by Thomas Schweitzer                          *
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

#include "UiGuiSettingsDialog.h"

/*!
    \class UiGuiSettingsDialog
    \ingroup grp_Settings
    \brief Displays a dialog window with settings for UniversalIndentGUI
*/

/*!
    \brief The constructor calls the setup function for the ui created by uic. and adds
*/
UiGuiSettingsDialog::UiGuiSettingsDialog(QWidget* parent, UiGuiSettings* settings) : QDialog(parent) {
    // Remember pointer to the UiGuiSettings object.
    this->settings = settings;

    // Init the user interface created by the UIC.
    setupUi(this);

    //TODO: This call has to be removed when the properties for the highlighters can be set
    // with the settings dialog.
    groupBoxSyntaxHighlighterProperties->setToolTip( "(Will be implemented soon)" + groupBoxSyntaxHighlighterProperties->toolTip() );

    // Get all check boxes that are used for settings.
    checkBoxes = findChildren<QCheckBox*>( QRegExp("uiGui*") );

    // Get all spin boxes that are used for settings.
    spinBoxes = findChildren<QSpinBox*>( QRegExp("uiGui*") );

    // Get all combo boxes that are used for settings.
    comboBoxes = findChildren<QComboBox*>( QRegExp("uiGui*") );

    // Connect the accepted signal to own function, to write values back to the UiGuiSettings object.
    connect(this, SIGNAL(accepted()), this, SLOT(writeWidgetValuesToSettings()) );

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
    uiGuiLanguageSelectionComboBox->clear();

    // Now add an entry into the box for every language short.
    foreach (QString languageShort, settings->getAvailableTranslations() ) {
        // Identify the language mnemonic and set the full name.
        if ( languageShort == "en" ) {
            uiGuiLanguageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("English") );
        }
        else if ( languageShort == "fr" ) {
            uiGuiLanguageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("French") );
        }
        else if ( languageShort == "de" ) {
            uiGuiLanguageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("German") );
        }
        else if ( languageShort == "zh_TW" ) {
            uiGuiLanguageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("Chinese (Taiwan)") );
        }
        else if ( languageShort == "ja_jp" ) {
            uiGuiLanguageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("Japanese") );
        }
        else if ( languageShort == "ru" ) {
            uiGuiLanguageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("Russian") );
        }
        else if ( languageShort == "uk" ) {
            uiGuiLanguageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("Ukrainian") );
        }

        else {
            uiGuiLanguageSelectionComboBox->addItem( tr("Unknown language mnemonic ") + languageShort );
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
        // Get the corresponding setting name from the check boxs property and remove "DONOTTRANSLATE:" from its beginning.
        QString settingName = checkBox->property("connectedSettingName").toString().remove(0, 15);
        // If the property is not set, try using the objects name for convenience.
        if ( settingName.isEmpty() ) {
            // Get the objects name and remove "uiGui" from its beginning and use that as setting name.
            settingName = checkBox->objectName();
            settingName.remove(0,5);
        }

        // Get value from settings and assign it to the checkbox.
        bool value = settings->getValueByName( settingName ).toBool();
        checkBox->setChecked(value);
    }

    // Get the values for the spin boxes from the settings object.
    foreach (QSpinBox* spinBox, spinBoxes) {
        // Get the corresponding setting name from the spin boxs property and remove "DONOTTRANSLATE:" from its beginning.
        QString settingName = spinBox->property("connectedSettingName").toString().remove(0, 15);
        // If the property is not set, try using the objects name for convenience.
        if ( settingName.isEmpty() ) {
            // Get the objects name and remove "uiGui" from its beginning and use that as setting name.
            settingName = spinBox->objectName();
            settingName.remove(0,5);
        }

        // Get value from settings and assign it to the checkbox.
        int value = settings->getValueByName( settingName ).toInt();
        spinBox->setValue(value);
    }

    // Get the values for the combo boxes from the settings object.
    foreach (QComboBox* comboBox, comboBoxes) {
        // Get the corresponding setting name from the combo boxs property and remove "DONOTTRANSLATE:" from its beginning.
        QString settingName = comboBox->property("connectedSettingName").toString().remove(0, 15);
        // If the property is not set, try using the objects name for convenience.
        if ( settingName.isEmpty() ) {
            // Get the objects name and remove "uiGui" from its beginning and use that as setting name.
            settingName = comboBox->objectName();
            settingName.remove(0,5);
        }

        // Get value from settings and assign it to the checkbox.
        int value = settings->getValueByName( settingName ).toInt();
        comboBox->setCurrentIndex(value);
    }

    // Execute the dialog.
    return exec();
}


/*!
    \brief This slot is called when the dialog box is closed by pressing the Ok button.

    Writes all settings to the UiGuiSettings object.
 */
void UiGuiSettingsDialog::writeWidgetValuesToSettings() {
    // Write the values of the check boxes to the settings object.
    foreach (QCheckBox* checkBox, checkBoxes) {
        // Get the corresponding setting name from the check boxs property and remove "DONOTTRANSLATE:" from its beginning.
        QString settingName = checkBox->property("connectedSettingName").toString().remove(0, 15);
        // If the property is not set, try using the objects name for convenience.
        if ( settingName.isEmpty() ) {
            // Get the objects name and remove "uiGui" from its beginning and use that as setting name.
            settingName = checkBox->objectName();
            settingName.remove(0,5);
        }

        // Write the check box value to the settings.
        settings->setValueByName( settingName, checkBox->isChecked() );
    }

    // Write the values for the spin boxes to the settings object.
    foreach (QSpinBox* spinBox, spinBoxes) {
        // Get the corresponding setting name from the spin boxs property and remove "DONOTTRANSLATE:" from its beginning.
        QString settingName = spinBox->property("connectedSettingName").toString().remove(0, 15);
        // If the property is not set, try using the objects name for convenience.
        if ( settingName.isEmpty() ) {
            // Get the objects name and remove "uiGui" from its beginning and use that as setting name.
            settingName = spinBox->objectName();
            settingName.remove(0,5);
        }

        // Write the spin box value to the settings.
        settings->setValueByName( settingName, spinBox->value() );
    }

    // Write the values for the spin boxes to the settings object.
    foreach (QComboBox* comboBox, comboBoxes) {
        // Get the corresponding setting name from the combo boxs property and remove "DONOTTRANSLATE:" from its beginning.
        QString settingName = comboBox->property("connectedSettingName").toString().remove(0, 15);
        // If the property is not set, try using the objects name for convenience.
        if ( settingName.isEmpty() ) {
            // Get the objects name and remove "uiGui" from its beginning and use that as setting name.
            settingName = comboBox->objectName();
            settingName.remove(0,5);
        }

        // Write the spin box value to the settings.
        settings->setValueByName( settingName, comboBox->currentIndex() );
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

        QStringList languageShortList = settings->getAvailableTranslations();

        // Now retranslate every entry in the language selection box.
        for (int i = 0; i < languageShortList.size(); i++ ) {
            QString languageShort = languageShortList.at(i);

            // Identify the language mnemonic and set the full name.
            if ( languageShort == "en" ) {
                uiGuiLanguageSelectionComboBox->setItemText( i, tr("English") );
            }
            else if ( languageShort == "fr" ) {
                uiGuiLanguageSelectionComboBox->setItemText( i, tr("French") );
            }
            else if ( languageShort == "de" ) {
                uiGuiLanguageSelectionComboBox->setItemText( i, tr("German") );
            }
            else if ( languageShort == "zh_TW" ) {
                uiGuiLanguageSelectionComboBox->setItemText( i, tr("Chinese (Taiwan)") );
            }
            else if ( languageShort == "ja_jp" ) {
                uiGuiLanguageSelectionComboBox->setItemText( i, tr("Japanese") );
            }
            else if ( languageShort == "ru" ) {
                uiGuiLanguageSelectionComboBox->setItemText( i, tr("Russian") );
            }
            else if ( languageShort == "uk" ) {
                uiGuiLanguageSelectionComboBox->setItemText( i, tr("Ukrainian") );
            }
            else {
                uiGuiLanguageSelectionComboBox->setItemText( i, tr("Unknown language mnemonic ") + languageShort );
            }
        }
    }
    else {
        QWidget::changeEvent(event);
    }
}
