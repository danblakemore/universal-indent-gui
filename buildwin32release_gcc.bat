@echo off

echo Making some environment settings
echo --------------------------------
rem set QTDIR=%QTDIR%_static
set QTDIR=C:\Programmierung\qt.4.3.0_gcc_static
set PATH=%QTDIR%\bin
set PATH=%PATH%;C:\Programmierung\MingW\bin;D:\Programme\Tools\7-Zip
set PATH=%PATH%;%SystemRoot%\System32
set QMAKESPEC=win32-g++
echo Done.
echo.

echo Generating the translation binaries
echo -----------------------------------
for %%A in ( de, zh_TW, ja_JP) do (
    lrelease .\translations\universalindent_%%A.ts -qm .\translations\universalindent_%%A.qm -silent
    IF ERRORLEVEL 1 goto ERROR
)
echo Done.
echo.

echo Calling qmake
echo -------------
qmake
IF ERRORLEVEL 1 goto ERROR
echo Done.
echo.

echo Calling make release
echo --------------------
make release >NUL
IF ERRORLEVEL 1 goto ERROR
echo Done.
echo.

echo Delete old release dir and create new one
echo -----------------------------------------
rmdir UniversalIndentGUI_win32 /S /Q
IF ERRORLEVEL 1 goto ERROR
md UniversalIndentGUI_win32
IF ERRORLEVEL 1 goto ERROR
cd UniversalIndentGUI_win32
IF ERRORLEVEL 1 goto ERROR
md translations
IF ERRORLEVEL 1 goto ERROR
md data
IF ERRORLEVEL 1 goto ERROR
md doc
IF ERRORLEVEL 1 goto ERROR
cd ..
echo Done.
echo.

echo Copying the indenter executables and example file to the release data dir
echo -------------------------------------------------------------------------
FOR %%A IN ( astyle.exe, astyle.html, bcpp.exe, bcpp.txt, csstidy.exe, gc.exe, gc.txt, htmltidy.exe, htmltidy.html, indent.exe, libiconv-2.dll, libintl-2.dll, indent.html, uncrustify.exe, uncrustify.txt, example.cpp ) DO (
    if not exist .\data\%%A (
        echo File .\data\%%A not found!
        goto ERROR
    )
    copy .\data\%%A .\UniversalIndentGUI_win32\data\ >NUL
    IF ERRORLEVEL 1 goto ERROR
)
echo Done.
echo.

echo Copying the indenter uigui ini files to the release data dir
echo ------------------------------------------------------------
FOR %%A IN ( uigui_astyle.ini, uigui_bcpp.ini, uigui_csstidy.ini, uigui_gnuindent.ini, uigui_greatcode.ini, uigui_htmltidy.ini, uigui_phpCB.ini, uigui_uncrustify.ini, highlighter.ini ) DO (
    if not exist .\data\%%A (
        echo File .\data\%%A not found!
        goto ERROR
    )
    copy .\data\%%A .\UniversalIndentGUI_win32\data\ >NUL
    IF ERRORLEVEL 1 goto ERROR
)
echo Done.
echo.

echo Copying some other files (README, CHANGELOG etc)
echo ------------------------------------------------
FOR %%A IN ( CHANGELOG.txt, LICENSE.GPL, INSTALL.txt, README.txt ) DO (
    if not exist .\%%A (
        echo File .\data\%%A not found!
        goto ERROR
    )
    copy .\%%A .\UniversalIndentGUI_win32\ >NUL
    IF ERRORLEVEL 1 goto ERROR
)
echo Done.
echo.

echo Copying the translation files to the release translation dir
echo ------------------------------------------------------------
copy %QTDIR%\translations\qt_de.qm .\translations\ >NUL
copy %QTDIR%\translations\qt_ja_jp.qm .\translations\qt_ja_JP.qm >NUL
copy .\translations\qt_de.qm .\UniversalIndentGUI_win32\translations\ >NUL
copy .\translations\qt_ja_JP.qm .\UniversalIndentGUI_win32\translations\ >NUL
for %%A in ( de, zh_TW, ja_JP ) do (
    if not exist .\translations\universalindent_%%A.qm (
        echo File .\translations\universalindent_%%A.qm not found!
        goto ERROR
    )
    copy .\translations\universalindent_%%A.qm .\UniversalIndentGUI_win32\translations\ >NUL
    IF ERRORLEVEL 1 goto ERROR
)
echo Done.
echo.

echo Copying doc and UniversalIndentGUI.exe to release dir
echo -----------------------------------------------------
copy .\doc\iniFileFormat.html .\UniversalIndentGUI_win32\doc\ >NUL
IF ERRORLEVEL 1 goto ERROR
copy .\release\UniversalIndentGUI.exe .\UniversalIndentGUI_win32\ >NUL
IF ERRORLEVEL 1 goto ERROR
echo Done.
echo.

echo Packing the whole release dir content
echo -------------------------------------
cd UniversalIndentGUI_win32
7z.exe a -tzip UniversalIndentGUI_0.6.1_Beta_win32.zip >NUL
IF ERRORLEVEL 1 goto ERROR
cd ..
echo Done.
echo.


:SUCCESS
echo Everything completed successfull!
goto END

:ERROR
echo.
echo Something during the above step went wrong! See output error message, if any.
goto END

:END
pause
