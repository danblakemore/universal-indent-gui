/***************************************************************************
*   Copyright (C) 2006-2010 by Thomas Schweitzer                          *
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
    languageSelectionComboBox->clear();

    // Now add an entry into the box for every language short.
    foreach (QString languageShort, settings->getAvailableTranslations() ) {
        // Identify the language mnemonic and set the full name.
        if ( languageShort == "en" ) {
            languageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("English") );
        }
        else if ( languageShort == "fr" ) {
            languageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("French") );
        }
        else if ( languageShort == "de" ) {
            languageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("German") );
        }
        else if ( languageShort == "zh_TW" ) {
            languageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("Chinese (Taiwan)") );
        }
        else if ( languageShort == "ja_jp" ) {
            languageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("Japanese") );
        }
        else if ( languageShort == "ru" ) {
            languageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("Russian") );
        }
        else if ( languageShort == "uk" ) {
            languageSelectionComboBox->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("Ukrainian") );
        }

        else {
            languageSelectionComboBox->addItem( tr("Unknown language mnemonic ") + languageShort );
        }
    }
}


/*!
    \brief Displays the dialog by calling the dialogs exec function.

    Before it gets all the values needed from the UiGuiSettings object.
 */
int UiGuiSettingsDialog::showDialog() {
    // Init all settings dialog objects with values from settings.
    settings->setObjectPropertyToSettingValueRecursive(this);

    // Execute the dialog.
    return exec();
}


/*!
    \brief This slot is called when the dialog box is closed by pressing the Ok button.

    Writes all settings to the UiGuiSettings object.
 */
void UiGuiSettingsDialog::writeWidgetValuesToSettings() {
    // Write settings dialog object values to settings.
    settings->setSettingToObjectPropertyValueRecursive(this);
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
                languageSelectionComboBox->setItemText( i, tr("English") );
            }
            else if ( languageShort == "fr" ) {
                languageSelectionComboBox->setItemText( i, tr("French") );
            }
            else if ( languageShort == "de" ) {
                languageSelectionComboBox->setItemText( i, tr("German") );
            }
            else if ( languageShort == "zh_TW" ) {
                languageSelectionComboBox->setItemText( i, tr("Chinese (Taiwan)") );
            }
            else if ( languageShort == "ja_jp" ) {
                languageSelectionComboBox->setItemText( i, tr("Japanese") );
            }
            else if ( languageShort == "ru" ) {
                languageSelectionComboBox->setItemText( i, tr("Russian") );
            }
            else if ( languageShort == "uk" ) {
                languageSelectionComboBox->setItemText( i, tr("Ukrainian") );
            }
            else {
                languageSelectionComboBox->setItemText( i, tr("Unknown language mnemonic ") + languageShort );
            }
        }
    }
    else {
        QWidget::changeEvent(event);
    }
}
