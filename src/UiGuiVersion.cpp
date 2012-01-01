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

#include "UiGuiVersion.h"

#include <QString>
#include <QStringList>
#include <QDate>

namespace UiGuiVersion {
/*!
    \brief Returns the build date as a localized string, e.g. "9. Februar 2009". If
    there was some kind of error, the returned string is empty.
 */
QString getBuildDate() {
    QStringList monthNames;
    QString buildDateString = "";

    monthNames << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" << "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dez";
    QStringList buildDateStringList = QString(__DATE__).simplified().split(' '); // __DATE__ returns eg "Feb  4 2009"

    if ( buildDateStringList.count() == 3 ) {
        QDate buildDate(buildDateStringList.last().toInt(), monthNames.indexOf( buildDateStringList.first() )+1, buildDateStringList.at(1).toInt());
        buildDateString = buildDate.toString("d. MMMM yyyy");
    }

    return buildDateString;
}


/*!
    \brief Returns the revision number, that the current build is based on, as string. If
    there was some kind of error, the returned string is empty.
 */
QString getBuildRevision() {
    QString buildRevision = "";
    QStringList buildRevisionStringList = QString(PROGRAM_REVISION).simplified().split(' ');

    if ( buildRevisionStringList.count() == 3 ) {
        buildRevision = buildRevisionStringList.at(1);  // PROGRAM_REVISION is eg "$Revision: 907 $"
    }

    return buildRevision;
}
}  // namespace UiGuiVersion
