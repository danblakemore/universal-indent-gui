/***************************************************************************
 *   Copyright (C) 2006-2009 by Thomas Schweitzer                          *
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

#include "MainWindow.h"
#include <QApplication>

#include "UiGuiIndentServer.h"
#include "UiGuiLogger.h"
#include "UiGuiIniFileParser.h"
#include "UiGuiSettings.h"
#include "UiGuiVersion.h"
#include "UiGuiSystemInfo.h"

#include <string>
#include <iostream>
#include <algorithm>
#include <tclap/CmdLine.h>

/*!
    /brief Entry point to UniversalIndentGUI application.

    Evaluates the following parameters:
    No parameters starts without server and full gui.
    -f filename --file filename   : Opens the by filename defined file on start.
    -p --plugin  : Run as plugin. Server will be started with a simplified gui.
    -s --server  : Run as server only without gui.
    If -p and -s are set, -p will be used.
	-v --verbose needs a following parameter defining the verbose level as a number from 0 to 3.
 */
int main(int argc, char *argv[]) {
	QString file2OpenOnStart = "";
	int verboseLevel = 1;
	bool startAsPlugin = false;
	bool startAsServer = false;

	// Wrap everything in a try block.  Do this every time, 
	// because exceptions will be thrown for problems. 
	try {  
		// Define the command line object.
		TCLAP::CmdLine cmd("If -p and -s are set, -p will be used.\nGiving no parameters starts full gui without server.", ' ', "UiGUI version " PROGRAM_VERSION_STRING " " PROGRAM_REVISION);
		//cmd.setExceptionHandling(false);

		// Define a value argument and add it to the command line.
		TCLAP::ValueArg<std::string> filenameArg("f", "file", "Opens the by filename defined file on start" , false, "", "string");
		cmd.add( filenameArg );

		// Define a switch and add it to the command line.
		TCLAP::SwitchArg pluginSwitch("p", "plugin", "Run as plugin. Server will be started with a simplified gui", false);
		cmd.add( pluginSwitch );

		// Define a switch and add it to the command line.
		TCLAP::SwitchArg serverSwitch("s", "server", "Run as server only without gui", false);
		cmd.add( serverSwitch );

		// Define a value argument and add it to the command line.
		TCLAP::ValueArg<int> verboselevelArg("v", "verbose", "Sets how many info is written to the log. 0 means with debug info, 3 means critical messages only" , false, 1, "int");
		cmd.add( verboselevelArg );

		// Parse the args.
		cmd.parse( argc, argv );

		// Get the value parsed by each arg. 
		file2OpenOnStart = QString::fromStdString( filenameArg.getValue() );
		startAsPlugin = pluginSwitch.getValue();
		startAsServer = serverSwitch.getValue();
		verboseLevel = verboselevelArg.getValue();
	} catch (TCLAP::ArgException &e) { // catch any exceptions
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
	}


    QApplication app(argc, argv);
    UiGuiIndentServer server;
    MainWindow *mainWindow = NULL;
    IndentHandler *indentHandler = NULL;

    // Init and install the logger function.
    // Setting UTF-8 as default 8-Bit encoding to ensure that qDebug does no false string conversion.
    QTextCodec::setCodecForCStrings( QTextCodec::codecForName("UTF-8") );
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") );
    // Force creation of an UiGuiLogger instance here, to avoid recursion with SettingsPaths init function.
#ifdef _DEBUG
    UiGuiLogger::getInstance(0);
#else
    UiGuiLogger::getInstance(verboseLevel);
#endif
    qInstallMsgHandler( UiGuiLogger::messageHandler );
    UiGuiLogger::messageHandler( UiGuiInfoMsg, QString("Starting UiGUI Version %1 %2").arg(PROGRAM_VERSION_STRING).arg(PROGRAM_REVISION).toAscii() );
    UiGuiLogger::messageHandler( UiGuiInfoMsg, QString("Running on %1").arg(UiGuiSystemInfo::getOperatingSystem()).toAscii() );

    // Set default values for all by UniversalIndentGUI used settings objects.
    QCoreApplication::setOrganizationName("UniversalIndentGUI");
    QCoreApplication::setOrganizationDomain("universalindent.sf.net");
    QCoreApplication::setApplicationName("UniversalIndentGUI");

    // Start normal with full gui and without server.
    if ( !startAsPlugin && !startAsServer ) {
        mainWindow = new MainWindow(file2OpenOnStart);
        mainWindow->show();
    }
    // Start as plugin with server.
    else if ( startAsPlugin ) {
        server.startServer();
        indentHandler = new IndentHandler(0);
        indentHandler->show();
    }
    // Start as server only without any gui.
    else if ( startAsServer ) {
        server.startServer();
    }

    int returnValue = app.exec();

    if ( startAsPlugin || startAsServer != NULL)
        server.stopServer();

    delete indentHandler;
	delete mainWindow;

    UiGuiSettings::deleteInstance();
    UiGuiLogger::deleteInstance();

    return returnValue;
}
