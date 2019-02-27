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


/* include files */
#include "UniversalIndentGUI_NPP.h"

#include "UiGuiLogger.h"
#include "UiGuiSystemInfo.h"
#include "UiGuiVersion.h"

/* information for notepad */
CONST INT   nbFunc  = 3;
CONST TCHAR  PLUGIN_NAME[] = _T("&UniversalIndentGUI");

/* global values */
HANDLE              g_hModule           = NULL;
NppData             nppData;
FuncItem            funcItem[nbFunc];

IndentHandler *indentHandler;

/* settings */
TCHAR               configPath[MAX_PATH];
TCHAR               iniFilePath[MAX_PATH];
tPluginProp         pluginProp;


/* main function of dll */
BOOL APIENTRY DllMain( HANDLE hModule, DWORD reasonForCall, LPVOID lpReserved ) {
    g_hModule = hModule;

    switch (reasonForCall)
    {
        case DLL_PROCESS_ATTACH :
        {
            if (!qApp) {
                int argc = 1;
                char *argv[] = {"setup", NULL};
                static QApplication qapp(argc, argv);
                indentHandler = new IndentHandler(0);
                indentHandler->setWindowModality( Qt::ApplicationModal );
                indentHandler->setWindowTitle("UniversalIndentGUI");
                indentHandler->setWindowIcon(QIcon(QString::fromUtf8(":/mainWindow/icon2.png")));
                indentHandler->setParameterChangedCallback( NULL );
                indentHandler->setWindowClosedCallback( showUiGUI );

                // Setting UTF-8 as default 8-Bit encoding to ensure that qDebug does no false string conversion.
                QTextCodec::setCodecForCStrings( QTextCodec::codecForName("UTF-8") );
                QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") );
                // Force creation of an UiGuiLogger instance here, to avoid recursion with SettingsPaths init function.
                UiGuiLogger::getInstance();
                qInstallMsgHandler( UiGuiLogger::messageHandler );
                UiGuiLogger::messageHandler( UiGuiInfoMsg, QString("Starting UiGUI Version %1 %2").arg(PROGRAM_VERSION_STRING).arg(PROGRAM_REVISION).toLatin1() );
                UiGuiLogger::messageHandler( UiGuiInfoMsg, QString("Running on %1").arg(UiGuiSystemInfo::getOperatingSystem()).toLatin1() );

#ifdef _DEBUG
                UiGuiLogger::getInstance()->setVerboseLevel(0);
#endif
            }

            /* Set function pointers */
            funcItem[TOGGLE_SHOW_UIGUI_INDEX]._pFunc = showUiGUI;
            funcItem[EXECUTE_TEXT_INDENT_INDEX]._pFunc = indentText;
            funcItem[TOGGLE_AUTO_UPDATE_INDEX]._pFunc = toggleAutoUpdate;

            /* Fill menu names */
#ifdef UNICODE
            wcscpy(funcItem[TOGGLE_SHOW_UIGUI_INDEX]._itemName, _T("&Show Parameter Settings"));
            wcscpy(funcItem[EXECUTE_TEXT_INDENT_INDEX]._itemName, _T("&Indent text"));
            wcscpy(funcItem[TOGGLE_AUTO_UPDATE_INDEX]._itemName, _T("&Enable Text Auto Update"));
#else
            strcpy(funcItem[TOGGLE_SHOW_UIGUI_INDEX]._itemName, "&Show Parameter Settings");
            strcpy(funcItem[EXECUTE_TEXT_INDENT_INDEX]._itemName, "&Indent text");
            strcpy(funcItem[TOGGLE_AUTO_UPDATE_INDEX]._itemName, "&Enable Text Auto Update");
#endif

            /* Set shortcuts */
            funcItem[TOGGLE_SHOW_UIGUI_INDEX]._pShKey = new ShortcutKey;
            funcItem[TOGGLE_SHOW_UIGUI_INDEX]._pShKey->_isAlt   = true;
            funcItem[TOGGLE_SHOW_UIGUI_INDEX]._pShKey->_isCtrl  = true;
            funcItem[TOGGLE_SHOW_UIGUI_INDEX]._pShKey->_isShift = true;
            funcItem[TOGGLE_SHOW_UIGUI_INDEX]._pShKey->_key     = 'T';
            funcItem[EXECUTE_TEXT_INDENT_INDEX]._pShKey = new ShortcutKey;
            funcItem[EXECUTE_TEXT_INDENT_INDEX]._pShKey->_isAlt   = true;
            funcItem[EXECUTE_TEXT_INDENT_INDEX]._pShKey->_isCtrl  = true;
            funcItem[EXECUTE_TEXT_INDENT_INDEX]._pShKey->_isShift = true;
            funcItem[EXECUTE_TEXT_INDENT_INDEX]._pShKey->_key     = 'J';
            funcItem[TOGGLE_AUTO_UPDATE_INDEX]._pShKey = NULL;
            break;
        }
        case DLL_PROCESS_DETACH :
        {
            delete funcItem[TOGGLE_SHOW_UIGUI_INDEX]._pShKey;
            delete indentHandler;

            /* save settings */
            saveSettings();
            break;
        }
        case DLL_THREAD_ATTACH :
            break;

        case DLL_THREAD_DETACH :
            break;
    }

    return TRUE;
}


extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData) {
    /* stores notepad data */
    nppData = notpadPlusData;

    /* load data of plugin */
    loadSettings();

    /* initial dialogs */
    //universalIndentGUI_NPPDialog.init((HINSTANCE)g_hModule, nppData, &pluginProp);
}


extern "C" __declspec(dllexport) const TCHAR * getName() {
    return PLUGIN_NAME;
}


extern "C" __declspec(dllexport) FuncItem * getFuncsArray(INT *nbF)
{
    *nbF = nbFunc;
    return funcItem;
}


#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode() {
    return true;
}
#endif //UNICODE


/***
*	beNotification()
*
*	This function is called, if a notification in Scintilla/Notepad++ occurs
*/
extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode) {
    if (notifyCode->nmhdr.hwndFrom == nppData._nppHandle) {
        /* on this notification code you can register your plugin icon in Notepad++ toolbar */
        if (notifyCode->nmhdr.code == NPPN_TBMODIFICATION) {
            //g_TBWndMgr.hToolbarBmp = (HBITMAP)::LoadImage((HINSTANCE)g_hModule, MAKEINTRESOURCE(IDB_TOOLBAR), IMAGE_BITMAP, 0, 0, (LR_LOADMAP3DCOLORS));
            //::SendMessage(nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)funcItem[TOGGLE_AUTO_UPDATE_INDEX]._cmdID, (LPARAM)&g_TBWndMgr);
        }
    }
}


/***
*	messageProc()
*
*	This function is called, if a notification from Notepad occurs
*/
extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam) {
    return TRUE;
}


/***
*	loadSettings()
*
*	Load the parameters of plugin
*/
void loadSettings(void) {
    /* initialize the config directory */
    ::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)configPath);

    /* Test if config path exist */
    if (PathFileExists(configPath) == FALSE) {
        ::CreateDirectory(configPath, NULL);
    }

#ifdef UNICODE
    wcscpy(iniFilePath, configPath);
    wcscat(iniFilePath, PLUGINTEMP_INI);
#else
    strcpy(iniFilePath, configPath);
    strcat(iniFilePath, PLUGINTEMP_INI);
#endif

    if (PathFileExists(iniFilePath) == FALSE) {
        ::CloseHandle(::CreateFile(iniFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));
    }

    pluginProp.iValue1  = ::GetPrivateProfileInt(dlgTemp, Value1, 200, iniFilePath);
    pluginProp.iValue2  = ::GetPrivateProfileInt(dlgTemp, Value2, 200, iniFilePath);
}


/***
*	saveSettings()
*
*	Saves the parameters of plugin
*/
void saveSettings(void) {
    TCHAR   temp[16];

#ifdef UNICODE
    ::WritePrivateProfileString(dlgTemp, Value1, _itow(pluginProp.iValue1, temp, 10), iniFilePath);
    ::WritePrivateProfileString(dlgTemp, Value2, _itow(pluginProp.iValue2, temp, 10), iniFilePath);
#else
    ::WritePrivateProfileString(dlgTemp, Value1, _itoa(pluginProp.iValue1, temp, 10), iniFilePath);
    ::WritePrivateProfileString(dlgTemp, Value2, _itoa(pluginProp.iValue2, temp, 10), iniFilePath);
#endif
}


