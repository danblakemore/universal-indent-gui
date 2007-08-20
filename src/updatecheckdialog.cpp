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

#include "updatecheckdialog.h"

UpdateCheckDialog::UpdateCheckDialog(QString currentVersion, UiGuiSettings *settings, QWidget *parent) : QDialog(parent) {
    setupUi(this);

    manualUpdateRequested = false;

    http = new QHttp(this);
    connect( http, SIGNAL(done(bool)), this, SLOT(checkForUpdatedReturned(bool)) );

    this->currentVersion = currentVersion;
    this->settings = settings;

    setModal(true);
    checkingWidget->show();
    newVersionAvailableWidget->hide();
    noNewVersionAvailableWidget->hide();
}


void UpdateCheckDialog::updateCheckManuallyInvoked() {
    manualUpdateRequested = true;
    checkingWidget->show();
    newVersionAvailableWidget->hide();
    noNewVersionAvailableWidget->hide();
    show();
    adjustSize();
    checkForUpdate();
}


void UpdateCheckDialog::updateCheckAutomaticallyInvoked() {
    manualUpdateRequested = false;
    checkForUpdate();
}


/*!
    This function checks whether updates for UniversalIndentGUI are available.
 */
void UpdateCheckDialog::checkForUpdate() {
    http->setHost("universalindent.sourceforge.net");
    http->get("/universalindentgui_pad.xml");
}


/*!
    This slot is called after the update check has returned. Shows a message if check was successful or not.
    Offers to go to the download page if a newer version exists.
 */
void UpdateCheckDialog::checkForUpdatedReturned(bool errorOccurred) {
    if ( !errorOccurred ) {
        // Try to find the version string.
        QString returnedString = http->readAll();
        int leftPosition = returnedString.indexOf("<Program_Version>");
        int rightPosition = returnedString.indexOf("</Program_Version>");

        // If the version string could be found in the returned string, show an update dialog and set last update check date.
        if ( leftPosition != -1 && rightPosition != -1 ) {

            // Get the pure version string from returned string.
            returnedString = returnedString.mid( leftPosition+17, rightPosition-(leftPosition+17) );

            // Only show update dialog, if the current version string is not equal to the received one.
            if ( returnedString != currentVersion ) {
                checkingWidget->hide();
                newVersionAvailableWidget->show();
                noNewVersionAvailableWidget->hide();
                adjustSize();

                // Show message box whether to download the new version.
                int ret = QMessageBox::question(this, tr("Update available"), 
                    tr("A newer version of UniversalIndentGUI is available.\nYour version is %1. New version is %2.\nDo you want to go to the download website?").arg(currentVersion).arg(returnedString),
                    QMessageBox::Yes | QMessageBox::Default,
                    QMessageBox::No);
                // If yes clicked, open the download url in the default browser.
                if (ret == QMessageBox::Yes) {
                    QDesktopServices::openUrl( QUrl("http://sourceforge.net/project/showfiles.php?group_id=167482") );
                }
            }
            else if ( manualUpdateRequested ) {
                checkingWidget->hide();
                newVersionAvailableWidget->hide();
                noNewVersionAvailableWidget->show();
                adjustSize();

                QMessageBox::information(this, tr("No new update available"), 
                    tr("You already have the latest version of UniversalIndentGUI."), QMessageBox::Ok | QMessageBox::Default);
            }
            // Set last update check date.
            settings->setValueByName("LastUpdateCheck", QDate::currentDate());
        }
        // In the returned string, the version string could not be found.
        else {
            QMessageBox::warning(this, "Update check error", "There was an error while trying to check for an update! The retrieved file did not contain expected content." );
        }
    }
    // There was some error while trying to retrieve the update info from server.
    else {
        QMessageBox::warning(this, "Update check error", "There was an error while trying to check for an update! Error was : " + http->errorString() );
    }
    manualUpdateRequested = false;
}
