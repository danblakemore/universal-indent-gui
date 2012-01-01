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

#ifndef SETTINGSPATHS_H
#define SETTINGSPATHS_H

class QString;


class SettingsPaths
{
public:
    static void init();
    static const QString getApplicationBinaryPath();
    static const QString getSettingsPath();
    static const QString getGlobalFilesPath();
    static const QString getIndenterPath();
    static const QString getTempPath();
    static bool getPortableMode();
    static void cleanAndRemoveTempDir();

private:
    SettingsPaths();

    static bool _alreadyInitialized;
    static QString _applicationBinaryPath;
    static QString _settingsPath;
    static QString _globalFilesPath;
    static QString _indenterPath;
    static QString _tempPath;
    static bool _portableMode;
};

#endif // SETTINGSPATHS_H
