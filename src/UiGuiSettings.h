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

#ifndef UIGUISETTINGS_H
#define UIGUISETTINGS_H

#include <QObject>
#include <QStringList>
#include <QMultiMap>
#include <QSharedPointer>

class QSettings;


class UiGuiSettings : public QObject
{
    Q_OBJECT
private:
    UiGuiSettings();
    static QWeakPointer<UiGuiSettings> _instance;

public:
    static QSharedPointer<UiGuiSettings> getInstance();
    ~UiGuiSettings();

    bool registerObjectProperty(QObject *obj, const QString &propertyName, const QString &settingName);
    bool registerObjectPropertyRecursive(QObject *obj);
    bool setObjectPropertyToSettingValue(QObject *obj, const QString &propertyName, const QString &settingName);
    bool setObjectPropertyToSettingValueRecursive(QObject *obj);
    bool setSettingToObjectPropertyValue(QObject *obj, const QString &propertyName, const QString &settingName);
    bool setSettingToObjectPropertyValueRecursive(QObject *obj);
    bool registerObjectSlot(QObject *obj, const QString &slotName, const QString &settingName);
    QVariant getValueByName(QString settingName);
    QStringList getAvailableTranslations();

public slots:
    void setValueByName(const QString &settingName, const QVariant &value);
    void unregisterObjectProperty(QObject *obj);
    void unregisterObjectSlot(QObject *obj, const QString &slotName = "", const QString &settingName = "");

protected:
    bool initSettings();
    bool invokeMethodWithValue(QObject *obj, QMetaMethod mMethod, QVariant value);
    bool checkCustomPropertiesAndCallFunction(QObject *obj, bool (UiGuiSettings::*callBackFunc)(QObject *obj, const QString &propertyName, const QString &settingName));

private slots:
    void handleObjectPropertyChange();

private:
    void readAvailableTranslations();

    //! Stores the mnemonics of the available translations.
    QStringList _availableTranslations;

    //! The settings file.
    QSettings *_qsettings;

    //! Maps an QObject to a string list containing the property name and the associated setting name.
    QMap<QObject*, QStringList> _registeredObjectProperties;

    //! Maps QObjects to a string list containing the method name and the associated setting name.
    QMultiMap<QObject*, QStringList> _registeredObjectSlots;

    QString _indenterDirctoryStr;
};

#endif // UIGUISETTINGS_H
