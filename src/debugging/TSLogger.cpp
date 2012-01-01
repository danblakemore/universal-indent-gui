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

#include "TSLogger.h"
#include "ui_TSLoggerDialog.h"

#include "SettingsPaths.h"

#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QUrl>
#include <QTextStream>
#include <QDesktopServices>
#include <QMessageBox>

#include <ctime>

using namespace tschweitzer;
using namespace tschweitzer::debugging;

TSLogger* TSLogger::_instance = NULL;

/*!
    \class TSLogger
    \brief This class handles any kind of data logging, for debugging and whatever purpose.

    Beneath being able of displaying a dialog window containing all log messages of the
    current session, a log file in the systems temporary directory is used. Its name
    is "UiGUI_log.html".

    Setting a verbose level allows to only write messages that have the selected minimum
    priority to the log.
 */

/*!
    \brief Returns the only existing instance of TSLogger. If the instance doesn't exist, it will be created.
 */
TSLogger* TSLogger::getInstance(int verboseLevel) {
    if ( _instance == NULL )
        _instance = new TSLogger(verboseLevel);

    return _instance;
}


/*!
    \brief Returns the only existing instance of TSLogger. If the instance doesn't exist, it will be created.
 */
TSLogger* TSLogger::getInstance() {
#ifdef _DEBUG
    return TSLogger::getInstance(QtDebugMsg);
#else
    return TSLogger::getInstance(QtWarningMsg);
#endif
}


/*!
    \brief Initializes the dialog and sets the path to the log file in the systems temporary directory.
    Sets the default verbose level to warning level.
 */
TSLogger::TSLogger(int verboseLevel) : QDialog() {
	_TSLoggerDialogForm = new Ui::TSLoggerDialog();
    _TSLoggerDialogForm->setupUi(this);
#ifdef _DEBUG
    _verboseLevel = QtDebugMsg;
#else
    _verboseLevel = QtMsgType(verboseLevel);
#endif

    _logFileInitState = NOTINITIALZED;

    connect( _TSLoggerDialogForm->openLogFileFolderToolButton, SIGNAL(clicked()), this, SLOT(openLogFileFolder()) );

    // Make the main application not to wait for the logging window to close.
    setAttribute(Qt::WA_QuitOnClose, false);
}


/*!
    \brief Logs all incoming messages \a msg to the dialogs text edit and to the log file.

    Only messages whos \a type have a higher priority than the set verbose level are logged.
 */
void TSLogger::messageHandler(QtMsgType type, const char *msg) {
    if ( _instance == NULL )
        _instance = TSLogger::getInstance();

/*
    QMessageBox messageBox;
    QString messageBoxText = QString::fromUtf8( msg );
    messageBox.setText( messageBoxText );
    messageBox.setWindowModality( Qt::ApplicationModal );
    messageBox.exec();
*/

    // Only log messages that have a higher or equal priority than set with the verbose level.
    if ( type < _instance->_verboseLevel )
        return;

    // Init log message with prepended date and time.
    QString message = QDateTime::currentDateTime().toString();

    // Depending on the QtMsgType prepend a different colored Debug, Warning, Critical or Fatal.
    switch (type) {
        case QtDebugMsg :
            message += " <span style=\"font-weight:bold; color:black;\">Debug:</span> ";
            break;
        case QtWarningMsg :
            message += " <span style=\"font-weight:bold; color:gold;\">Warning:</span> ";
            break;
        case QtCriticalMsg :
            message += "<span style=\"font-weight:bold; color:red;\">Critical:</span> ";
            break;
        case QtFatalMsg :
            message += " <span style=\"font-weight:bold; color:#D60000;\">Fatal:</span> ";
        // This one is no Qt message type, but can be used to send info messages to the log
        // by calling TSLogger::messageHandler() directly.
        case TSLoggerInfoMsg :
            message += " <span style=\"font-weight:bold; color:darkgray;\">Info:</span> ";
            break;
    }

    // Append the to UTF-8 back converted message parameter.
    message += QString::fromUtf8( msg ) + "<br/>\n";

    // Write the message to the log windows text edit.
    _instance->_TSLoggerDialogForm->logTextEdit->append( message );

    // Write/append the log message to the log file.
    _instance->writeToLogFile( message );

    // In case of a fatal error abort the application.
    if ( type == QtFatalMsg )
        abort();
}


/*!
    \brief Calling this the verbose level can be set in a range from 0 to 3
    which is equal to debug, warning, critical and fatal priority.
 */
void TSLogger::setVerboseLevel(int level) {
    if ( level < 0 )
        _verboseLevel = QtDebugMsg;
    if ( level > 3 )
        _verboseLevel = QtFatalMsg;
    else
        _verboseLevel = QtMsgType(level);
}


/*!
    \brief Deletes the existing _instance of TSLogger.
 */
void TSLogger::deleteInstance() {
    if ( _instance != NULL ) {
        delete _instance;
        _instance = NULL;
    }
}


/*!
    \brief Opens the folder that contains the created log file with the name "UiGUI_log.html".
 */
void TSLogger::openLogFileFolder() {
    QDesktopServices::openUrl( QFileInfo( _logFile ).absolutePath() );
}


/*!
    \brief Writes the \a message to the used log file.
 */
void TSLogger::writeToLogFile(const QString &message) {
    // If the file where all logging messages should go to isn't initilized yet, do that now.
    if ( _logFileInitState == NOTINITIALZED ) {
        _logFileInitState = INITIALIZING;

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
            case 0 :
                randomChar = qrand() % 10 + '0';
                break;
            // Append a upper case characer between A and Z.
            case 1 :
                randomChar = qrand() % 26 + 'A';
                break;
            // Append a lower case characer between a and z.
            default :
                randomChar = qrand() % 26 + 'a';
                break;
        }
        logFileName += "_" + QString(randomChar) + ".html";

        _logFile.setFileName( tempPath + "/" + logFileName );

        // Set the tooltip of the open log file folder button to show the unique name of the log file.
        _TSLoggerDialogForm->openLogFileFolderToolButton->setToolTip( _TSLoggerDialogForm->openLogFileFolderToolButton->toolTip() + " (" + logFileName + ")" );

        _logFileInitState = INITIALZED;
    }

    // Add the message to the message queue.
    _messageQueue << message;

    // If the logging file is initialzed, write all messages contained in the message queue into the file.
    if ( _logFileInitState == INITIALZED ) {
        // Write/append the log message to the log file.
        if ( _logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append) ) {
            QTextStream out(&_logFile);

            while ( !_messageQueue.isEmpty() ) {
                out << _messageQueue.takeFirst() << "\n";
            }

            _logFile.close();
        }
    }
}
