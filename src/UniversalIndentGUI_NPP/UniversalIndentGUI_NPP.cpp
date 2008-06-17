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


/* include files */
#include "stdafx.h"
#include "UniversalIndentGUI_NPP.h"
#include "UniversalIndentGUI_NPPDialog.h"

#include "indenthandler.h"


/* information for notepad */
CONST INT	nbFunc	= 3;
CONST CHAR	PLUGIN_NAME[] = "&UniversalIndentGUI_NPP";

/* global values */
HANDLE				g_hModule			= NULL;
NppData				nppData;
FuncItem			funcItem[nbFunc];
toolbarIcons		g_TBWndMgr;

IndentHandler *indentHandler;

/* dialog classes */
UniversalIndentGUI_NPPDialog universalIndentGUI_NPPDialog;


/* settings */
TCHAR				configPath[MAX_PATH];
TCHAR				iniFilePath[MAX_PATH];
tPluginProp			pluginProp;


/* main function of dll */
BOOL APIENTRY DllMain( HANDLE hModule, DWORD reasonForCall, LPVOID lpReserved )
{
	g_hModule = hModule;

	switch (reasonForCall)
	{
		case DLL_PROCESS_ATTACH:
		{
            //IndentHandler *indentHandler = new IndentHandler("E:/EigeneDateien/Dokumente/Informatik/UniversalIndentGUI/indenters", "E:/EigeneDateien/Dokumente/Informatik/UniversalIndentGUI/config", "E:/EigeneDateien/Dokumente/Informatik/UniversalIndentGUI/temp", 0);
            //indentHandler->show();
            //app.exec();
            if (!qApp) {
                int argc = 1;
                char *argv[] = {"setup", NULL};
                static QApplication qapp(argc, argv);
                indentHandler = new IndentHandler(0);
                indentHandler->setWindowModality( Qt::ApplicationModal );
                indentHandler->setWindowTitle("UniversalIndentGUI");
                indentHandler->setWindowIcon(QIcon(QString::fromUtf8(":/mainWindow/icon2.png")));
                indentHandler->setParameterChangedCallback( indentText );
                //qapp.setActiveWindow(indentHandler);
            }

			/* Set function pointers */
			funcItem[0]._pFunc = toggleView;
			funcItem[1]._pFunc = aboutDlg;
            funcItem[2]._pFunc = indentText;
		    	
			/* Fill menu names */
			strcpy(funcItem[0]._itemName, "&Show Dockable View");
            strcpy(funcItem[1]._itemName, "&About...");
            strcpy(funcItem[2]._itemName, "&Indent text");

			/* Set shortcuts */
			funcItem[0]._pShKey = new ShortcutKey;
			funcItem[0]._pShKey->_isAlt		= true;
			funcItem[0]._pShKey->_isCtrl	= true;
			funcItem[0]._pShKey->_isShift	= true;
			funcItem[0]._pShKey->_key		= 'T';
            funcItem[1]._pShKey = NULL;
            funcItem[2]._pShKey = NULL;
			break;
		}	
		case DLL_PROCESS_DETACH:
		{
			delete funcItem[0]._pShKey;
            delete indentHandler;

			/* save settings */
			saveSettings();
			break;
		}
		case DLL_THREAD_ATTACH:
			break;
			
		case DLL_THREAD_DETACH:
			break;
	}

	return TRUE;
}


extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData)
{
	/* stores notepad data */
	nppData = notpadPlusData;

	/* load data of plugin */
	loadSettings();

	/* initial dialogs */
	universalIndentGUI_NPPDialog.init((HINSTANCE)g_hModule, nppData, &pluginProp);
	//nppAboutDialog.init((HINSTANCE)g_hModule, nppData);
}


extern "C" __declspec(dllexport) LPCSTR getName()
{
	return PLUGIN_NAME;
}


extern "C" __declspec(dllexport) FuncItem * getFuncsArray(INT *nbF)
{
	*nbF = nbFunc;
	return funcItem;
}


/***
 *	beNotification()
 *
 *	This function is called, if a notification in Scintilla/Notepad++ occurs
 */
extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode)
{
	if (notifyCode->nmhdr.hwndFrom == nppData._nppHandle)
	{
		/* on this notification code you can register your plugin icon in Notepad++ toolbar */
		if (notifyCode->nmhdr.code == NPPN_TBMODIFICATION)
		{
			g_TBWndMgr.hToolbarBmp = (HBITMAP)::LoadImage((HINSTANCE)g_hModule, MAKEINTRESOURCE(IDB_TOOLBAR), IMAGE_BITMAP, 0, 0, (LR_LOADMAP3DCOLORS));
			::SendMessage(nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)funcItem[TOGGLE_DOCKABLE_WINDOW_INDEX]._cmdID, (LPARAM)&g_TBWndMgr);
		}
	}
}


