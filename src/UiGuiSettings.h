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

#ifndef UIGUISETTINGS_H
#define UIGUISETTINGS_H

#include <QObject>
#include <QString>
#include <QSettings>
#include <QPoint>
#include <QSize>
#include <QDir>
#include <QDate>
#include <QStringList>
#include <QCoreApplication>

class UiGuiSettings : public QObject
{
	Q_OBJECT
private:
    UiGuiSettings();
    static UiGuiSettings* instance;

public:
	static UiGuiSettings* getInstance();
    static void deleteInstance();
    ~UiGuiSettings();
    bool loadSettings();
    bool saveSettings();
    bool setValueByName(QString settingName, QVariant value);
	QVariant getValueByName(QString settingName);
    void updateAllDependend();
	QStringList getAvailableTranslations();

// Only one slot per possible value type needed, because wanted setting
// is recognized by the sender objects name.
public slots:
	void handleValueChangeFromExtern(int value);
    void handleValueChangeFromExtern(bool value);
    void handleValueChangeFromExtern(QDate value);
    void handleValueChangeFromExtern(QByteArray value);

// Each possible setting needs an own signal.
signals:
    void versionInSettingsFile(QString value);
    void windowIsMaximized(bool value);
    void windowPosition(QPoint value);
    void windowSize(QSize value);
    void fileEncoding(QString value);
    void recentlyOpenedListSize(int value);
    void loadLastOpenedFileOnStartup(bool value);
    void lastOpenedFiles(QString value);
    void selectedIndenter(int value);
    void syntaxHighlightningEnabled(bool value);
    void whiteSpaceIsVisible(bool value);
    void indenterParameterTooltipsEnabled(bool value);
    void tabWidth(int value);
    void language(int value);
    void checkForUpdate(bool value);
    void lastUpdateCheck(QDate value);
    void mainWindowState(QByteArray value);

private:
    void emitSignalForSetting(QString settingName);
	void readAvailableTranslations();

	//! Stores the mnemonics of the available translations.
	QStringList availableTranslations;

    //! The settings file.
    QSettings *qsettings;

	//! This map holds all possible settings defined by their name as QString. The value is of the type QVariant.
	QMap<QString, QVariant> settings;

    QString indenterDirctoryStr;
};

#endif // UIGUISETTINGS_H
