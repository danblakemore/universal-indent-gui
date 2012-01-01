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

#ifndef UPDATECHECKDIALOG_H
#define UPDATECHECKDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>

class UiGuiSettings;
namespace Ui {
	class UpdateCheckDialog;
}

class QTimer;
class QDesktopServices;
class QNetworkAccessManager;
class QNetworkReply;


class UpdateCheckDialog : public QDialog
{
    Q_OBJECT

public:
    UpdateCheckDialog(QSharedPointer<UiGuiSettings> settings, QWidget *parent = NULL);
    ~UpdateCheckDialog();

public slots:
    void checkForUpdateAndShowDialog();
    void checkForUpdate();

private slots:
    void checkResultsOfFetchedPadXMLFile(QNetworkReply *networkReply);
    void handleUpdateCheckDialogButtonClicked(QAbstractButton *clickedButton);
    void updateUpdateCheckProgressBar();
    void initProxySettings();

private:
	Ui::UpdateCheckDialog *_updateCheckDialogForm;

    void getPadXMLFile();
    void showCheckingForUpdateDialog();
    void showNewVersionAvailableDialog(QString newVersion);
    void showNoNewVersionAvailableDialog();
    int convertVersionStringToNumber(QString versionString);

    QSharedPointer<UiGuiSettings> _settings;
    bool _manualUpdateRequested;
    QNetworkAccessManager *_networkAccessManager;
    QNetworkReply *_currentNetworkReply;
    QDialogButtonBox::ButtonRole _roleOfClickedButton;
    QTimer *_updateCheckProgressTimer;
    int _updateCheckProgressCounter;
};

#endif // UPDATECHECKDIALOG_H
