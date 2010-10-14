/***************************************************************************
 *   Copyright (C) 2006-2010 by Thomas Schweitzer                          *
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

#define UiGuiInfoMsg QtMsgType(4)

#include <QDialog>
#include <QFile>

namespace Ui {
	class UiGuiLoggerDialog;
}


class UiGuiLogger : public QDialog
{
    Q_OBJECT

public:
    static UiGuiLogger* getInstance(int verboseLevel);
    static UiGuiLogger* getInstance();
    static void messageHandler(QtMsgType type, const char *msg);
    static void deleteInstance();
    void setVerboseLevel(int level);

private slots:
    void openLogFileFolder();

private:
	Ui::UiGuiLoggerDialog *_uiGuiLoggerDialogForm;

    enum LogFileInitState { NOTINITIALZED, INITIALIZING, INITIALZED } _logFileInitState;
    UiGuiLogger(int verboseLevel);
    void writeToLogFile(const QString &message);

    static UiGuiLogger* _instance;
    QtMsgType _verboseLevel;
    QFile _logFile;
    QStringList _messageQueue;
};

#endif // UIGUILOGGER_H
