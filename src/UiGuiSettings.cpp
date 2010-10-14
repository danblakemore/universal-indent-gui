/***************************************************************************
*   Copyright (C) 2006-2010 by Thomas Schweitzer                          *
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

#include "UiGuiSettings.h"

#include "SettingsPaths.h"

#include <QSettings>
#include <QPoint>
#include <QSize>
#include <QDir>
#include <QDate>
#include <QStringList>
#include <QCoreApplication>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QWidget>

//! \defgroup grp_Settings All concerning the settings.

/*!
    \class UiGuiSettings
    \ingroup grp_Settings
    \brief Handles the settings of the program. Reads them on startup and saves them on exit.
    Is a singleton class and can only be accessed via getInstance().
*/

// Inits the single class instance pointer.
QWeakPointer<UiGuiSettings> UiGuiSettings::_instance;


/*!
    \brief The constructor for the settings.
*/
UiGuiSettings::UiGuiSettings() : QObject() {
    // Create the main application settings object from the UniversalIndentGUI.ini file.
    _qsettings = new QSettings(SettingsPaths::getSettingsPath() + "/UniversalIndentGUI.ini", QSettings::IniFormat, this);

    _indenterDirctoryStr = SettingsPaths::getGlobalFilesPath() + "/indenters";
    readAvailableTranslations();
    initSettings();
}


/*!
    \brief Returns the instance of the settings class. If no instance exists, ONE will be created.
 */
QSharedPointer<UiGuiSettings> UiGuiSettings::getInstance() {
	QSharedPointer<UiGuiSettings> sharedInstance = _instance.toStrongRef();
    if ( sharedInstance.isNull() ) {
        // Create the settings object, which loads all UiGui settings from a file.
        sharedInstance = QSharedPointer<UiGuiSettings>(new UiGuiSettings());
        _instance = sharedInstance.toWeakRef();
    }

    return sharedInstance;
}


/*!
    \brief The destructor saves the settings to a file.
 */
UiGuiSettings::~UiGuiSettings() {
    // Convert the language setting from an integer index to a string.
    int index = _qsettings->value("UniversalIndentGUI/language", 0).toInt();
    if ( index < 0 || index >= _availableTranslations.size() )
        index = 0;

    _qsettings->setValue( "UniversalIndentGUI/language", _availableTranslations.at(index) );
}


/*!
    \brief Scans the translations directory for available translation files and
    stores them in the QList \a _availableTranslations.
 */
void UiGuiSettings::readAvailableTranslations() {
    QString languageShort;
    QStringList languageFileList;

    // English is the default language. A translation file does not exist but to have a menu entry, added here.
    languageFileList << "universalindent_en.qm";

    // Find all translation files in the "translations" directory.
    QDir translationDirectory = QDir( SettingsPaths::getGlobalFilesPath() + "/translations" );
    languageFileList << translationDirectory.entryList( QStringList("universalindent_*.qm") );

    // Loop for each found translation file
    foreach ( languageShort, languageFileList ) {
        // Remove the leading string "universalindent_" from the filename.
        languageShort.remove(0,16);
        // Remove trailing file extension ".qm".
        languageShort.chop(3);

        _availableTranslations.append(languageShort);
    }
}


/*!
    \brief Returns a list of the mnemonics of the available translations.
 */
QStringList UiGuiSettings::getAvailableTranslations() {
    return _availableTranslations;
}


/*!
    \brief Returns the value of the by \a settingsName defined setting as QVariant.

    If the named setting does not exist, 0 is being returned.
*/
QVariant UiGuiSettings::getValueByName(QString settingName) {
    return _qsettings->value("UniversalIndentGUI/" + settingName);
}


