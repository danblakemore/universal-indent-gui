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
#include <QString>
#include <QSettings>
#include <QPoint>
#include <QSize>
#include <QDir>
#include <QStringList>

class UiGuiSettings : public QObject
{
	Q_OBJECT

public:
	UiGuiSettings(QString settingFilePath = "./UniversalIndentGUI.ini");
    virtual ~UiGuiSettings();
    bool loadSettings();
    bool saveSettings();
    bool setValueByName(QString settingName, QVariant value);
	QVariant getValueByName(QString settingName);
    void updatedAllDependend();
	QStringList getAvailableTranslations();

// Only one slot per possible value needed, because wanted setting
// is recognized by the sender objects name.
public slots:
	void handleValueChangeFromExtern(int value);
    void handleValueChangeFromExtern(bool value);

// Each possible setting needs an own signal.
signals:
    void versionInSettingsFile(QString value);
    void windowIsMaximized(bool value);
    void windowPosition(QPoint value);
    void windowSize(QSize value);
    void fileEncoding(QString value);
    void loadLastOpenedFileOnStartup(bool value);
    void lastOpenedFile(QString value);
    void lastSelectedIndenterID(int value);
    void syntaxHighlightningEnabled(bool value);
    void whiteSpaceIsVisible(bool value);
    void indenterParameterTooltipsEnabled(bool value);
    void tabWidth(int value);
    void language(QString value);

private:
    void emitSignalForSetting(QString settingName);
	void readAvailableTranslations();

	//! Stores the mnemonics of the available translations.
	QStringList availableTranslations;

    //! The settings file.
    QSettings *qsettings;

	//! This map holds all possible settings defined by their name as QString. The value is of the type QVariant.
	QMap<QString, QVariant> settings;
};

#endif // UIGUISETTINGS_H
