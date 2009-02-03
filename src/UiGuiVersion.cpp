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

#include "UiGuiVersion.h"

#include "stdlib.h"
#include "string.h"

const int programVersionDate()
{
    static int version = 0;

    if (0 == version) {
        union timeUnion {
            struct {
                char Mmm[3];
                char space1;
                char dd[2];
                char space2;
                char yyyy[4];
                char terminator;
            };
            char timeString[12];
        };
        timeUnion timeVar;
        //get date from system
        const char * dateString = __DATE__;
        //copy to internal
        strncpy((char *)&timeVar.timeString, dateString, 11);
        //terminate string
        timeVar.timeString[11] = 0;
        //swap characters
        char convertString[9];
        strncpy((char *)&convertString, (char *)&timeVar.yyyy, 4);
        strncpy((char *)&convertString[6], (char *)&timeVar.dd, 2);
        //convert month
        const char * month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dez", NULL};
        const char * monthNumbers[] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "00"};
        //find string in table
        for (int i = 0; month[i] != NULL; i++) {
            if (strncmp(month[i], (char *)&timeVar.Mmm, 2) == 0) {
                strncpy((char *)&convertString[4], monthNumbers[i], 2);
                break;
            }
        }
        //terminate string
        convertString[8] = 0;
        version = atoi((char *)&convertString);
    }

    return version;
    //return PROGRAM_DATE; //ORIGINAL 24.11.2008!!!
}