/***
 *	messageProc()
 *
 *	This function is called, if a notification from Notepad occurs
 */
extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}


/***
 *	loadSettings()
 *
 *	Load the parameters of plugin
 */
void loadSettings(void)
{
	/* initialize the config directory */
	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)configPath);

	/* Test if config path exist */
	if (PathFileExists(configPath) == FALSE) {
		::CreateDirectory(configPath, NULL);
	}

	strcpy(iniFilePath, configPath);
	strcat(iniFilePath, PLUGINTEMP_INI);
	if (PathFileExists(iniFilePath) == FALSE)
	{
		::CloseHandle(::CreateFile(iniFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));
	}

	pluginProp.iValue1	= ::GetPrivateProfileInt(dlgTemp, Value1, 200, iniFilePath);
	pluginProp.iValue2	= ::GetPrivateProfileInt(dlgTemp, Value2, 200, iniFilePath);
}


/***
 *	saveSettings()
 *
 *	Saves the parameters of plugin
 */
void saveSettings(void)
{
	TCHAR	temp[16];

	::WritePrivateProfileString(dlgTemp, Value1, _itoa(pluginProp.iValue1, temp, 10), iniFilePath);
	::WritePrivateProfileString(dlgTemp, Value2, _itoa(pluginProp.iValue2, temp, 10), iniFilePath);
}


/**************************************************************************
 *	Interface functions
 */
void toggleView(void)
{
	/* get menu and test if dockable dialog is open */
	HMENU	hMenu = ::GetMenu(nppData._nppHandle);
	UINT state = ::GetMenuState(hMenu, funcItem[TOGGLE_DOCKABLE_WINDOW_INDEX]._cmdID, MF_BYCOMMAND);
	universalIndentGUI_NPPDialog.doDialog(state & MF_CHECKED ? false : true);
}


void aboutDlg(void)
{
    HMENU	hMenu = ::GetMenu(nppData._nppHandle);
    UINT state = ::GetMenuState(hMenu, funcItem[1]._cmdID, MF_BYCOMMAND);

    if ( state & MF_CHECKED ) {
        indentHandler->hide();
        state = ::CheckMenuItem(hMenu, funcItem[1]._cmdID, MF_BYCOMMAND | MF_UNCHECKED);
    }
    else {
        indentHandler->show();
        state = ::CheckMenuItem(hMenu, funcItem[1]._cmdID, MF_BYCOMMAND | MF_CHECKED);
    }

}


HWND getCurrentHScintilla(int which)
{
    return (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
};


void indentText() {
    int currentEdit = 0;
    int textLength = 0;
    char *fullEditorText = NULL;

    // Get scintilla text edit window handle
    //::SendMessage(nppData._nppHandle, WM_GETCURRENTSCINTILLA, 0, (LPARAM)&currentEdit);

    // Get the length of the selected text plus a 0 byte ending.
    textLength = ::SendMessage(getCurrentHScintilla(currentEdit), SCI_GETSELTEXT, 0, 0);

    // No text has been selected to format the whole text.
    if ( textLength - 1 == 0 ) {

        textLength = ::SendMessage(getCurrentHScintilla(currentEdit), SCI_GETTEXTLENGTH, 0, 0);
        // Because we need space for a trailing 0 byte increment the length.
        textLength++;

        fullEditorText = new char[textLength];

        // Get whole text
        ::SendMessage(getCurrentHScintilla(currentEdit), SCI_GETTEXT, textLength, (LPARAM)fullEditorText);

        QString indentedText = indentHandler->callIndenter(fullEditorText, "cpp");

        QByteArray indentedTextByteArray = indentedText.toAscii();

        // Set whole text
        ::SendMessage(getCurrentHScintilla(currentEdit), SCI_SETTEXT, 0, (LPARAM)indentedTextByteArray.constData());
    }
    // Format only the selected text.
    else {
        fullEditorText = new char[textLength];

        // Get selection start.
        int selectionStartPos = ::SendMessage(getCurrentHScintilla(currentEdit), SCI_GETSELECTIONSTART, 0, 0);

        // Get the selected text.
        ::SendMessage(getCurrentHScintilla(currentEdit), SCI_GETSELTEXT, 0, (LPARAM)fullEditorText);

        QString indentedText = indentHandler->callIndenter(fullEditorText, "cpp");

        QByteArray indentedTextByteArray = indentedText.toAscii();

        // Replace selected text.
        ::SendMessage(getCurrentHScintilla(currentEdit), SCI_REPLACESEL, 0, (LPARAM)indentedTextByteArray.constData());

        // Set selection again.
        ::SendMessage(getCurrentHScintilla(currentEdit), SCI_SETSEL, selectionStartPos, selectionStartPos + indentedTextByteArray.length() );
    }

    delete fullEditorText;
    fullEditorText = NULL;
}
