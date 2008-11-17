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


#include <stdlib.h>
#include <QDirIterator>
#include <QStack>
#include <QtDebug>

#include "SettingsPaths.h"


//! \defgroup grp_Settings All concerning applications settings.

/*!
    \class SettingsPaths
    \ingroup grp_Settings
    \brief SettingsPaths is a pure static functions class from which info about the
    paths needed for settings can be retrieved.
*/

bool SettingsPaths::alreadyInitialized = false;
QString SettingsPaths::applicationBinaryPath = "";
QString SettingsPaths::settingsPath = "";
QString SettingsPaths::globalFilesPath = "";
QString SettingsPaths::indenterPath = "";
QString SettingsPaths::tempPath = "";
bool SettingsPaths::portableMode = false;


/*!
    \brief Initializes all available information about the paths.

    Mainly during this init it is detected whether to start in portable mode or not. This is
    done by testing whether the directory "config" is in the same directory as this
    applications executable file.
    In portable mode all data is ONLY written to subdirectories of the applications executable file.
    Means also that the directory "indenters" has to be there.
    In not portable mode (multiuser mode) only users home directory is used for writing config data.
 */
void SettingsPaths::init() {
    // Get the applications binary path, with respect to MacOSXs use of the .app folder.
    applicationBinaryPath = QCoreApplication::applicationDirPath();
    // Remove any trailing slashes
    while ( applicationBinaryPath.right(1) == "/" ) {
        applicationBinaryPath.chop(1);
    }

#ifdef UNIVERSALINDENTGUI_NPP_EXPORTS
    applicationBinaryPath += "/plugins/uigui";
#endif

#ifdef Q_OS_MAC
    // Because on Mac universal binaries are used, the binary path is not equal
    // to the applications (.app) path. So get the .apps path here.
    int indexOfDotApp = applicationBinaryPath.indexOf(".app");
    if ( indexOfDotApp != -1 ) {
        // Cut off after the dot of ".app".
        applicationBinaryPath = applicationBinaryPath.left( indexOfDotApp-1 );
        // Cut off after the first slash that was in front of ".app" (normally this is the word "UniversalIndentGUI")
        applicationBinaryPath = applicationBinaryPath.left( applicationBinaryPath.lastIndexOf("/") );
    }
#endif

    // If the "config" directory is a subdir of the applications binary path, use this one (portable mode)
    settingsPath = applicationBinaryPath + "/config";
    if ( QFile::exists( settingsPath ) ) {
        portableMode = true;
        QDir dirCreator;
        globalFilesPath = applicationBinaryPath;
        indenterPath = applicationBinaryPath + "/indenters";
        dirCreator.mkpath( settingsPath );
        tempPath = applicationBinaryPath + "/temp";
        //TODO: If the portable drive has write protection, use local temp path and clean it up on exit.
        dirCreator.mkpath( tempPath );
    }
    // ... otherwise use the system specific global application data path.
    else {
        portableMode = false;
        QDir dirCreator;
#ifdef Q_OS_WIN
        // Get the local users application settings directory.
        // Remove any trailing slashes.
        settingsPath = QDir::fromNativeSeparators( qgetenv("APPDATA") );
        while ( settingsPath.right(1) == "/" ) {
            settingsPath.chop(1);
        }
        settingsPath = settingsPath + "/UniversalIndentGUI";

        // On windows systems the directories "indenters", "translations" are subdirs of the applicationBinaryPath.
        globalFilesPath = applicationBinaryPath;
#else
        // Remove any trailing slashes.
        settingsPath = QDir::homePath();
        while ( settingsPath.right(1) == "/" ) {
            settingsPath.chop(1);
        }
        settingsPath = settingsPath + "/.universalindentgui";
        globalFilesPath = "/usr/share/universalindentgui";
#endif
        dirCreator.mkpath( settingsPath );
        // If a highlighter config file does not exist in the users home config dir
        // copy the default config file over there.
        if ( !QFile::exists(settingsPath+"/UiGuiSyntaxHighlightConfig.ini") ) {
            QFile::copy( globalFilesPath+"/config/UiGuiSyntaxHighlightConfig.ini", settingsPath+"/UiGuiSyntaxHighlightConfig.ini" );
        }
        indenterPath = globalFilesPath + "/indenters";

        // On different systems it may be that "QDir::tempPath()" ends with a "/" or not. So check this.
        // Remove any trailing slashes.
        tempPath = QDir::tempPath();
        while ( tempPath.right(1) == "/" ) {
            tempPath.chop(1);
        }
        tempPath = tempPath + "/UniversalIndentGUI";

#if defined(Q_OS_WIN32)
        dirCreator.mkpath( tempPath );
#else
        // On Unix based systems create a random temporary directory for security
        // reasons. Otherwise an evil human being could create a symbolic link
        // to an important existing file which gets overwritten when UiGUI writes
        // into this normally temporary but linked file.
        char *pathTemplate = new char[tempPath.length()+8];
        QByteArray pathTemplateQBA = QString(tempPath + "-XXXXXX").toAscii();
        pathTemplate = pathTemplateQBA.data();
        pathTemplate = mkdtemp( pathTemplate );
        tempPath = pathTemplate;
#endif
    }

    alreadyInitialized = true;
}


