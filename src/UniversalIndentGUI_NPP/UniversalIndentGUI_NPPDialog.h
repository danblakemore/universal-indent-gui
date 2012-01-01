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



#ifndef UNIVERSALINDENTGUI_NPPDIALOG_H
#define UNIVERSALINDENTGUI_NPPDIALOG_H

#include "UniversalIndentGUI_NPP.h"
#include "DockingDlgInterface.h"
#include "Toolbar.h"

#include "indenthandler.h"

class UniversalIndentGUI_NPPDialog : public DockingDlgInterface
{
public:
    UniversalIndentGUI_NPPDialog(void);
    ~UniversalIndentGUI_NPPDialog(void);

    void init(HINSTANCE hInst, NppData nppData, tPluginProp *pMgrProp);

    void destroy(void) {
    };

    void doDialog(bool willBeShown = true);

protected:

    /* get toolbar tooltips */
    void GetNameStrFromCmd(UINT resID, LPTSTR tip);

    virtual BOOL CALLBACK run_dlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    /* initial dialog here */
    void InitialDialog(void);

    /* toolbar commands */
    void tb_cmd(UINT message);

private:
    /* Handles */
    NppData                 _nppData;
    HWND                    _hEdit;

    /* classes */
    ToolBar                 _ToolBar;
    ReBar                   _Rebar;

    /* settings */
    tTbData                 _data;
    tPluginProp*            _pPluginProp;

    IndentHandler *indentHandler;
};


#endif // UNIVERSALINDENTGUI_NPPDIALOG_H

