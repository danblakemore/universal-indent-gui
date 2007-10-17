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

#include "uiguierrormessage.h"

UiGuiErrorMessage::UiGuiErrorMessage(QWidget *parent) : QErrorMessage(parent) {
    showAgainCheckBox = findChild<QCheckBox *>();
}

UiGuiErrorMessage::~UiGuiErrorMessage(void) {
}

void UiGuiErrorMessage::showMessage( const QString &message ) {
    bool showAgain = true;

    if ( showAgainCheckBox != 0 ) {
        showAgain = showAgainCheckBox->isChecked();
    }

    if ( !errorMessageList.contains(message) ) {
        errorMessageList << message;
        if ( showAgainCheckBox != 0 ) {
            showAgainCheckBox->setChecked(true);
        }
        QErrorMessage::showMessage( message );
    }
    else if ( showAgain ) {
        QErrorMessage::showMessage( message );
    }
    
}
