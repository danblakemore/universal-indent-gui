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

#include "SettingsPaths.h"

#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <QStack>
#include <QtDebug>

#include <stdlib.h>

//! \defgroup grp_Settings All concerning applications settings.

/*!
    \class SettingsPaths
    \ingroup grp_Settings
    \brief SettingsPaths is a pure static functions class from which info about the
    paths needed for settings can be retrieved.
*/

bool SettingsPaths::_alreadyInitialized = false;
QString SettingsPaths::_applicationBinaryPath = "";
QString SettingsPaths::_settingsPath = "";
QString SettingsPaths::_globalFilesPath = "";
QString SettingsPaths::_indenterPath = "";
QString SettingsPaths::_tempPath = "";
bool SettingsPaths::_portableMode = false;


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
    _alreadyInitialized = true;

    qDebug() << __LINE__ << " " << __FUNCTION__ << ": Initializing application paths.";

    // Get the applications binary path, with respect to MacOSXs use of the .app folder.
    _applicationBinaryPath = QCoreApplication::applicationDirPath();
    // Remove any trailing slashes
    while ( _applicationBinaryPath.right(1) == "/" ) {
        _applicationBinaryPath.chop(1);
    }

#ifdef UNIVERSALINDENTGUI_NPP_EXPORTS
    _applicationBinaryPath += "/plugins/uigui";
#endif

#ifdef Q_OS_MAC
    // Because on Mac universal binaries are used, the binary path is not equal
    // to the applications (.app) path. So get the .apps path here.
    int indexOfDotApp = _applicationBinaryPath.indexOf(".app");
    if ( indexOfDotApp != -1 ) {
        // Cut off after the dot of ".app".
        _applicationBinaryPath = _applicationBinaryPath.left( indexOfDotApp-1 );
        // Cut off after the first slash that was in front of ".app" (normally this is the word "UniversalIndentGUI")
        _applicationBinaryPath = _applicationBinaryPath.left( _applicationBinaryPath.lastIndexOf("/") );
    }
#endif

    // If the "config" directory is a subdir of the applications binary path, use this one (portable mode)
    _settingsPath = _applicationBinaryPath + "/config";
    if ( QFile::exists( _settingsPath ) ) {
        _portableMode = true;
        QDir dirCreator;
        _globalFilesPath = _applicationBinaryPath;
        _indenterPath = _applicationBinaryPath + "/indenters";
        dirCreator.mkpath( _settingsPath );
        _tempPath = _applicationBinaryPath + "/temp";
        //TODO: If the portable drive has write protection, use local temp path and clean it up on exit.
        dirCreator.mkpath( _tempPath );
    }
    // ... otherwise use the system specific global application data path.
    else {
        _portableMode = false;
        QDir dirCreator;
#ifdef Q_OS_WIN
        // Get the local users application settings directory.
        // Remove any trailing slashes.
        _settingsPath = QDir::fromNativeSeparators( qgetenv("APPDATA") );
        while ( _settingsPath.right(1) == "/" ) {
            _settingsPath.chop(1);
        }
        _settingsPath = _settingsPath + "/UniversalIndentGUI";

        // On windows systems the directories "indenters", "translations" are subdirs of the _applicationBinaryPath.
        _globalFilesPath = _applicationBinaryPath;
#else
        // Remove any trailing slashes.
        _settingsPath = QDir::homePath();
        while ( _settingsPath.right(1) == "/" ) {
            _settingsPath.chop(1);
        }
        _settingsPath = _settingsPath + "/.universalindentgui";
        _globalFilesPath = "/usr/share/universalindentgui";
#endif
        dirCreator.mkpath( _settingsPath );
        // If a highlighter config file does not exist in the users home config dir
        // copy the default config file over there.
        if ( !QFile::exists(_settingsPath+"/UiGuiSyntaxHighlightConfig.ini") ) {
            QFile::copy( _globalFilesPath+"/config/UiGuiSyntaxHighlightConfig.ini", _settingsPath+"/UiGuiSyntaxHighlightConfig.ini" );
        }
        _indenterPath = _globalFilesPath + "/indenters";

        // On different systems it may be that "QDir::tempPath()" ends with a "/" or not. So check this.
        // Remove any trailing slashes.
        _tempPath = QDir::tempPath();
        while ( _tempPath.right(1) == "/" ) {
            _tempPath.chop(1);
        }
        _tempPath = _tempPath + "/UniversalIndentGUI";

#if defined(Q_OS_WIN32)
        dirCreator.mkpath( _tempPath );
#else
        // On Unix based systems create a random temporary directory for security
        // reasons. Otherwise an evil human being could create a symbolic link
        // to an important existing file which gets overwritten when UiGUI writes
        // into this normally temporary but linked file.
        char *pathTemplate = new char[_tempPath.length()+8];
        QByteArray pathTemplateQBA = QString(_tempPath + "-XXXXXX").toLatin1();
        delete [] pathTemplate;
        pathTemplate = pathTemplateQBA.data();
        pathTemplate = mkdtemp( pathTemplate );
        _tempPath = pathTemplate;
#endif
    }

    qDebug() << __LINE__ << " " << __FUNCTION__ << ": Paths are:" \
        "<ul><li>_applicationBinaryPath=" << _applicationBinaryPath \
        << "</li><li>_settingsPath=" << _settingsPath \
        << "</li><li>_globalFilesPath=" << _globalFilesPath \
        << "</li><li>_indenterPath=" << _indenterPath \
        << "</li><li>_tempPath=" << _tempPath \
        << "</li><li>Running in portable mode=" << _portableMode << "</li></ul>";
}


