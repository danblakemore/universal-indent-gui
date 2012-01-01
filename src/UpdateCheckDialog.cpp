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

#include "UpdateCheckDialog.h"
#include "ui_UpdateCheckDialog.h"

#include "UiGuiSettings.h"
#include "UiGuiVersion.h"

#include <QMessageBox>
#include <QDesktopServices>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QDate>
#include <QUrl>
#include <QRegExpValidator>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkProxy>

/*!
    \class UpdateCheckDialog
    \ingroup grp_MainWindow
    \brief UpdateCheckDialog is a dialog widget that contains functions
    for online checking for a new version of UniversalIndentGUI.
*/

/*!
    \brief Initializes member variables and stores the version of UiGui and a pointer to the _settings object.
 */
UpdateCheckDialog::UpdateCheckDialog(QSharedPointer<UiGuiSettings> settings, QWidget *parent) : QDialog(parent),
    _manualUpdateRequested(false),
    _currentNetworkReply(NULL),
    _roleOfClickedButton(QDialogButtonBox::InvalidRole)
{
	_updateCheckDialogForm = new Ui::UpdateCheckDialog();
    _updateCheckDialogForm->setupUi(this);

    // Create object for _networkAccessManager request and connect it with the request return handler.
    _networkAccessManager = new QNetworkAccessManager(this);
    connect( _networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(checkResultsOfFetchedPadXMLFile(QNetworkReply*)) );

    // Create a timer object used for the progress bar.
    _updateCheckProgressTimer = new QTimer(this);
    _updateCheckProgressTimer->setInterval(5);
    connect( _updateCheckProgressTimer, SIGNAL(timeout()), this, SLOT(updateUpdateCheckProgressBar()) );
    _updateCheckProgressCounter = 0;

    // Connect the dialogs buttonbox with a button click handler.
    connect( _updateCheckDialogForm->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(handleUpdateCheckDialogButtonClicked(QAbstractButton*)) );

    settings->registerObjectSlot(this, "initProxySettings()", "ProxyEnabled");
    settings->registerObjectSlot(this, "initProxySettings()", "ProxyHostName");
    settings->registerObjectSlot(this, "initProxySettings()", "ProxyPort");
    settings->registerObjectSlot(this, "initProxySettings()", "ProxyUserName");
    settings->registerObjectSlot(this, "initProxySettings()", "ProxyPassword");

    _settings = settings;

	initProxySettings();

    // This dialog is always modal.
    setModal(true);
}


/*!
    \brief On destroy cancels any currently running network request.
 */
UpdateCheckDialog::~UpdateCheckDialog() {
    disconnect( _networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(checkResultsOfFetchedPadXMLFile(QNetworkReply*)) );
    if (_currentNetworkReply != NULL)
        _currentNetworkReply->abort();
}


/*!
    \brief This slot should be called, if an update check is manually invoked.

    In difference to the automatic update check, during manual update check request
    a modal progress indicator dialog will be shown.
 */
void UpdateCheckDialog::checkForUpdateAndShowDialog() {
    _manualUpdateRequested = true;
    getPadXMLFile();
    showCheckingForUpdateDialog();
}


/*!
    \brief This slot should be called, if an update check is automatically invoked.

    An automatic invoked update check should run in background, so the user
    gets not interrupted by a dialog box.
 */
void UpdateCheckDialog::checkForUpdate() {
    _manualUpdateRequested = false;
    getPadXMLFile();
}


/*!
    \brief This function tries to download the UniversalIndentGui pad file from the SourceForge server.
 */
void UpdateCheckDialog::getPadXMLFile() {
    //_networkAccessManager->setHost("universalindent.sourceforge.net");
    //_networkAccessManager->get("/universalindentgui_pad.xml");
    _currentNetworkReply = _networkAccessManager->get(QNetworkRequest(QUrl("http://universalindent.sourceforge.net/universalindentgui_pad.xml")));
}


/*!
    \brief This slot is called after the update check has returned, either by successfully
    retrieving the pad file, or on any kind of network error.

    Shows a message if check was successful or not. Offers the user to go to the
    download page if a newer version exists. In case of an error during update
    check, a message box with the error will be displayed.
 */
void UpdateCheckDialog::checkResultsOfFetchedPadXMLFile(QNetworkReply *networkReply) {
    Q_ASSERT(_currentNetworkReply == networkReply);

    // Stop the progress bar timer.
    _updateCheckProgressTimer->stop();

    if ( networkReply->error() == QNetworkReply::NoError ) {
        // Try to find the version string.
        QString returnedString = networkReply->readAll();
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
                if ( _roleOfClickedButton == QDialogButtonBox::YesRole ) {
                    QDesktopServices::openUrl( QUrl("_networkAccessManager://sourceforge.net/project/showfiles.php?group_id=167482") );
                }
            }
            else if ( _manualUpdateRequested ) {
                showNoNewVersionAvailableDialog();
            }
            // Set last update check date.
            _settings->setValueByName("LastUpdateCheck", QDate::currentDate());
        }
        // In the returned string, the version string could not be found.
        else {
            QMessageBox::warning(this, tr("Update check error"), tr("There was an error while trying to check for an update! The retrieved file did not contain expected content.") );
        }
    }
    // If there was some error while trying to retrieve the update info from server and not cancel was pressed.
    else if ( _roleOfClickedButton != QDialogButtonBox::RejectRole ) {
        QMessageBox::warning(this, tr("Update check error"), tr("There was an error while trying to check for an update! Error was : %1").arg(networkReply->errorString()) );
		hide();
    }
    _manualUpdateRequested = false;
    networkReply->deleteLater();
    _currentNetworkReply = NULL;
}


