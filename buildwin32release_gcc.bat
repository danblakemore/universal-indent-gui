@echo off

echo Making some environment settings
echo --------------------------------
rem set QTDIR=%QTDIR%_static
rem set QTDIR=C:\Programmierung\qt.4.3.0_gcc_static
set QTDIR=F:\Qt\qt.4.3.2_gpl_static
set PATH=%QTDIR%\bin
set PATH=%PATH%;D:\Programme\Informat\MingW\bin;D:\Programme\Tools\7-Zip
set PATH=%PATH%;%SystemRoot%\System32
set QMAKESPEC=win32-g++
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
md indenters
IF ERRORLEVEL 1 goto ERROR
md doc
IF ERRORLEVEL 1 goto ERROR
md config
IF ERRORLEVEL 1 goto ERROR
cd ..
echo Done.
echo.

echo Copying the indenter executables and example file to the release indenters dir
echo ------------------------------------------------------------------------------
FOR %%A IN ( astyle.exe, astyle.html, bcpp.exe, bcpp.txt, csstidy.exe, gc.exe, gc.txt, indent.exe, libiconv-2.dll, libintl-2.dll, indent.html, JsDecoder.js, perltidy, PerlTidyLib.pm, php_beautifier.html, shellindent.awk, tidy.exe, tidy.html, uncrustify.exe, uncrustify.txt, example.cpp, example.css, example.html, example.js, example.php, example.pl, example.sh ) DO (
    if not exist .\indenters\%%A (
        echo File .\indenters\%%A not found!
        goto ERROR
    )
    copy .\indenters\%%A .\UniversalIndentGUI_win32\indenters\ >NUL
    IF ERRORLEVEL 1 goto ERROR
)
echo Done.
echo.

echo Copying the indenter uigui ini files to the release indenters dir
echo -----------------------------------------------------------------
copy .\indenters\uigui_*.ini .\UniversalIndentGUI_win32\indenters\ >NUL
IF ERRORLEVEL 1 goto ERROR
echo Done.
echo.

echo Copying the default highlighter ini files to the release config dir
echo -------------------------------------------------------------------
FOR %%A IN ( UiGuiSyntaxHighlightConfig.ini ) DO (
    if not exist .\config\%%A (
        echo File .\config\%%A not found!
        goto ERROR
    )
    copy .\config\%%A .\UniversalIndentGUI_win32\config\ >NUL
    IF ERRORLEVEL 1 goto ERROR
)
echo Done.
echo.

echo Copying some other files (README, CHANGELOG etc)
echo ------------------------------------------------
FOR %%A IN ( CHANGELOG.txt, LICENSE.GPL, INSTALL.txt, README.txt ) DO (
    if not exist .\%%A (
        echo File .\%%A not found!
        goto ERROR
    )
    copy .\%%A .\UniversalIndentGUI_win32\ >NUL
    IF ERRORLEVEL 1 goto ERROR
)
echo Done.
echo.

echo Copying the translation files to the release translation dir
echo ------------------------------------------------------------
copy .\translations\*.qm .\UniversalIndentGUI_win32\translations\ >NUL
IF ERRORLEVEL 1 goto ERROR
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
7z.exe a -tzip UniversalIndentGUI_0.8.1_win32.zip >NUL
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
