/***************************************************************************
*   Copyright (C) 2006-2007 by Thomas Schweitzer                          *
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

#ifndef UIGUISETTINGS_H
#define UIGUISETTINGS_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QSettings>
#include <QPoint>
#include <QSize>
#include <QMainWindow>
#include <QLocale>

class UiGuiSettings : public QObject
{
	Q_OBJECT

public:
	UiGuiSettings(QMainWindow *parent);
    virtual ~UiGuiSettings();
    bool loadSettings();
    bool saveSettings();

private:
    QMainWindow *parent;
    QString version;
    QSettings *settings;
    bool isFirstRunOfThisVersion;
    QString currentSourceFile;
    QString dataDirctoryStr;
    QString currentEncoding;
    bool loadLastSourceCodeFileOnStartup;
    QString sourceFileContent;
    QString currentSourceFileExtension;
    int currentIndenterID;
    QString language;
    bool indenterParameterTooltipsEnabled;
    bool whiteSpaceIsVisible;
    int tabWidth;
};

#endif // UIGUISETTINGS_H
