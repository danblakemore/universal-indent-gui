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

#include "MainWindow.h"

#include "UiGuiIndentServer.h"
#include "debugging/TSLogger.h"
#include "UiGuiIniFileParser.h"
#include "UiGuiSettings.h"
#include "UiGuiVersion.h"
#include "UiGuiSystemInfo.h"
#include "IndentHandler.h"
#include "SettingsPaths.h"

#include <QApplication>
#include <QTextCodec>
#include <QDebug>

#include <string>
#include <iostream>
#include <algorithm>
#include <tclap/CmdLine.h>

#ifdef _MSC_VER

#include <windows.h>
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

/**
	\brief Calling this function tries to attach to the console of the parent process and
	redirect all inputs and outputs from and to this console.

	The redirected streams are stdout, stdin, stderr, cout, wcout, cin, wcin, wcerr, cerr, wclog and clog.

	Code based on info from http://dslweb.nwnexus.com/~ast/dload/guicon.htm.
 */
bool attachToConsole(/*enum ATTACH_ONLY|TRY_ATTACH_ELSE_CREATE|CREATE_NEW*/)
{
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;

	// Trying to attach to the console of the parent process.
	BOOL successful = AttachConsole(ATTACH_PARENT_PROCESS);
	// In case that the parent process has no console return false and do no input/output redirection.
	if ( !successful )
		return false;

	// Set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	// Set maximum console lines.
	coninfo.dwSize.Y = 500;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	// Redirect unbuffered STDOUT to the console.
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
        if ( hConHandle != -1 ) {
            fp = _fdopen( hConHandle, "w" );
            *stdout = *fp;
            setvbuf( stdout, NULL, _IONBF, 0 );
        }

	// Redirect unbuffered STDIN to the console.
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
        if ( hConHandle != -1 ) {
            fp = _fdopen( hConHandle, "r" );
            *stdin = *fp;
            setvbuf( stdin, NULL, _IONBF, 0 );
        }

	// Redirect unbuffered STDERR to the console.
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
        if ( hConHandle != -1 ) {
            fp = _fdopen( hConHandle, "w" );
            *stderr = *fp;
            setvbuf( stderr, NULL, _IONBF, 0 );
        }

	// Make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well.
	std::ios::sync_with_stdio();

	return true;
}
#else
bool attachToConsole()
{
    return false;
}

#endif

using namespace tschweitzer::debugging;

/*!
    /brief Entry point to UniversalIndentGUI application.

    Evaluates the following parameters:
    No parameters starts without server and full gui.
    A string without any parameter prefix will be loaded as file on start.
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
	bool tclapExitExceptionThrown = false;
	int returnValue = 0;

	bool attachedToConsole = false;
	attachedToConsole = attachToConsole();

#ifdef __APPLE__
		// Filter out -psn_0_118813 and similar parameters.
		std::vector<char*> argList;
		for ( int i = 0; i < argc; i++ ) {
			QString argString(argv[i]);

			if ( argString.startsWith("-psn_") == false ) {
				argList.push_back(argv[i]);
			}
			else {
//				std::cerr << std::endl << "The parameter "<< i << " is an invalid finder parameter. Parameter was " << argv[i] << std::endl;
			}
		}
		for ( size_t i = 0; i < argList.size(); i++ ) {
			argv[i] = argList.at(i);
		}
		argc = argList.size();
#endif

	// Wrap everything in a try block. Do this every time,
	// because exceptions will be thrown for problems.
	try {
		// Define the command line object.
		TCLAP::CmdLine cmd("If -p and -s are set, -p will be used.\nGiving no parameters starts full gui without server.", ' ', "UiGUI version " PROGRAM_VERSION_STRING " " PROGRAM_REVISION);
		cmd.setExceptionHandling(false);

		// Define a value argument and add it to the command line.
                TCLAP::UnlabeledValueArg<std::string> filenameArg("file", "Opens the by filename defined file on start" , false, "", "filename");
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
                file2OpenOnStart = filenameArg.getValue().c_str();
		startAsPlugin = pluginSwitch.getValue();
		startAsServer = serverSwitch.getValue();
		verboseLevel = verboselevelArg.getValue();
	}
	catch (TCLAP::ArgException &e) { // catch arg exceptions
		std::cerr << std::endl << "error: " << e.error() << ". " << e.argId() << std::endl;
		returnValue = 1;
	}
	catch (TCLAP::ExitException &e) { // catch exit exceptions
		tclapExitExceptionThrown = true;
		returnValue = e.getExitStatus();
	}
	catch (...) { // catch any exceptions
		std::cerr << std::endl << "There was an error! Maybe faulty command line arguments set. See --help." << std::endl;
		returnValue = 1;
	}

	if ( returnValue != 0 || tclapExitExceptionThrown ) {
#ifdef _MSC_VER
		if ( attachedToConsole ) {
			// Workaround for skipped command line prompt: Get the current working directory and print it to console.
			char* buffer;
			if( (buffer = _getcwd( NULL, 0 )) != NULL ) {
				std::cerr << std::endl << buffer << ">";
				free(buffer);
			}
			// Release the connection to the parents console.
			FreeConsole();
		}
#endif
		return returnValue;
	}

    QApplication app(argc, argv);
    UiGuiIndentServer server;
    MainWindow *mainWindow = NULL;
    IndentHandler *indentHandler = NULL;

    // Init and install the logger function.
    // Setting UTF-8 as default 8-Bit encoding to ensure that qDebug does no false string conversion.
    QTextCodec::setCodecForCStrings( QTextCodec::codecForName("UTF-8") );
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") );
    // Force creation of an TSLogger instance here, to avoid recursion with SettingsPaths init function.
#ifdef _DEBUG
    TSLogger::getInstance(0);
#else
    TSLogger::getInstance(verboseLevel);
#endif
    qInstallMsgHandler( TSLogger::messageHandler );
    TSLogger::messageHandler( TSLoggerInfoMsg, QString("Starting UiGUI Version %1 %2").arg(PROGRAM_VERSION_STRING).arg(PROGRAM_REVISION).toAscii() );
    TSLogger::messageHandler( TSLoggerInfoMsg, QString("Running on %1").arg(UiGuiSystemInfo::getOperatingSystem()).toAscii() );

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

    try {
        returnValue = app.exec();
    }
    catch (std::exception &ex) {
        qCritical() << __LINE__ << " " << __FUNCTION__ << ": Something went terribly wrong:" << ex.what();
    }

    if ( startAsPlugin || startAsServer )
        server.stopServer();

    delete indentHandler;
	delete mainWindow;

    SettingsPaths::cleanAndRemoveTempDir();
    TSLogger::deleteInstance();

    return returnValue;
}
