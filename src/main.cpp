/****************************************************************************
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QSettings>


/*!
	Entry point to UniversalIndentGUI application. Does not evaluate any command line parameters.
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTranslator translator;
    QSettings *settings;
    QString language;

    // read last selected language from settings if the settings file exists
    if ( QFile::exists("./UniversalIndentGUI.ini") ) {
        settings = new QSettings("./UniversalIndentGUI.ini", QSettings::IniFormat);
        language = settings->value("UniversalIndentGUI/language").toString();

        delete settings;
    }

    // if no language was set use the system language
    if ( language.isEmpty() ) {
        language = QLocale::system().name();
        language.truncate(2);
    }        

    // load the translation file and set it for the application
    translator.load( QString("./translations/universalindent_") + language );
    app.installTranslator(&translator);

    MainWindow mainWindow( language );

    mainWindow.show();
	
    return app.exec();
}
