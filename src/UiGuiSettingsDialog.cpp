/***************************************************************************
*   Copyright (C) 2006-2012 by Thomas Schweitzer                          *
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
#include "ui_UiGuiSettingsDialog.h"

#include "UiGuiSettings.h"

/*!
    \class UiGuiSettingsDialog
    \ingroup grp_Settings
    \brief Displays a dialog window with settings for UniversalIndentGUI
*/

/*!
    \brief The constructor calls the setup function for the ui created by uic.
*/
UiGuiSettingsDialog::UiGuiSettingsDialog(QWidget* parent, QSharedPointer<UiGuiSettings> settings) : QDialog(parent) {
    // Remember pointer to the UiGuiSettings object.
    _settings = settings;

    // Init the user interface created by the UIC.
    _settingsDialogForm = new Ui::SettingsDialog();
    _settingsDialogForm->setupUi(this);

    //TODO: This call has to be removed when the properties for the highlighters can be set
    // with the settings dialog.
    _settingsDialogForm->groupBoxSyntaxHighlighterProperties->setToolTip( "(Will be implemented soon)" + _settingsDialogForm->groupBoxSyntaxHighlighterProperties->toolTip() );

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
    _settingsDialogForm->languageSelectionComboBox->clear();

    // Now add an entry into the box for every language short.
    foreach (QString languageShort, _settings->getAvailableTranslations() ) {
        // Identify the language mnemonic and set the full name.
        if ( languageShort == "en" ) {
            _settingsDialogForm->languageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("English") );
        }
        else if ( languageShort == "fr" ) {
            _settingsDialogForm->languageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("French") );
        }
        else if ( languageShort == "de" ) {
            _settingsDialogForm->languageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("German") );
        }
        else if ( languageShort == "zh_TW" ) {
            _settingsDialogForm->languageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("Chinese (Taiwan)") );
        }
        else if ( languageShort == "ja" ) {
            _settingsDialogForm->languageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("Japanese") );
        }
        else if ( languageShort == "ru" ) {
            _settingsDialogForm->languageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("Russian") );
        }
        else if ( languageShort == "uk" ) {
            _settingsDialogForm->languageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("Ukrainian") );
        }

        else {
            _settingsDialogForm->languageSelectionComboBox->addItem( tr("Unknown language mnemonic ") + languageShort );
        }
    }
}


/*!
    \brief Displays the dialog by calling the dialogs exec function.

    Before it gets all the values needed from the UiGuiSettings object.
 */
int UiGuiSettingsDialog::showDialog() {
    // Init all settings dialog objects with values from settings.
    _settings->setObjectPropertyToSettingValueRecursive(this);

    // Execute the dialog.
    return exec();
}


/*!
    \brief This slot is called when the dialog box is closed by pressing the Ok button.

    Writes all settings to the UiGuiSettings object.
 */
void UiGuiSettingsDialog::writeWidgetValuesToSettings() {
    // Write settings dialog object values to settings.
    _settings->setSettingToObjectPropertyValueRecursive(this);
}


/*!
    \brief Catches language change events and retranslates all needed widgets.
 */
void UiGuiSettingsDialog::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        _settingsDialogForm->retranslateUi(this);
        // If this is not explicit set here, Qt < 4.3.0 does not translate the buttons.
        _settingsDialogForm->buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

        //TODO: This has to be removed when the properties for the highlighters can be set.
        _settingsDialogForm->groupBoxSyntaxHighlighterProperties->setToolTip( "(Will be implemented soon)" + _settingsDialogForm->groupBoxSyntaxHighlighterProperties->toolTip() );

        QStringList languageShortList = _settings->getAvailableTranslations();

        // Now retranslate every entry in the language selection box.
        for (int i = 0; i < languageShortList.size(); i++ ) {
            QString languageShort = languageShortList.at(i);

            // Identify the language mnemonic and set the full name.
            if ( languageShort == "en" ) {
                _settingsDialogForm->languageSelectionComboBox->setItemText( i, tr("English") );
            }
            else if ( languageShort == "fr" ) {
                _settingsDialogForm->languageSelectionComboBox->setItemText( i, tr("French") );
            }
            else if ( languageShort == "de" ) {
                _settingsDialogForm->languageSelectionComboBox->setItemText( i, tr("German") );
            }
            else if ( languageShort == "zh_TW" ) {
                _settingsDialogForm->languageSelectionComboBox->setItemText( i, tr("Chinese (Taiwan)") );
            }
            else if ( languageShort == "ja" ) {
                _settingsDialogForm->languageSelectionComboBox->setItemText( i, tr("Japanese") );
            }
            else if ( languageShort == "ru" ) {
                _settingsDialogForm->languageSelectionComboBox->setItemText( i, tr("Russian") );
            }
            else if ( languageShort == "uk" ) {
                _settingsDialogForm->languageSelectionComboBox->setItemText( i, tr("Ukrainian") );
            }
            else {
                _settingsDialogForm->languageSelectionComboBox->setItemText( i, tr("Unknown language mnemonic ") + languageShort );
            }
        }
    }
    else {
        QWidget::changeEvent(event);
    }
}