/*!
    \brief Returns the path of the applications executable.
 */
const QString SettingsPaths::getApplicationBinaryPath() {
    if ( !alreadyInitialized ) {
        SettingsPaths::init();
    }
    return applicationBinaryPath;
}


/*!
    \brief Returns the path where all settings are being/should be written to.
 */
const QString SettingsPaths::getSettingsPath() {
    if ( !alreadyInitialized ) {
        SettingsPaths::init();
    }
    return settingsPath;
}


/*!
    \brief Returns the path where the files concerning all users reside. For example translations.
 */
const QString SettingsPaths::getGlobalFilesPath() {
    if ( !alreadyInitialized ) {
        SettingsPaths::init();
    }
    return globalFilesPath;
}


/*!
    \brief Returns the path where the indenter executables reside.
 */
const QString SettingsPaths::getIndenterPath() {
    if ( !alreadyInitialized ) {
        SettingsPaths::init();
    }
    return indenterPath;
}


/*!
    \brief Returns the path where the where all temporary data should be written to.
 */
const QString SettingsPaths::getTempPath() {
    if ( !alreadyInitialized ) {
        SettingsPaths::init();
    }
    return tempPath;
}


/*!
    \brief Returns true if portable mode shall be used.
 */
bool SettingsPaths::getPortableMode() {
    if ( !alreadyInitialized ) {
        SettingsPaths::init();
    }
    return portableMode;
}


/*!
    \brief Returns true if portable mode shall be used.
 */
void SettingsPaths::cleanAndRemoveTempDir() {
    QDirIterator dirIterator(tempPath, QDirIterator::Subdirectories);
    QStack<QString> directoryStack;
    bool noErrorsOccurred = true;
    
    while ( dirIterator.hasNext() ) {
        QString currentDirOrFile = dirIterator.next();
        // If this dummy call isn't done here, calling "dirIterator.fileInfo().isDir()" later somehow fails.
        dirIterator.fileInfo();

        if ( !currentDirOrFile.isEmpty() && dirIterator.fileName() != "." && dirIterator.fileName() != ".." ) {
            // There is a path on the stack but the current path doesn't start with that path.
            // So we changed into another parent directory and the one on the stack can be deleted
            // since it must be empty.
            if ( !directoryStack.isEmpty() && !currentDirOrFile.startsWith(directoryStack.top()) ) {
                QString dirToBeRemoved = directoryStack.pop();
                noErrorsOccurred &= QDir(dirToBeRemoved).rmdir(dirToBeRemoved);
                //qDebug() << "Removing Dir " << directoryStack.pop();
            }
            
            // If the iterator currently points to a directory push it onto the stack.
            if ( dirIterator.fileInfo().isDir() ) {
                directoryStack.push( currentDirOrFile );
                //qDebug() << "Pushing onto Stack " << currentDirOrFile;
            }
            // otherwise it must be a file, so delete it.
            else {
                noErrorsOccurred &= QFile::remove( currentDirOrFile );
                //qDebug() << "Removing File " << currentDirOrFile;
            }
        }
    }
    noErrorsOccurred &= QDir(tempPath).rmdir(tempPath);
    //qDebug() << "Removing tempPath " << tempPath;
}