/*!
    \brief Loads the settings for the main application.

    Settings are for example last selected indenter, last loaded source code file and so on.
*/
bool UiGuiSettings::initSettings()
{
    // Read the version string saved in the settings file.
    _qsettings->setValue( "UniversalIndentGUI/version", _qsettings->value("UniversalIndentGUI/version", "") );

    // Read windows last size and position from the settings file.
    _qsettings->setValue( "UniversalIndentGUI/maximized", _qsettings->value("UniversalIndentGUI/maximized", false) );
    _qsettings->setValue( "UniversalIndentGUI/position", _qsettings->value("UniversalIndentGUI/position", QPoint(50, 50)) );
    _qsettings->setValue( "UniversalIndentGUI/size", _qsettings->value("UniversalIndentGUI/size", QSize(800, 600)) );

    // Read last selected encoding for the opened source code file.
    _qsettings->setValue( "UniversalIndentGUI/encoding", _qsettings->value("UniversalIndentGUI/encoding", "UTF-8") );

    // Read maximum length of list for recently opened files.
    _qsettings->setValue( "UniversalIndentGUI/recentlyOpenedListSize", _qsettings->value("UniversalIndentGUI/recentlyOpenedListSize", 5) );

    // Read if last opened source code file should be loaded on startup.
    _qsettings->setValue( "UniversalIndentGUI/loadLastSourceCodeFileOnStartup", _qsettings->value("UniversalIndentGUI/loadLastSourceCodeFileOnStartup", true) );

    // Read last opened source code file from the settings file.
    _qsettings->setValue( "UniversalIndentGUI/lastSourceCodeFile", _qsettings->value("UniversalIndentGUI/lastSourceCodeFile",  _indenterDirctoryStr+"/example.cpp") );

    // Read last selected indenter from the settings file.
    int selectedIndenter = _qsettings->value("UniversalIndentGUI/selectedIndenter", 0).toInt();
    if ( selectedIndenter < 0 ) {
        selectedIndenter = 0;
    }
    _qsettings->setValue( "UniversalIndentGUI/selectedIndenter", selectedIndenter );

    // Read if syntax highlighting is enabled.
    _qsettings->setValue( "UniversalIndentGUI/SyntaxHighlightingEnabled", _qsettings->value("UniversalIndentGUI/SyntaxHighlightingEnabled", true) );

    // Read if white space characters should be displayed.
    _qsettings->setValue( "UniversalIndentGUI/whiteSpaceIsVisible", _qsettings->value("UniversalIndentGUI/whiteSpaceIsVisible", false) );

    // Read if indenter parameter tool tips are enabled.
    _qsettings->setValue( "UniversalIndentGUI/indenterParameterTooltipsEnabled", _qsettings->value("UniversalIndentGUI/indenterParameterTooltipsEnabled", true) );

    // Read the tab width from the settings file.
    _qsettings->setValue( "UniversalIndentGUI/tabWidth", _qsettings->value("UniversalIndentGUI/tabWidth", 4) );

    // Read the last selected language and stores the index it has in the list of available translations.
    _qsettings->setValue( "UniversalIndentGUI/language", _availableTranslations.indexOf( _qsettings->value("UniversalIndentGUI/language", "").toString() ) );

    // Read the update check settings from the settings file.
    _qsettings->setValue( "UniversalIndentGUI/CheckForUpdate", _qsettings->value("UniversalIndentGUI/CheckForUpdate", true) );
    _qsettings->setValue( "UniversalIndentGUI/LastUpdateCheck", _qsettings->value("UniversalIndentGUI/LastUpdateCheck", QDate(1900,1,1)) );

    // Read the main window state.
    _qsettings->setValue( "UniversalIndentGUI/MainWindowState", _qsettings->value("UniversalIndentGUI/MainWindowState", QByteArray()) );

    return true;
}


/*!
    \brief Register the by \a propertyName defined property of \a obj to be connected to the setting defined by \a settingName.

    The \a propertyName must be one of those that are listed in the Qt "Properties" documentation section of a Qt Object.
    All further needed info is retrieved via the \a obj's MetaObject, like the to the property bound signal.
 */
bool UiGuiSettings::registerObjectProperty( QObject *obj, const QString &propertyName, const QString &settingName )
{
    const QMetaObject *metaObject = obj->metaObject();
    bool connectSuccess = false;

    // Connect to the objects destroyed signal, so that it will be correctly unregistered.
    connectSuccess = connect(obj, SIGNAL(destroyed(QObject*)), this, SLOT(unregisterObjectProperty(QObject*)));

    int indexOfProp = metaObject->indexOfProperty( qPrintable(propertyName) );
    if ( connectSuccess && indexOfProp > -1 ) {
        QMetaProperty mProp = metaObject->property(indexOfProp);

        // Connect to the property's value changed signal.
        if ( mProp.hasNotifySignal() ) {
            QMetaMethod signal = mProp.notifySignal();
            //QString teststr = qPrintable(SIGNAL() + QString(signal.signature()));
            // The command "SIGNAL() + QString(signal.signature())" assembles the signal methods signature to a valid Qt SIGNAL.
            connectSuccess = connect(obj, qPrintable(SIGNAL() + QString(signal.signature())), this, SLOT(handleObjectPropertyChange()));
        }

        if ( connectSuccess ) {
            _registeredObjectProperties[obj] = QStringList() << propertyName << settingName;
        }
        else {
            //TODO: Write a debug warning to the log.
            disconnect(obj, SIGNAL(destroyed(QObject*)), this, SLOT(unregisterObjectProperty(QObject*)));
            return false;
        }

        // If setting already exists, set the objects property to the setting value.
        if ( _qsettings->contains("UniversalIndentGUI/" + settingName) ) {
            mProp.write(obj, _qsettings->value("UniversalIndentGUI/" + settingName));
        }
        // Otherwise add the setting and set it to the value of the objects property.
        else {
            _qsettings->setValue("UniversalIndentGUI/" + settingName, mProp.read(obj));
        }
    }
    else {
        //TODO: Write a debug warning to the log.
        disconnect(obj, SIGNAL(destroyed(QObject*)), this, SLOT(unregisterObjectProperty(QObject*)));
        return false;
    }

    return true;
}