/*!
    \brief Returns the path of the applications executable.
 */
const QString SettingsPaths::getApplicationBinaryPath() {
    if ( !_alreadyInitialized ) {
        SettingsPaths::init();
    }
    return _applicationBinaryPath;
}


/*!
    \brief Returns the path where all settings are being/should be written to.
 */
const QString SettingsPaths::getSettingsPath() {
    if ( !_alreadyInitialized ) {
        SettingsPaths::init();
    }
    return _settingsPath;
}


/*!
    \brief Returns the path where the files concerning all users reside. For example translations.
 */
const QString SettingsPaths::getGlobalFilesPath() {
    if ( !_alreadyInitialized ) {
        SettingsPaths::init();
    }
    return _globalFilesPath;
}


/*!
    \brief Returns the path where the indenter executables reside.
 */
const QString SettingsPaths::getIndenterPath() {
    if ( !_alreadyInitialized ) {
        SettingsPaths::init();
    }
    return _indenterPath;
}


/*!
    \brief Returns the path where the where all temporary data should be written to.
 */
const QString SettingsPaths::getTempPath() {
    if ( !_alreadyInitialized ) {
        SettingsPaths::init();
    }
    return _tempPath;
}


/*!
    \brief Returns true if portable mode shall be used.
 */
bool SettingsPaths::getPortableMode() {
    if ( !_alreadyInitialized ) {
        SettingsPaths::init();
    }
    return _portableMode;
}


/*!
    \brief Completely deletes the created temporary directory with all of its content.
 */
void SettingsPaths::cleanAndRemoveTempDir() {
    QDirIterator dirIterator(_tempPath, QDirIterator::Subdirectories);
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
                bool couldRemoveDir = QDir(dirToBeRemoved).rmdir(dirToBeRemoved);
                noErrorsOccurred &= couldRemoveDir;
                if ( couldRemoveDir == false )
                    qWarning() << __LINE__ << " " << __FUNCTION__ << "Could not remove the directory: " << dirToBeRemoved;
                //qDebug() << "Removing Dir " << directoryStack.pop();
            }

            // If the iterator currently points to a directory push it onto the stack.
            if ( dirIterator.fileInfo().isDir() ) {
                directoryStack.push( currentDirOrFile );
                //qDebug() << "Pushing onto Stack " << currentDirOrFile;
            }
            // otherwise it must be a file, so delete it.
            else {
                bool couldRemoveFile = QFile::remove( currentDirOrFile );
                noErrorsOccurred &= couldRemoveFile;
                if ( couldRemoveFile == false )
                    qWarning() << __LINE__ << " " << __FUNCTION__ << "Could not remove the file: " << currentDirOrFile;
                //qDebug() << "Removing File " << currentDirOrFile;
            }
        }
    }
    noErrorsOccurred &= QDir(_tempPath).rmdir(_tempPath);
    if ( noErrorsOccurred == false )
        qWarning() << __LINE__ << " " << __FUNCTION__ << "While cleaning up the temp dir an error occurred.";
}