/**************************************************************************
*	Interface functions
*/
void toggleAutoUpdate(void) {
    HMENU   hMenu = ::GetMenu(nppData._nppHandle);
    UINT state = ::GetMenuState(hMenu, funcItem[TOGGLE_AUTO_UPDATE_INDEX]._cmdID, MF_BYCOMMAND);

    if ( state & MF_CHECKED ) {
        indentHandler->setParameterChangedCallback( NULL );
        state = ::CheckMenuItem(hMenu, funcItem[TOGGLE_AUTO_UPDATE_INDEX]._cmdID, MF_BYCOMMAND | MF_UNCHECKED);
    }
    else {
        if ( !indentHandler->isVisible() ) {
            showUiGUI();
        }
        indentHandler->setParameterChangedCallback( indentText );
        state = ::CheckMenuItem(hMenu, funcItem[TOGGLE_AUTO_UPDATE_INDEX]._cmdID, MF_BYCOMMAND | MF_CHECKED);
    }

    indentText();
}


void showUiGUI(void) {
    HMENU   hMenu = ::GetMenu(nppData._nppHandle);
    UINT menuState = ::GetMenuState(hMenu, funcItem[TOGGLE_SHOW_UIGUI_INDEX]._cmdID, MF_BYCOMMAND);
    bool windowIsVisible = indentHandler->isVisible();

    if ( menuState & MF_CHECKED ) {
        if ( windowIsVisible ) {
            indentHandler->hide();
        }
        menuState = ::CheckMenuItem(hMenu, funcItem[TOGGLE_SHOW_UIGUI_INDEX]._cmdID, MF_BYCOMMAND | MF_UNCHECKED);
    }
    else {
        if ( !windowIsVisible ) {
            indentHandler->show();
        }
        menuState = ::CheckMenuItem(hMenu, funcItem[TOGGLE_SHOW_UIGUI_INDEX]._cmdID, MF_BYCOMMAND | MF_CHECKED);
    }
}


HWND getCurrentHScintilla(int which) {
    return (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
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

        // Get whole text.
        ::SendMessage(getCurrentHScintilla(currentEdit), SCI_GETTEXT, textLength, (LPARAM)fullEditorText);

        // Get the first visible line and add the max visible lines to it so later scrolling to the correct position is ensured.
        int firstLine = ::SendMessage(getCurrentHScintilla(currentEdit), SCI_GETFIRSTVISIBLELINE, 0, 0);
        firstLine += ::SendMessage(getCurrentHScintilla(currentEdit), SCI_LINESONSCREEN, 0, 0) - 1;

        QString indentedText = indentHandler->callIndenter(fullEditorText, "cpp");

        QByteArray indentedTextByteArray = indentedText.toLatin1();

        // Set whole text.
        ::SendMessage(getCurrentHScintilla(currentEdit), SCI_SETTEXT, 0, (LPARAM)indentedTextByteArray.constData());

        // Set first visible line again.
        ::SendMessage(getCurrentHScintilla(currentEdit), SCI_GOTOLINE, firstLine, 0);
    }
    // Format only the selected text.
    else {
        fullEditorText = new char[textLength];

        // Get selection start.
        int selectionStartPos = ::SendMessage(getCurrentHScintilla(currentEdit), SCI_GETSELECTIONSTART, 0, 0);

        // Get the selected text.
        ::SendMessage(getCurrentHScintilla(currentEdit), SCI_GETSELTEXT, 0, (LPARAM)fullEditorText);

        QString indentedText = indentHandler->callIndenter(fullEditorText, "cpp");

        QByteArray indentedTextByteArray = indentedText.toLatin1();

        // Replace selected text.
        ::SendMessage(getCurrentHScintilla(currentEdit), SCI_REPLACESEL, 0, (LPARAM)indentedTextByteArray.constData());

        // Set selection again.
        ::SendMessage(getCurrentHScintilla(currentEdit), SCI_SETSEL, selectionStartPos, selectionStartPos + indentedTextByteArray.length() );
    }

    delete [] fullEditorText;
    fullEditorText = NULL;
}