/*!
    \brief Searches the child QObjects of \a obj for a property name and setting name definition within
    their custom properties and registers this property name to that setting name if both were found.

    The custom properties, for which are searched, are "connectedPropertyName" and "connectedSettingName",
    where "connectedPropertyName" is the name of a QObject property as it is documented in the QtDocs, and
    "connectedSettingName" is the name of a setting here within UiGuiSettings. If the mentioned setting
    name doesn't exist, it will be created.

    Returns true, if all found property and setting definitions could be successfully registered.
    Returns false, if any of those registrations fails.
 */
bool UiGuiSettings::registerObjectPropertyRecursive(QObject *obj) {
    return checkCustomPropertiesAndCallFunction(obj, &UiGuiSettings::registerObjectProperty);
}


/*!
    \brief Assigns the by \a settingName defined setting value to the by \a propertyName defined property of \a obj.

    Returns true, if the value could be assigned, otherwise returns false, which is the case if settingName doesn't exist
    within the settings or if the mentioned propertyName wasn't found for the \a obj.
 */
bool UiGuiSettings::setObjectPropertyToSettingValue( QObject *obj, const QString &propertyName, const QString &settingName )
{
    const QMetaObject *metaObject = obj->metaObject();

    int indexOfProp = metaObject->indexOfProperty( qPrintable(propertyName) );
    if ( indexOfProp > -1 ) {
        QMetaProperty mProp = metaObject->property(indexOfProp);

        // If setting already exists, set the objects property to the setting value.
        if ( _qsettings->contains("UniversalIndentGUI/" + settingName) ) {
            mProp.write(obj, _qsettings->value("UniversalIndentGUI/" + settingName));
        }
        // The setting didn't exist so return that setting the objects property failed.
        else {
            //TODO: Write a debug warning to the log.
            return false;
        }
    }
    else {
        //TODO: Write a debug warning to the log.
        return false;
    }

    return true;
}


/*!
    \brief Searches the child QObjects of \a obj for a property name and setting name definition within
    their custom properties and sets each property to settings value.

    The custom properties, for which are searched, are "connectedPropertyName" and "connectedSettingName",
    where "connectedPropertyName" is the name of a QObject property as it is documented in the QtDocs, and
    "connectedSettingName" is the name of a setting here within UiGuiSettings.

    Returns true, if all found property and setting definitions could be successfully registered.
    Returns false, if any of those registrations fails.
 */
bool UiGuiSettings::setObjectPropertyToSettingValueRecursive(QObject *obj) {
    return checkCustomPropertiesAndCallFunction(obj, &UiGuiSettings::setObjectPropertyToSettingValue);
}


/*!
    \brief Assigns the by \a propertyName defined property's value of \a obj to the by \a settingName defined setting.

    If the \a settingName didn't exist yet, it will be created.

    Returns true, if the value could be assigned, otherwise returns false, which is the case if the mentioned
    propertyName wasn't found for the \a obj.
 */
bool UiGuiSettings::setSettingToObjectPropertyValue( QObject *obj, const QString &propertyName, const QString &settingName )
{
    const QMetaObject *metaObject = obj->metaObject();

    int indexOfProp = metaObject->indexOfProperty( qPrintable(propertyName) );
    if ( indexOfProp > -1 ) {
        QMetaProperty mProp = metaObject->property(indexOfProp);

        setValueByName(settingName, mProp.read(obj));
    }
    else {
        //TODO: Write a debug warning to the log.
        return false;
    }

    return true;
}


