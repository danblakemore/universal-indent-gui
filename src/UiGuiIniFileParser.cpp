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

#include "UiGuiIniFileParser.h"

#include <QFile>
#include <QStringList>
#include <QVariant>
#include <QTextStream>

//! \defgroup grp_Settings All concerning applications settings.

/*!
    \class UiGuiIniFileParser
    \ingroup grp_Settings
    \brief UiGuiIniFileParser is a simple ini file format parser.

    These ini files need to have key-value pairs in the style
    "keyname=keyvalue". Groups can be defined by writing the groupname
    in the style [groupname] before some key-value pairs.

    The reason why I use my own class instead of QSettings is mainly, that
    QSettings always internally sorts the groups alphabetically and also
    rewrites a settings file sorted. Very annoying for me.
*/

/*!
    \brief Init and empty all needed lists and strings.
 */
UiGuiIniFileParser::UiGuiIniFileParser(void) {
	init();
}


/*!
    \brief Directly loads and parses the file with name \a iniFileName.
 */
UiGuiIniFileParser::UiGuiIniFileParser(const QString &iniFileName) {
	init();
    _iniFileName = iniFileName;
    parseIniFile();
}


void UiGuiIniFileParser::init() {
	_sections.clear();
	_keyValueMap.clear();
	_iniFileName = "";
}


UiGuiIniFileParser::~UiGuiIniFileParser(void) {
}


/*!
    \brief Returns the group/section names in the same order as they occurr in the ini file as QStringList.
 */
QStringList UiGuiIniFileParser::childGroups() {
    QStringList sectionsStringList;

    for( unsigned int i = 0; i < _sections.size(); i++ ) {
        sectionsStringList << _sections[i];
    }

    return sectionsStringList;
}


/*!
    \brief Returns the value of the defined \a keyName as QVariant.

    The \a keyName is assembled by a section name, a slash and the key name itself.
    For example if you wish to access the value of the following setting:
    <code>[NiceSection]</br>niceKeyName=2</code> you would have to call
    value("NiceSection/niceKeyName").
 */
QVariant UiGuiIniFileParser::value(const QString &keyName, const QString &defaultValue) {
    return _keyValueMap.value( keyName, defaultValue );
}


/*!
    \brief Parses the ini file and stores the key value pairs in the internal vectors \a keys and \a values.
 */
void UiGuiIniFileParser::parseIniFile() {
    QFile iniFile(_iniFileName);

    if ( iniFile.open(QFile::ReadOnly) ) {
        // Clear the vectors holding the keys and values.
        _sections.clear();
        _keyValueMap.clear();

        QTextStream iniFileStream( &iniFile );
        QString line;
        QString currentSectionName = "";
        QString keyName = "";
        QString valueAsString = "";

        while ( !iniFileStream.atEnd() ) {
            line = iniFileStream.readLine().trimmed();

            // Test if the read line is a section name and if so remeber it.
            if ( line.startsWith("[") && line.endsWith("]") ) {
                currentSectionName = line.remove(0, 1);
                currentSectionName.chop(1);

                // Store the section name.
                _sections.push_back( currentSectionName );
            }
            // Otherwise test whether the line has a assign char
            else if ( line.contains("=") ) {
                int indexOfFirstAssign = line.indexOf("=");
                keyName = line.left(indexOfFirstAssign);

                if ( !keyName.isEmpty() ) {
                    valueAsString = line.remove(0, indexOfFirstAssign+1);
                    // Remove any existing double quotes from the value.
                    if ( valueAsString.startsWith("\"") && valueAsString.endsWith("\"") ) {
                        valueAsString = valueAsString.remove(0, 1);
                        valueAsString.chop(1);
                    }

                    // Prepend an eventually section name to the key name.
                    if ( !currentSectionName.isEmpty() ) {
                        keyName = currentSectionName + "/" + keyName;
                    }

                    // Store the key and value in the map.
                    _keyValueMap.insert(keyName, valueAsString );
                }
            }
        }
    }
}
