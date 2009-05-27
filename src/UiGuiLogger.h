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

#ifndef UIGUILOGGER_H
#define UIGUILOGGER_H

#include <QDialog>
#include <QFile>

#include "ui_UiGuiLoggerDialog.h"

/*!
    \class UiGuiLogger
    \brief This class handles any kind of data logging, for debugging and whatever purpose.

    Beneath being able of displaying a dialog window containing all log messages of the
    current session, a log file in the systems temporary directory is used. Its name
    is "UiGUI_log.html".

    Setting a verbose level allows to only write messages that have the selected minimum
    priority to the log.
 */

class UiGuiLogger : public QDialog, private Ui::UiGuiLoggerDialog
{
    Q_OBJECT

public:
    static UiGuiLogger* getInstance();
    static void messageHandler(QtMsgType type, const char *msg);
    static void deleteInstance();
    void setVerboseLevel(int level);

private:
    UiGuiLogger();

    static UiGuiLogger* instance;
    QtMsgType verboseLevel;
    QFile logFile;

private slots:
    void openLogFileFolder();
};

#endif // UIGUILOGGER_H