/*!
    \brief Searches the child QObjects of \a obj for a property name and setting name definition within
    their custom properties and sets each setting to the property value.

    The custom properties, for which are searched, are "connectedPropertyName" and "connectedSettingName",
    where "connectedPropertyName" is the name of a QObject property as it is documented in the QtDocs, and
    "connectedSettingName" is the name of a setting here within UiGuiSettings. If the settingName
    didn't exist yet, it will be created.

    Returns true, if all found property and setting definitions could be successfully registered.
    Returns false, if any of those registrations fails.
 */
bool UiGuiSettings::setSettingToObjectPropertyValueRecursive(QObject *obj) {
    return checkCustomPropertiesAndCallFunction(obj, &UiGuiSettings::setSettingToObjectPropertyValue);
}


/*!
    \brief Iterates over all \a objs child QObjects and checks whether they have the custom properties
    "connectedPropertyName" and "connectedSettingName" set. If both are set, it invokes the \a callBackFunc
    with both.
 */
bool UiGuiSettings::checkCustomPropertiesAndCallFunction(QObject *obj, bool (UiGuiSettings::*callBackFunc)(QObject *obj, const QString &propertyName, const QString &settingName)) {
    bool success = true;

    // Find all widgets that have PropertyName and SettingName defined in their style sheet.
    QList<QObject *> allObjects = obj->findChildren<QObject *>();
    foreach (QObject *object, allObjects) {
        QString propertyName = object->property("connectedPropertyName").toString();
        QString settingName = object->property("connectedSettingName").toString();

        // If property and setting name were found, register that widget with the settings.
        if ( !propertyName.isEmpty() && !settingName.isEmpty() ) {
            success &= (this->*callBackFunc)( object, propertyName, settingName );
        }
    }

    return success;
}


/*!
    \brief The with a certain property registered \a obj gets unregistered.
 */
void UiGuiSettings::unregisterObjectProperty(QObject *obj) {
    if ( _registeredObjectProperties.contains(obj) ) {
        const QMetaObject *metaObject = obj->metaObject();
        QString propertyName = _registeredObjectProperties[obj].first();
        QString settingName = _registeredObjectProperties[obj].last();

        bool connectSuccess = false;
        int indexOfProp = metaObject->indexOfProperty( qPrintable(propertyName) );
        if ( indexOfProp > -1 ) {
            QMetaProperty mProp = metaObject->property(indexOfProp);

            // Disconnect to the property's value changed signal.
            if ( mProp.hasNotifySignal() ) {
                QMetaMethod signal = mProp.notifySignal();
                // The command "SIGNAL() + QString(signal.signature())" assembles the signal methods signature to a valid Qt SIGNAL.
                connectSuccess = disconnect(obj, qPrintable(SIGNAL() + QString(signal.signature())), this, SLOT(handleObjectPropertyChange()));
            }
        }
        _registeredObjectProperties.remove(obj);
    }
}


/*!
    \brief Registers a slot form the \a obj by its \a slotName to be invoked, if the by \a settingName defined
    setting changes.

    The registered slot may have no parameters or exactly one. If it accepts one parameter, whenever the setting
    \a settingName changes the slot gets tried to be invoked with the settings value as parameter. This only works,
    if the slot parameter is of the same type as the setting.
 */
bool UiGuiSettings::registerObjectSlot(QObject *obj, const QString &slotName, const QString &settingName) {

    const QMetaObject *metaObject = obj->metaObject();

    bool connectSuccess = false;
    // Connect to the objects destroyed signal, so that it will be correctly unregistered.
    connectSuccess = connect(obj, SIGNAL(destroyed(QObject*)), this, SLOT(unregisterObjectSlot(QObject*)));

    QString normalizedSlotName = QMetaObject::normalizedSignature( qPrintable(slotName) );
    int indexOfMethod = metaObject->indexOfMethod( qPrintable(normalizedSlotName) );
    if ( connectSuccess && indexOfMethod > -1 ) {
        QMetaMethod mMethod = metaObject->method(indexOfMethod);
        QMetaMethod::Access access = mMethod.access();
        QMetaMethod::MethodType methType = mMethod.methodType();

        // Since the method can at maximum be invoked with the setting value as argument,
        // only methods taking max one argument are allowed.
        if ( mMethod.parameterTypes().size() <= 1 ) {
            _registeredObjectSlots.insert(obj, QStringList() << normalizedSlotName << settingName);
        }
        else {
            //TODO: Write a debug warning to the log.
            disconnect(obj, SIGNAL(destroyed(QObject*)), this, SLOT(unregisterObjectSlot(QObject*)));
            return false;
        }
    }
    else {
        //TODO: Write a debug warning to the log.
        disconnect(obj, SIGNAL(destroyed(QObject*)), this, SLOT(unregisterObjectSlot(QObject*)));
        return false;
    }

    return true;
}


