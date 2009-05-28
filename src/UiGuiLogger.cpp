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

#include "UiGuiLogger.h"

#include <QDateTime>
#include <QFile>
#include <QUrl>
#include <QTextStream>
#include <QDesktopServices>

#include "SettingsPaths.h"

UiGuiLogger* UiGuiLogger::instance = NULL;

/*!
    \brief Returns the only existing instance of UiGuiLogger. If the instance doesn't exist, it will be created.
 */
UiGuiLogger* UiGuiLogger::getInstance() {
    if ( instance == NULL )
        instance = new UiGuiLogger();

    return instance;
}


/*!
    \brief Initializes the dialog and sets the path to the log file in the systems temporary directory.
    Sets the default verbose level to warning level.
 */
UiGuiLogger::UiGuiLogger() : QDialog() {
    setupUi(this);
    verboseLevel = QtWarningMsg;

    // On different systems it may be that "QDir::tempPath()" ends with a "/" or not. So check this.
    // Remove any trailing slashes.
    QString tempPath = QDir::tempPath();
    while ( tempPath.right(1) == "/" ) {
        tempPath.chop(1);
    }
    logFile.setFileName( tempPath + "/UiGUI_log.html" );

    connect( openLogFileFolderToolButton, SIGNAL(clicked()), this, SLOT(openLogFileFolder()) );

    // Make the main application not to wait for the logging window to close.
    setAttribute(Qt::WA_QuitOnClose, false);

}


/*!
    \brief Logs all incoming messages \a msg to the dialogs text edit and to the log file.

    Only messages whos \a type have a higher priority than the set verbose level are logged.
 */
void UiGuiLogger::messageHandler(QtMsgType type, const char *msg) {
    if ( instance == NULL )
        instance = new UiGuiLogger();

    // Only log messages that have a higher or equal priority than set with the verbose level.
    if ( type < instance->verboseLevel )
        return;

    // Init log message with prepended date and time.
    QString message = QDateTime::currentDateTime().toString();

    // Depending on the QtMsgType prepend a different colored Debug, Warning, Critical or Fatal.
    switch (type) {
        case QtDebugMsg:
            message += " <span style=\"font-weight:bold; color:black;\">Debug:</span> ";
            break;
        case QtWarningMsg:
            message += " <span style=\"font-weight:bold; color:gold;\">Warning:</span> ";
            break;
        case QtCriticalMsg:
            message += "<span style=\"font-weight:bold; color:red;\">Critical:</span> ";
            break;
        case QtFatalMsg:
            message += " <span style=\"font-weight:bold; color:#D60000;\">Fatal:</span> ";
        // This one is no Qt message type, but can be used to send info messages to the log
        // by calling UiGuiLogger::messageHandler() directly.
        case UiGuiInfoMsg:
            message += " <span style=\"font-weight:bold; color:darkgray;\">Info:</span> ";
            break;
    }

    // Append the ti UTF-8 back converted message parameter.
    message += QString::fromUtf8( msg ) + "<br/>\n";

    // Write the message to the log windows text edit.
    instance->logTextEdit->append( message );

    // Write/append the log message to the log file.
    if ( instance->logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append) ) {
        QTextStream out(&instance->logFile);
        out << message << "\n";
        instance->logFile.close();
    }

    // In case of a fatal error abort the application.
    if ( type == QtFatalMsg )
        abort();
}


/*!
    \brief Calling this the verbose level can be set in a range from 0 to 3
    which is equal to debug, warning, critical and fatal priority.
 */
void UiGuiLogger::setVerboseLevel(int level) {
    if ( level < 0 )
        verboseLevel = QtDebugMsg;
    if ( level > 3 )
        verboseLevel = QtFatalMsg;
    else
        verboseLevel = QtMsgType(level);
}


/*!
    \brief Deletes the existing instance of UiGuiLogger.
 */
void UiGuiLogger::deleteInstance() {
    if ( instance != NULL ) {
        delete instance;
        instance = NULL;
    }
}


/*!
    \brief Opens the folder that contains the created log file with the name "UiGUI_log.html".
 */
void UiGuiLogger::openLogFileFolder() {
    QDesktopServices::openUrl( QFileInfo( logFile ).absolutePath() );
}
