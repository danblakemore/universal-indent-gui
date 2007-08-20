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
    roleOfClickedButton = QDialogButtonBox::InvalidRole;

    http = new QHttp(this);
    connect( http, SIGNAL(done(bool)), this, SLOT(checkForUpdatedReturned(bool)) );

    updateCheckProgressTimer = new QTimer(this);
    updateCheckProgressTimer->setInterval(100);
    connect( updateCheckProgressTimer, SIGNAL(timeout()), this, SLOT(updateUpdateCheckProgressBar()) );
    updateCheckProgressCounter = 0;

    connect( buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(handleDialogButtonClicked(QAbstractButton*)) );

    this->currentVersion = currentVersion;
    this->settings = settings;

    setModal(true);
}


void UpdateCheckDialog::updateCheckManuallyInvoked() {
    manualUpdateRequested = true;
    showCheckingForUpdateDialog();
    show();
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
    // Stop the progressbar timer.
    updateCheckProgressTimer->stop();

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
            if ( returnedString == currentVersion ) {
                // Show message box whether to download the new version.
                showNewVersionAvailableDialog(returnedString);

                // If yes clicked, open the download url in the default browser.
                if ( roleOfClickedButton == QDialogButtonBox::YesRole ) {
                    QDesktopServices::openUrl( QUrl("http://sourceforge.net/project/showfiles.php?group_id=167482") );
                }
            }
            else if ( manualUpdateRequested ) {
                showNoNewVersionAvailableDialog();
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


void UpdateCheckDialog::showCheckingForUpdateDialog() {
    updateCheckProgressTimer->start();
    progressBar->show();
    setWindowTitle( tr("Checking for update...") );
    label->setText( tr("Checking whether a newer version is available") );
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel);
}


void UpdateCheckDialog::showNewVersionAvailableDialog(QString newVersion) {
    progressBar->hide();
    setWindowTitle( tr("Update available") );
    label->setText( tr("A newer version of UniversalIndentGUI is available.\nYour version is %1. New version is %2.\nDo you want to go to the download website?").arg(currentVersion).arg(newVersion) );
    buttonBox->setStandardButtons(QDialogButtonBox::No|QDialogButtonBox::NoButton|QDialogButtonBox::Yes);
    exec();
}


void UpdateCheckDialog::showNoNewVersionAvailableDialog() {
    progressBar->hide();
    setWindowTitle( tr("No new update available") );
    label->setText( tr("You already have the latest version of UniversalIndentGUI.") );
    buttonBox->setStandardButtons(QDialogButtonBox::Ok);
    exec();
}


void UpdateCheckDialog::handleDialogButtonClicked(QAbstractButton *clickedButton) {
    roleOfClickedButton = buttonBox->buttonRole(clickedButton);
    accept();
}


void UpdateCheckDialog::updateUpdateCheckProgressBar() {
    progressBar->setValue(updateCheckProgressCounter);
    updateCheckProgressCounter++;
    //updateCheckProgressCounter = updateCheckProgressCounter % 100;
}