/*!
    \brief If \a obj, \a slotName and \a settingName are given, that certain connection is unregistered.
    If only \a obj is given, all to this object registered slot-setting connections are unregistered.
 */
void UiGuiSettings::unregisterObjectSlot(QObject *obj, const QString &slotName, const QString &settingName) {
    const QMetaObject *metaObject = obj->metaObject();
    QString normalizedSlotName = QMetaObject::normalizedSignature( qPrintable(slotName) );
    QMutableMapIterator<QObject*, QStringList> it(_registeredObjectSlots);
    while (it.hasNext()) {
        it.next();
        if (it.key() == obj && slotName.isEmpty() && settingName.isEmpty())
            it.remove();
        else if (it.key() == obj && it.value().first() == slotName && it.value().last() == settingName)
            it.remove();
    }
}


/*!
    \brief This private slot gets invoked whenever a registered objects property changes
    and distributes the new value to all other to the same settingName registered objects.
 */
void UiGuiSettings::handleObjectPropertyChange() {
    QObject *obj = QObject::sender();
    QString className = obj->metaObject()->className();
    const QMetaObject *metaObject = obj->metaObject();
    QString propertyName = _registeredObjectProperties[obj].first();
    QString settingName = _registeredObjectProperties[obj].last();

    int indexOfProp = metaObject->indexOfProperty( qPrintable(propertyName) );
    if ( indexOfProp > -1 ) {
        QMetaProperty mProp = metaObject->property(indexOfProp);
        setValueByName(settingName, mProp.read(obj));
    }
}


/*!
    \brief Sets the setting defined by \a settingName to \a value.

    When setting a changed value, all to this settingName registered objects get
    the changed value set too.
    If the \a settingName didn't exist yet, it will be created.
 */
void UiGuiSettings::setValueByName(const QString &settingName, const QVariant &value) {
    // Do the updating only, if the setting was really changed.
    if ( _qsettings->value("UniversalIndentGUI/" + settingName) != value ) {
        _qsettings->setValue("UniversalIndentGUI/" + settingName, value);

        // Set the new value for all registered object properties for settingName.
        for ( QMap<QObject*, QStringList>::ConstIterator it = _registeredObjectProperties.begin(); it != _registeredObjectProperties.end(); ++it ) {
            if ( it.value().last() == settingName ) {
                QObject *obj = it.key();
                const QMetaObject *metaObject = obj->metaObject();
                QString propertyName = it.value().first();

                int indexOfProp = metaObject->indexOfProperty( qPrintable(propertyName) );
                if ( indexOfProp > -1 ) {
                    QMetaProperty mProp = metaObject->property(indexOfProp);
                    mProp.write(obj, value);
                }
            }
        }

        // Invoke all registered object methods for settingName.
        for ( QMap<QObject*, QStringList>::ConstIterator it = _registeredObjectSlots.begin(); it != _registeredObjectSlots.end(); ++it ) {
            if ( it.value().last() == settingName ) {
                QObject *obj = it.key();
                const QMetaObject *metaObject = obj->metaObject();
                QString slotName = it.value().first();

                int indexOfMethod = metaObject->indexOfMethod( qPrintable(slotName) );
                if ( indexOfMethod > -1 ) {
                    QMetaMethod mMethod = metaObject->method(indexOfMethod);
                    QMetaMethod::Access access = mMethod.access();
                    QMetaMethod::MethodType methType = mMethod.methodType();

                    bool success = false;

                    // Handle registered slots taking one parameter.
                    if ( mMethod.parameterTypes().size() == 1 ) {
                        if ( mMethod.parameterTypes().first() == value.typeName() ) {
                            success = invokeMethodWithValue(obj, mMethod, value);
                        }
                    }
                    // Handle registered slots taking zero parameters.
                    else {
                        success = mMethod.invoke( obj, Qt::DirectConnection );
                    }

                    if ( success == false ) {
                        // TODO: Write a warning to the log if no success.
                    }
                }
            }
        }
    }
}


#include <QBitArray>
#include <QBitmap>
#include <QBrush>
#include <QCursor>
#include <QDateTime>
#include <QFont>
#include <QIcon>
#include <QKeySequence>
#include <QLocale>
#include <QPalette>
#include <QPen>
#include <QSizePolicy>
#include <QTextFormat>
#include <QTextLength>
#include <QUrl>
#if QT_VERSION >= 0x040600
#include <QMatrix4x4>
#include <QVector2D>
#endif

