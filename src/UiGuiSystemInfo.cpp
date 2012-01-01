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

#include "UiGuiSystemInfo.h"

#include <QString>
#include <QSysInfo>
#include <QProcess>
#include <QFile>

UiGuiSystemInfo::UiGuiSystemInfo() {
}


/*!
    \brief Returns the operating system UiGUI is currently running on as one string.

    The String contains name and version of the os. E.g. Linux Ubuntu 9.04.
 */
QString UiGuiSystemInfo::getOperatingSystem() {
    QString operatingSystemString = "";

#if defined(Q_WS_WIN)
    switch ( QSysInfo::WindowsVersion ) {
        case QSysInfo::WV_32s :
            operatingSystemString = "Windows 3.1 with Win 32s";
            break;
        case QSysInfo::WV_95 :
            operatingSystemString = "Windows 95";
            break;
        case QSysInfo::WV_98 :
            operatingSystemString = "Windows 98";
            break;
        case QSysInfo::WV_Me :
            operatingSystemString = "Windows Me";
            break;
        case QSysInfo::WV_NT :
            operatingSystemString = "Windows NT (operating system version 4.0)";
            break;
        case QSysInfo::WV_2000 :
            operatingSystemString = "Windows 2000 (operating system version 5.0)";
            break;
        case QSysInfo::WV_XP :
            operatingSystemString = "Windows XP (operating system version 5.1)";
            break;
        case QSysInfo::WV_2003 :
            operatingSystemString = "Windows Server 2003, Windows Server 2003 R2, Windows Home Server, Windows XP Professional x64 Edition (operating system version 5.2)";
            break;
        case QSysInfo::WV_VISTA :
            operatingSystemString = "Windows Vista, Windows Server 2008 (operating system version 6.0)";
            break;
        case QSysInfo::WV_WINDOWS7 :
            operatingSystemString = "Windows 7 (operating system version 6.1)";
            break;
        case QSysInfo::WV_CE :
            operatingSystemString = "Windows CE";
            break;
        case QSysInfo::WV_CENET :
            operatingSystemString = "Windows CE .NET";
            break;
        case QSysInfo::WV_CE_5 :
            operatingSystemString = "Windows CE 5.x";
            break;
        case QSysInfo::WV_CE_6 :
            operatingSystemString = "Windows CE 6.x";
            break;
        default :
            operatingSystemString = "Unknown Windows operating system.";
            break;
    }
#elif defined(Q_WS_MAC)
    switch ( QSysInfo::MacintoshVersion ) {
        case QSysInfo::MV_9 :
            operatingSystemString = "Mac OS 9 (unsupported)";
            break;
        case QSysInfo::MV_10_0 :
            operatingSystemString = "Mac OS X 10.0 Cheetah (unsupported)";
            break;
        case QSysInfo::MV_10_1 :
            operatingSystemString = "Mac OS X 10.1 Puma (unsupported)";
            break;
        case QSysInfo::MV_10_2 :
            operatingSystemString = "Mac OS X 10.2 Jaguar (unsupported)";
            break;
        case QSysInfo::MV_10_3 :
            operatingSystemString = "Mac OS X 10.3 Panther";
            break;
        case QSysInfo::MV_10_4 :
            operatingSystemString = "Mac OS X 10.4 Tiger";
            break;
        case QSysInfo::MV_10_5 :
            operatingSystemString = "Mac OS X 10.5 Leopard";
            break;
        case QSysInfo::MV_10_6 :
            operatingSystemString = "Mac OS X 10.6 Snow Leopard";
            break;
        case QSysInfo::MV_Unknown :
            operatingSystemString = "An unknown and currently unsupported platform";
            break;
        default :
            operatingSystemString = "Unknown Mac operating system.";
            break;
    }
#else
    //TODO: Detect Unix, Linux etc. distro as described on http://www.novell.com/coolsolutions/feature/11251.html
    operatingSystemString = "Linux";
    QProcess process;

    process.start("uname -s");
    bool result = process.waitForFinished(1000);
    QString os = process.readAllStandardOutput().trimmed();

    process.start("uname -r");
    result = process.waitForFinished(1000);
    QString rev = process.readAllStandardOutput().trimmed();

    process.start("uname -m");
    result = process.waitForFinished(1000);
    QString mach = process.readAllStandardOutput().trimmed();

    if ( os == "SunOS" ) {
        os = "Solaris";

        process.start("uname -p");
        result = process.waitForFinished(1000);
        QString arch = process.readAllStandardOutput().trimmed();

        process.start("uname -v");
        result = process.waitForFinished(1000);
        QString timestamp = process.readAllStandardOutput().trimmed();

        operatingSystemString = os + " " + rev + " (" + arch + " " + timestamp + ")";
    }
    else if ( os == "AIX" ) {
        process.start("oslevel -r");
        result = process.waitForFinished(1000);
        QString oslevel = process.readAllStandardOutput().trimmed();

        operatingSystemString = os + "oslevel " + oslevel;
    }
    else if ( os == "Linux" ) {
        QString dist;
        QString pseudoname;
        QString kernel = rev;

        if ( QFile::exists("/etc/redhat-release") ) {
            dist = "RedHat";

            process.start("sh -c \"cat /etc/redhat-release | sed s/.*\\(// | sed s/\\)//\"");
            result = process.waitForFinished(1000);
            pseudoname = process.readAllStandardOutput().trimmed();

            process.start("sh -c \"cat /etc/redhat-release | sed s/.*release\\ // | sed s/\\ .*//\"");
            result = process.waitForFinished(1000);
            rev = process.readAllStandardOutput().trimmed();
        }
        else if ( QFile::exists("/etc/SUSE-release") ) {
            process.start("sh -c \"cat /etc/SUSE-release | tr '\\n' ' '| sed s/VERSION.*//\"");
            result = process.waitForFinished(1000);
            dist = process.readAllStandardOutput().trimmed();

            process.start("sh -c \"cat /etc/SUSE-release | tr '\\n' ' ' | sed s/.*=\\ //\"");
            result = process.waitForFinished(1000);
            rev = process.readAllStandardOutput().trimmed();
        }
        else if ( QFile::exists("/etc/mandrake-release") ) {
            dist = "Mandrake";

            process.start("sh -c \"cat /etc/mandrake-release | sed s/.*\\(// | sed s/\\)//\"");
            result = process.waitForFinished(1000);
            pseudoname = process.readAllStandardOutput().trimmed();

            process.start("sh -c \"cat /etc/mandrake-release | sed s/.*release\\ // | sed s/\\ .*//\"");
            result = process.waitForFinished(1000);
            rev = process.readAllStandardOutput().trimmed();
        }
        else if ( QFile::exists("/etc/lsb-release") ) {
            dist = "Ubuntu";

            QString processCall = "sh -c \"cat /etc/lsb-release | tr '\\n' ' ' | sed s/.*DISTRIB_RELEASE=// | sed s/\\ .*//\"";
            process.start( processCall );
            result = process.waitForFinished(1000);
            rev = process.readAllStandardOutput().trimmed();
            QString errorStr = process.readAllStandardError();

            process.start("sh -c \"cat /etc/lsb-release | tr '\\n' ' ' | sed s/.*DISTRIB_CODENAME=// | sed s/\\ .*//\"");
            result = process.waitForFinished(1000);
            pseudoname = process.readAllStandardOutput().trimmed();
        }
        else if ( QFile::exists("/etc/debian_version") ) {
            dist = "Debian";

            process.start("cat /etc/debian_version");
            result = process.waitForFinished(1000);
            dist += process.readAllStandardOutput().trimmed();

            rev = "";
        }

        if ( QFile::exists("/etc/UnitedLinux-release") ) {
            process.start("sh -c \"cat /etc/UnitedLinux-release | tr '\\n' ' ' | sed s/VERSION.*//\"");
            result = process.waitForFinished(1000);
            dist += process.readAllStandardOutput().trimmed();
        }

        operatingSystemString = os + " " + dist + " " + rev + " (" + pseudoname + " " + kernel + " " + mach + ")";
    }
#endif

    return operatingSystemString;
}
