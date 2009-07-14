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

#include <ctime>
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
UiGuiLogger* UiGuiLogger::getInstance(int verboseLevel) {
    if ( instance == NULL )
        instance = new UiGuiLogger(verboseLevel);

    return instance;
}

/*!
    \brief Returns the only existing instance of UiGuiLogger. If the instance doesn't exist, it will be created.
 */
UiGuiLogger* UiGuiLogger::getInstance() {
#ifdef _DEBUG
    return UiGuiLogger::getInstance(QtDebugMsg);
#else
    return UiGuiLogger::getInstance(QtWarningMsg);
#endif
}


/*!
    \brief Initializes the dialog and sets the path to the log file in the systems temporary directory.
    Sets the default verbose level to warning level.
 */
UiGuiLogger::UiGuiLogger(int verboseLevel) : QDialog() {
    setupUi(this);
#ifdef _DEBUG
    this->verboseLevel = QtDebugMsg;
#else
    this->verboseLevel = QtMsgType(verboseLevel);
#endif

    logFileInitState = NOTINITIALZED;

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
        instance = UiGuiLogger::getInstance();

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
    instance->writeToLogFile( message );

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


/*!
    \brief Writes the \a message to the used log file.
 */
void UiGuiLogger::writeToLogFile(const QString message) {

    // If the file where all logging messages should go to isn't initilized yet, do that now.
    if ( logFileInitState == NOTINITIALZED ) {
        logFileInitState = INITIALIZING;

        // On different systems it may be that "QDir::tempPath()" ends with a "/" or not. So check this.
        // Remove any trailing slashes.
        QString tempPath = QFileInfo( SettingsPaths::getTempPath() ).absolutePath();
        while ( tempPath.right(1) == "/" ) {
            tempPath.chop(1);
        }

        // To make the temporary log file invulnerable against file symbolic link hacks
        // append the current date and time up to milliseconds to its name and a random character.
        QString logFileName = "UiGUI_log_" + QDateTime::currentDateTime().toString("yyyyMMdd");
        logFileName += "-" + QDateTime::currentDateTime().toString("hhmmsszzz");
        // By random decide whether to append a number or an upper or lower case character.
        qsrand( time(NULL) );
        unsigned char randomChar;
        switch ( qrand() % 3 ) {
            // Append a number from 0 to 9.
        case 0:
            randomChar = qrand() % 10 + '0';
            break;
            // Append a upper case characer between A and Z.
        case 1:
            randomChar = qrand() % 26 + 'A';
            break;
            // Append a lower case characer between a and z.
        default:
            randomChar = qrand() % 26 + 'a';
            break;
        }
        logFileName += "_" + QString(randomChar) + ".html";

        logFile.setFileName( tempPath + "/" + logFileName );

        // Set the tooltip of the open log file folder button to show the unique name of the log file.
        openLogFileFolderToolButton->setToolTip( openLogFileFolderToolButton->toolTip() + " (" + logFileName + ")" );

        logFileInitState = INITIALZED;
    }

    // Add the message to the message queue.
    messageQueue << message;

    // If the logging file is initialzed, write all messages contained in the message queue into the file.
    if ( logFileInitState == INITIALZED ) {
        // Write/append the log message to the log file.
        if ( logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append) ) {
            QTextStream out(&logFile);

            while ( !messageQueue.isEmpty() ) {
                out << messageQueue.takeFirst() << "\n";
            }

            logFile.close();
        }
    }
}
