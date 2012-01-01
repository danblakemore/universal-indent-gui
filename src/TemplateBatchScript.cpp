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

#include "TemplateBatchScript.h"

// Need to include QObject here so that platform specific defines like Q_OS_WIN32 are set.
#include <QObject>

/*!
    \brief The only and static function of this class returns a batch or shell script
    as string that can be used to call an indenter with the current settings from
    the command line.

    The returned string contains some placeholders where the indenter name needs to
    be filled in. The placeholders are "__INDENTERCALLSTRING1__" that should be replaced
    by the indenter call string that indents a complete directory.
    "__INDENTERCALLSTRING2__" the call string for indenting only one file.
    And "__INDENTERCALLSTRINGSCRIPTNAME__" which is only the shown name of the indenter.
 */
const char* TemplateBatchScript::getTemplateBatchScript() {
    static const char* templateBatchScript =
#if defined(Q_OS_WIN32)
        "@echo off\n"
        "\n"
        "IF (%1)==() GOTO error\n"
        "dir /b /ad %1 >nul 2>nul && GOTO indentDir\n"
        "IF NOT EXIST %1 GOTO error\n"
        "goto indentFile\n"
        "\n"
        ":indentDir\n"
        "set searchdir=%1\n"
        "\n"
        "IF (%2)==() GOTO assignDefaultSuffix\n"
        "set filesuffix=%2\n"
        "\n"
        "GOTO run\n"
        "\n"
        ":assignDefaultSuffix\n"
        "::echo !!!!DEFAULT SUFFIX!!!\n"
        "set filesuffix=*\n"
        "\n"
        ":run\n"
        "FOR /F \"tokens=*\" %%G IN ('DIR /B /S %searchdir%\\*.%filesuffix%') DO (\n"
        "echo Indenting file \"%%G\"\n"
        "__INDENTERCALLSTRING1__\n"
        ")\n"
        "GOTO ende\n"
        "\n"
        ":indentFile\n"
        "echo Indenting one file %1\n"
        "__INDENTERCALLSTRING2__\n"
        "\n"
        "GOTO ende\n"
        "\n"
        ":error\n"
        "echo .\n"
        "echo ERROR: As parameter given directory or file does not exist!\n"
        "echo Syntax is: __INDENTERCALLSTRINGSCRIPTNAME__ dirname filesuffix\n"
        "echo Syntax is: __INDENTERCALLSTRINGSCRIPTNAME__ filename\n"
        "echo Example: __INDENTERCALLSTRINGSCRIPTNAME__ temp cpp\n"
        "echo .\n"
        "\n"
        ":ende\n";

#else

        "#!/bin/sh \n"
        "\n"
        "if [ ! -n \"$1\" ]; then\n"
        "echo \"Syntax is: recurse.sh dirname filesuffix\"\n"
        "echo \"Syntax is: recurse.sh filename\"\n"
        "echo \"Example: recurse.sh temp cpp\"\n"
        "exit 1\n"
        "fi\n"
        "\n"
        "if [ -d \"$1\" ]; then\n"
        "#echo \"Dir ${1} exists\"\n"
        "if [ -n \"$2\" ]; then\n"
        "filesuffix=$2\n"
        "else\n"
        "filesuffix=\"*\"\n"
        "fi\n"
        "\n"
        "#echo \"Filtering files using suffix ${filesuffix}\"\n"
        "\n"
        "file_list=`find ${1} -name \"*.${filesuffix}\" -type f`\n"
        "for file2indent in $file_list\n"
        "do \n"
        "echo \"Indenting file $file2indent\"\n"
        "__INDENTERCALLSTRING1__\n"
        "done\n"
        "else\n"
        "if [ -f \"$1\" ]; then\n"
        "echo \"Indenting one file $1\"\n"
        "__INDENTERCALLSTRING2__\n"
        "else\n"
        "echo \"ERROR: As parameter given directory or file does not exist!\"\n"
        "echo \"Syntax is: __INDENTERCALLSTRINGSCRIPTNAME__ dirname filesuffix\"\n"
        "echo \"Syntax is: __INDENTERCALLSTRINGSCRIPTNAME__ filename\"\n"
        "echo \"Example: __INDENTERCALLSTRINGSCRIPTNAME__ temp cpp\"\n"
        "exit 1\n"
        "fi\n"
        "fi\n";
#endif // #if defined(Q_OS_WIN32)
    return templateBatchScript;
}


/* Here comes the original batch script without the c++ markup
@echo off

IF (%1)==() GOTO error
dir /b /ad %1 >nul 2>nul && GOTO indentDir
IF NOT EXIST %1 GOTO error
goto indentFile

:indentDir
set searchdir=%1

IF (%2)==() GOTO assignDefaultSuffix
set filesuffix=%2

GOTO run

:assignDefaultSuffix
::echo !!!!DEFAULT SUFFIX!!!
set filesuffix=*

:run
FOR /F "tokens=*" %%G IN ('DIR /B /S %searchdir%\*.%filesuffix%') DO (
    echo Indenting file "%%G"
    ::call call_CSSTidy.bat "%%G"
    "C:/Dokumente und Einstellungen/ts/Eigene Dateien/Visual Studio 2005/Projects/UiGuixy/indenters/csstidy.exe" "%%G" --timestamp=true --allow_html_in_templates=false --compress_colors=true --compress_font=true --lowercase_s=false --preserve_css=false --remove_last_;=false --remove_bslash=true --sort_properties=false --sort_selectors=false  indentoutput.css
    move /Y indentoutput.css "%%G"
)
GOTO ende

:indentFile
echo Indenting one file %1
"C:/Dokumente und Einstellungen/ts/Eigene Dateien/Visual Studio 2005/Projects/UiGuixy/indenters/csstidy.exe" %1 --timestamp=true --allow_html_in_templates=false --compress_colors=true --compress_font=true --lowercase_s=false --preserve_css=false --remove_last_;=false --remove_bslash=true --sort_properties=false --sort_selectors=false  indentoutput.css
move /Y indentoutput.css %1

GOTO ende

:error
echo .
echo ERROR: As parameter given directory or file does not exist!
echo Syntax is: recurse.bat dirname filesuffix
echo Syntax is: recurse.bat filename
echo Example: recurse.bat temp cpp
echo .

:ende

*/
