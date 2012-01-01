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

#include "UiGuiErrorMessage.h"

#include <QCheckBox>

/*!
    \class UiGuiErrorMessage
    \ingroup grp_Dialogs
    \brief UiGuiErrorMessage is a child of QErrorMessage. But QErrorMessages
    "Do not show again" didn't work with my strings, so this is my own, working
    implementation of it.
*/


/*!
    \brief Initializes the dialog.

    Retrieves the object pointer to the \a _showAgainCheckBox check box, sets the dialogs
    modality and for a working translation sets the check box text.
 */
UiGuiErrorMessage::UiGuiErrorMessage(QWidget *parent) : QErrorMessage(parent) {
    _showAgainCheckBox = findChild<QCheckBox *>();
    setWindowModality( Qt::ApplicationModal );
    _showAgainCheckBox->setText( tr("Show this message again") );
}


/*!
    \brief Just a lazy nothin doin destructive destructor.
 */
UiGuiErrorMessage::~UiGuiErrorMessage(void) {
}


/*!
    \brief Shows an error \a message in a dialog box with \a title.

    The shown \a message is added to a list, if not already in there. If it is
    already in that list and "Show this message again" is not checked, that
    message will not be shown.
 */
void UiGuiErrorMessage::showMessage( const QString &title, const QString &message ) {
    bool showAgain = true;

    if ( _showAgainCheckBox != 0 ) {
        showAgain = _showAgainCheckBox->isChecked();
    }

    setWindowTitle(title);

    if ( !_errorMessageList.contains(message) ) {
        _errorMessageList << message;
        if ( _showAgainCheckBox != 0 ) {
            _showAgainCheckBox->setChecked(true);
        }
        QErrorMessage::showMessage( message );
    }
    else if ( showAgain ) {
        QErrorMessage::showMessage( message );
    }
}


/*!
    \brief For convinience, for showing a dialog box with the default title "UniversalIndentGUI".
 */
void UiGuiErrorMessage::showMessage( const QString &message ) {
    showMessage( "UniversalIndentGUI", message );
}
