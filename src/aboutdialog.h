/***************************************************************************
 *   Copyright (C) 2006-2008 by Thomas Schweitzer                          *
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
#include <QScrollBar>
#include <QTimer>
#include "ui_aboutdialog.h"

#include <QLocale>


class AboutDialog : public QDialog, private Ui::AboutDialog
{
    Q_OBJECT

public:
    AboutDialog(QWidget *parent = 0, Qt::WindowFlags flags = 0, QString version = "", QString revision = "", QString buildDate = "");

public slots:
    int exec();
    void show();

private slots:
    void restoreTextBrowserContentAndOpenLink(const QUrl &link);
    void scroll();

private:
    void changeEvent(QEvent *event);

    QString gplText;
    QString textBrowserSavedContent;
    QString version;
    QString revision;
    QString buildDate;
    int scrollDirection;
    int scrollSpeed;
    QTimer *timer;
    QWidget *parent;
};

#endif // ABOUTDIALOG_H