bool UiGuiSettings::invokeMethodWithValue( QObject *obj, QMetaMethod mMethod, QVariant value )
{
    switch (value.type()) {
    case QVariant::BitArray :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QBitArray, value.toBitArray()) );
    case QVariant::Bitmap :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QBitmap, value.value<QBitmap>()) );
    case QVariant::Bool :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(bool, value.toBool()) );
    case QVariant::Brush :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QBrush, value.value<QBrush>()) );
    case QVariant::ByteArray :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QByteArray, value.toByteArray()) );
    case QVariant::Char :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QChar, value.toChar()) );
    case QVariant::Color :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QColor, value.value<QColor>()) );
    case QVariant::Cursor :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QCursor, value.value<QCursor>()) );
    case QVariant::Date :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QDate, value.toDate()) );
    case QVariant::DateTime :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QDateTime, value.toDateTime()) );
    case QVariant::Double :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(double, value.toDouble()) );
    case QVariant::Font :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QFont, value.value<QFont>()) );
    case QVariant::Hash :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QVariantHash, value.toHash()) );
    case QVariant::Icon :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QIcon, value.value<QIcon>()) );
    case QVariant::Image :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QImage, value.value<QImage>()) );
    case QVariant::Int :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(int, value.toInt()) );
    case QVariant::KeySequence :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QKeySequence, value.value<QKeySequence>()) );
    case QVariant::Line :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QLine, value.toLine()) );
    case QVariant::LineF :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QLineF, value.toLineF()) );
    case QVariant::List :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QVariantList, value.toList()) );
    case QVariant::Locale :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QLocale, value.toLocale()) );
    case QVariant::LongLong :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(qlonglong, value.toLongLong()) );
    case QVariant::Map :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QVariantMap, value.toMap()) );
    case QVariant::Matrix :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QMatrix, value.value<QMatrix>()) );
    case QVariant::Transform :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QTransform, value.value<QTransform>()) );
#if QT_VERSION >= 0x040600
    case QVariant::Matrix4x4 :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QMatrix4x4, value.value<QMatrix4x4>()) );
#endif
    case QVariant::Palette :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QPalette, value.value<QPalette>()) );
    case QVariant::Pen :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QPen, value.value<QPen>()) );
    case QVariant::Pixmap :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QPixmap, value.value<QPixmap>()) );
    case QVariant::Point :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QPoint, value.toPoint()) );
        //    case QVariant::PointArray :
        //        return Q_ARG(QPointArray, value.value<QPointArray>()) );
    case QVariant::PointF :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QPointF, value.toPointF()) );
    case QVariant::Polygon :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QPolygon, value.value<QPolygon>()) );
#if QT_VERSION >= 0x040600
    case QVariant::Quaternion :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QQuaternion, value.value<QQuaternion>()) );
#endif
    case QVariant::Rect :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QRect, value.toRect()) );
    case QVariant::RectF :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QRectF, value.toRectF()) );
    case QVariant::RegExp :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QRegExp, value.toRegExp()) );
    case QVariant::Region :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QRegion, value.value<QRegion>()) );
    case QVariant::Size :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QSize, value.toSize()) );
    case QVariant::SizeF :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QSizeF, value.toSizeF()) );
    case QVariant::SizePolicy :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QSizePolicy, value.value<QSizePolicy>()) );
    case QVariant::String :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QString, value.toString()) );
    case QVariant::StringList :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QStringList, value.toStringList()) );
    case QVariant::TextFormat :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QTextFormat, value.value<QTextFormat>()) );
    case QVariant::TextLength :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QTextLength, value.value<QTextLength>()) );
    case QVariant::Time :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QTime, value.toTime()) );
    case QVariant::UInt :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(uint, value.toUInt()) );
    case QVariant::ULongLong :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(qulonglong, value.toULongLong()) );
    case QVariant::Url :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QUrl, value.toUrl()) );
#if QT_VERSION >= 0x040600
    case QVariant::Vector2D :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QVector2D, value.value<QVector2D>()) );
    case QVariant::Vector3D :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QVector3D, value.value<QVector3D>()) );
    case QVariant::Vector4D :
        return mMethod.invoke( obj, Qt::DirectConnection, Q_ARG(QVector4D, value.value<QVector4D>()) );
#endif
    default:
        return false;
    }
}