/*!
    \brief Displays the progress bar during update check.

    For displaying activity during update check, a timer is started to
    updated the progress bar. The user can press a cancel button to
    stop the update check.
 */
void UpdateCheckDialog::showCheckingForUpdateDialog() {
    // Reset the progress bar.
    _updateCheckProgressCounter = 0;
    _updateCheckDialogForm->progressBar->setValue(_updateCheckProgressCounter);
    _updateCheckDialogForm->progressBar->setInvertedAppearance( false );

    _updateCheckProgressTimer->start();
    _updateCheckDialogForm->progressBar->show();
    setWindowTitle( tr("Checking for update...") );
    _updateCheckDialogForm->label->setText( tr("Checking whether a newer version is available") );
    _updateCheckDialogForm->buttonBox->setStandardButtons(QDialogButtonBox::Cancel);
    show();
}


/*!
    \brief Displays the dialog with info about the new available version.
 */
void UpdateCheckDialog::showNewVersionAvailableDialog(QString newVersion) {
    _updateCheckDialogForm->progressBar->hide();
    setWindowTitle( tr("Update available") );
    _updateCheckDialogForm->label->setText( tr("A newer version of UniversalIndentGUI is available.\nYour version is %1. New version is %2.\nDo you want to go to the download website?").arg(PROGRAM_VERSION_STRING).arg(newVersion) );
    _updateCheckDialogForm->buttonBox->setStandardButtons(QDialogButtonBox::No|QDialogButtonBox::NoButton|QDialogButtonBox::Yes);
    exec();
}


/*!
    \brief Displays the dialog, that no new version is available.
 */
void UpdateCheckDialog::showNoNewVersionAvailableDialog() {
    _updateCheckDialogForm->progressBar->hide();
    setWindowTitle( tr("No new update available") );
    _updateCheckDialogForm->label->setText( tr("You already have the latest version of UniversalIndentGUI.") );
    _updateCheckDialogForm->buttonBox->setStandardButtons(QDialogButtonBox::Ok);
    exec();
}


/*!
    \brief This slot is called, when a button in the dialog is clicked.

    If the clicked button was the cancel button, the user wants to cancel
    the update check. So the _networkAccessManager request is aborted and the timer for the
    progress bar animation is stopped.

    In any case if a button is clicked, the dialog box will be closed.
 */
void UpdateCheckDialog::handleUpdateCheckDialogButtonClicked(QAbstractButton *clickedButton) {
    _roleOfClickedButton = _updateCheckDialogForm->buttonBox->buttonRole(clickedButton);

    if ( _roleOfClickedButton == QDialogButtonBox::RejectRole ) {
        // Abort the _networkAccessManager request.
        _currentNetworkReply->abort();
        // Stop the progress bar timer.
        _updateCheckProgressTimer->stop();
    }

    accept();
}


/*!
    \brief This slot is responsible for the animation of the update check progress bar.
 */
void UpdateCheckDialog::updateUpdateCheckProgressBar() {
    // Depending on the progress bar direction, decrease or increase the progressbar value.
    if ( _updateCheckDialogForm->progressBar->invertedAppearance() ) {
        _updateCheckProgressCounter--;
    }
    else {
        _updateCheckProgressCounter++;
    }

    // If the progress bar reaches 0 or 100 as value, swap the animation direction.
    if ( _updateCheckProgressCounter == 0 || _updateCheckProgressCounter == 100 ) {
        _updateCheckDialogForm->progressBar->setInvertedAppearance( !_updateCheckDialogForm->progressBar->invertedAppearance() );
    }

    // Update the progress bar value.
    _updateCheckDialogForm->progressBar->setValue(_updateCheckProgressCounter);
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

void UpdateCheckDialog::initProxySettings()
{
    if ( _settings->getValueByName("ProxyEnabled") == true ) {
		QString proxyHostName = _settings->getValueByName("ProxyHostName").toString();
		int proxyPort = _settings->getValueByName("ProxyPort").toInt();
		QString proxyUserName = _settings->getValueByName("ProxyUserName").toString();
		QString proxyPassword = _settings->getValueByName("ProxyPassword").toString();
		_networkAccessManager->setProxy(QNetworkProxy(QNetworkProxy::Socks5Proxy, proxyHostName, proxyPort, proxyUserName, proxyPassword));
    }
    else {
		_networkAccessManager->setProxy(QNetworkProxy());
    }
}
