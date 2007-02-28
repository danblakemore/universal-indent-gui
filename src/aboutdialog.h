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

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QUrl>
#include <QDesktopServices>
#include "ui_aboutdialog.h"

class AboutDialog : public QDialog, private Ui::AboutDialog
{
    Q_OBJECT

public:
    AboutDialog(QWidget *parent, QString version, QString revision, QString buildDate);
    void retranslate();
private slots:
    void linkClicked(const QUrl &link);
private:
    QString textBrowserSavedContent;
    QString version;
    QString revision;
    QString buildDate;
};

#endif // ABOUTDIALOG_H
