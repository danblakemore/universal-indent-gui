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


#ifndef UNIVERSALINDENTGUI_NPP_H
#define UNIVERSALINDENTGUI_NPP_H

#define WIN32_LEAN_AND_MEAN
#include "PluginInterface.h"
#include "Scintilla.h"
#include <tchar.h>
#include <shlwapi.h>

#include "IndentHandler.h"
#include "SettingsPaths.h"

/* menu position in funcItem */
#define TOGGLE_SHOW_UIGUI_INDEX     0
#define EXECUTE_TEXT_INDENT_INDEX   1
#define TOGGLE_AUTO_UPDATE_INDEX    2


/* ini file name */
CONST TCHAR PLUGINTEMP_INI[]    = _T("\\UniversalIndentGUI.ini");

/* param names of ini file */
/* section */
CONST TCHAR dlgTemp[]           = _T("UniversalIndentGUI");
/* keys of section */
CONST TCHAR Value1[]            = _T("Value1");
CONST TCHAR Value2[]            = _T("Value2");


/* define here your plugin properties (stored in an ini file) */
typedef struct tPluginProp {
    INT     iValue1;
    INT     iValue2;
} tPluginProp;


/* load and save properties from/into ini file */
void loadSettings(void);
void saveSettings(void);

/* menu functions */
void showUiGUI(void);
void toggleAutoUpdate(void);
void indentText(void);



#endif  // UNIVERSALINDENTGUI_NPP_H

