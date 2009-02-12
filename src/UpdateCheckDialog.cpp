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

#include "UpdateCheckDialog.h"

#include "UiGuiVersion.h"
#include <QRegExpValidator>

/*!
    \class UpdateCheckDialog
    \ingroup grp_MainWindow
    \brief UpdateCheckDialog is a dialog widget that contains functions
    for online checking for a new version of UniversalIndentGUI.
*/

/*!
    \brief Initializes member variables and stores the version of UiGui and a pointer to the settings object.
 */
UpdateCheckDialog::UpdateCheckDialog(UiGuiSettings *settings, QWidget *parent) : QDialog(parent) {
    setupUi(this);

    manualUpdateRequested = false;
    roleOfClickedButton = QDialogButtonBox::InvalidRole;

    // Create object for http request and connect it with the request return handler.
    http = new QHttp(this);
    connect( http, SIGNAL(done(bool)), this, SLOT(checkResultsOfFetchedPadXMLFile(bool)) );

    // Create a timer object used for the progress bar.
    updateCheckProgressTimer = new QTimer(this);
    updateCheckProgressTimer->setInterval(5);
    connect( updateCheckProgressTimer, SIGNAL(timeout()), this, SLOT(updateUpdateCheckProgressBar()) );
    updateCheckProgressCounter = 0;

    // Connect the dialogs buttonbox with a button click handler.
    connect( buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(handleUpdateCheckDialogButtonClicked(QAbstractButton*)) );

    this->settings = settings;

    // This dialog is always modal.
    setModal(true);
}


/*!
    \brief This slot should be called, if an update check is manually invoked.

    In difference to the automatic update check, during manual update check request
    a modal progress indicator dialog will be shown.
 */
void UpdateCheckDialog::checkForUpdateAndShowDialog() {
    manualUpdateRequested = true;
    getPadXMLFile();
    showCheckingForUpdateDialog();
}


/*!
    \brief This slot should be called, if an update check is automatically invoked.

    An automatic invoked update check should run in background, so the user
    gets not interrupted by a dialog box.
 */
void UpdateCheckDialog::checkForUpdate() {
    manualUpdateRequested = false;
    getPadXMLFile();
}


/*!
    \brief This function tries to download the UniversalIndentGui pad file from the SourceForge server
 */
void UpdateCheckDialog::getPadXMLFile() {
    http->setHost("universalindent.sourceforge.net");
    http->get("/universalindentgui_pad.xml");
}


/*!
    \brief This slot is called after the update check has returned, either by successfully
    retrieving the pad file, or on any kind of network error.

    Shows a message if check was successful or not. Offers the user to go to the
    download page if a newer version exists. In case of an error during update
    check, a message box with the error will be displayed.
 */
void UpdateCheckDialog::checkResultsOfFetchedPadXMLFile(bool errorOccurred) {
    // Stop the progress bar timer.
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

            // Create integer values from the version strings.
            int versionOnServerInt = convertVersionStringToNumber( returnedString );
            int currentVersionInt = convertVersionStringToNumber( PROGRAM_VERSION_STRING );

            // Only show update dialog, if the current version number is lower than the one received from the server.
            if ( versionOnServerInt > currentVersionInt && currentVersionInt >= 0 && versionOnServerInt >= 0 ) {
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
    // If there was some error while trying to retrieve the update info from server and not cancel was pressed.
    else if ( roleOfClickedButton != QDialogButtonBox::RejectRole ) {
        QMessageBox::warning(this, "Update check error", "There was an error while trying to check for an update! Error was : " + http->errorString() );
    }
    manualUpdateRequested = false;
}


/*!
    \brief Displays the progress bar during update check.

    For displaying activity during update check, a timer is started to
    updated the progress bar. The user can press a cancel button to
    stop the update check.
 */
void UpdateCheckDialog::showCheckingForUpdateDialog() {
    // Reset the progress bar.
    updateCheckProgressCounter = 0;
    progressBar->setValue(updateCheckProgressCounter);
    progressBar->setInvertedAppearance( false );

    updateCheckProgressTimer->start();
    progressBar->show();
    setWindowTitle( tr("Checking for update...") );
    label->setText( tr("Checking whether a newer version is available") );
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel);
    show();
}


/*!
    \brief Displays the dialog with info about the new available version.
 */
void UpdateCheckDialog::showNewVersionAvailableDialog(QString newVersion) {
    progressBar->hide();
    setWindowTitle( tr("Update available") );
    label->setText( tr("A newer version of UniversalIndentGUI is available.\nYour version is %1. New version is %2.\nDo you want to go to the download website?").arg(PROGRAM_VERSION_STRING).arg(newVersion) );
    buttonBox->setStandardButtons(QDialogButtonBox::No|QDialogButtonBox::NoButton|QDialogButtonBox::Yes);
    exec();
}


/*!
    \brief Displays the dialog, that no new version is available.
 */
void UpdateCheckDialog::showNoNewVersionAvailableDialog() {
    progressBar->hide();
    setWindowTitle( tr("No new update available") );
    label->setText( tr("You already have the latest version of UniversalIndentGUI.") );
    buttonBox->setStandardButtons(QDialogButtonBox::Ok);
    exec();
}


/*!
    \brief This slot is called, when a button in the dialog is clicked.

    If the clicked button was the cancel button, the user wants to cancel
    the update check. So the http request is aborted and the timer for the
    progress bar animation is stopped.

    In any case if a button is clicked, the dialog box will be closed.
 */
void UpdateCheckDialog::handleUpdateCheckDialogButtonClicked(QAbstractButton *clickedButton) {
    roleOfClickedButton = buttonBox->buttonRole(clickedButton);

    if ( roleOfClickedButton == QDialogButtonBox::RejectRole ) {
        // Abort the http request.
        http->abort();
        // Stop the progress bar timer.
        updateCheckProgressTimer->stop();
    }

    accept();
}


/*!
    \brief This slot is responsible for the animation of the update check progress bar.
 */
void UpdateCheckDialog::updateUpdateCheckProgressBar() {
    // Depending on the progress bar direction, decrease or increase the progressbar value.
    if ( progressBar->invertedAppearance() ) {
        updateCheckProgressCounter--;
    }
    else {
        updateCheckProgressCounter++;
    }

    // If the progress bar reaches 0 or 100 as value, swap the animation direction.
    if ( updateCheckProgressCounter == 0 || updateCheckProgressCounter == 100 ) {
        progressBar->setInvertedAppearance( !progressBar->invertedAppearance() );
    }

    // Update the progress bar value.
    progressBar->setValue(updateCheckProgressCounter);
}


/*!
    \brief Converts the as string given version \a versionString to an integer number.

    The \a versionString must have the format x.x.x where each x represents a number
    of a maximum of 999. If the input format is wrong, -1 will be returned.The first
    number will be multiplied by 1000000 the second by 1000 and then all three will
    be summarized.

    Thus for example 12.5.170 will result in 12005170.
 */
int UpdateCheckDialog::convertVersionStringToNumber(QString versionString) {
    int versionInteger = 0;
    int pos = 0;

    QRegExp regEx("\\d{1,3}.\\d{1,3}.\\d{1,3}");
    QRegExpValidator validator(regEx, NULL);

    if ( validator.validate(versionString, pos) == QValidator::Acceptable ) {
        QStringList versionNumberStringList = versionString.split(".");
        versionInteger = versionNumberStringList.at(0).toInt() * 1000000;
        versionInteger += versionNumberStringList.at(1).toInt() * 1000;
        versionInteger += versionNumberStringList.at(2).toInt();
    }
    else {
        versionInteger = -1;
    }

    return versionInteger;
}
