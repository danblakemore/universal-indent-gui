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

#ifndef UIGUISETTINGSDIALOG_H
#define UIGUISETTINGSDIALOG_H

#include <QDialog>
#include "ui_UiGuiSettingsDialog.h"
#include "uiguisettings.h"

class UiGuiSettingsDialog : public QDialog, private Ui::SettingsDialog
{
	Q_OBJECT

public:
	UiGuiSettingsDialog(QWidget* parent, UiGuiSettings* settings);
	void retranslate();

public slots:
    int showDialog();

private slots:
    void handleAccepted();

private:
    UiGuiSettings* settings;
    QList<QCheckBox*> checkBoxes;
    QList<QSpinBox*> spinBoxes;
};

#endif // UIGUISETTINGSDIALOG_H
