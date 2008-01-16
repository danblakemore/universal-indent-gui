@echo off

set targetname=universalindentgui-0.8.0

echo Making some environment settings
echo --------------------------------
set PATH=%PATH%;D:\Programme\Tools\7-Zip
echo Done.
echo.

echo Delete old release dir and create new one
echo -----------------------------------------
rd %targetname% /S /Q
md %targetname%
IF ERRORLEVEL 1 goto ERROR
cd %targetname%
IF ERRORLEVEL 1 goto ERROR
md UniversalIndentGUI.xcodeproj
IF ERRORLEVEL 1 goto ERROR
md indenters
IF ERRORLEVEL 1 goto ERROR
md resources
IF ERRORLEVEL 1 goto ERROR
md translations
IF ERRORLEVEL 1 goto ERROR
md src
IF ERRORLEVEL 1 goto ERROR
md doc
IF ERRORLEVEL 1 goto ERROR
md config
IF ERRORLEVEL 1 goto ERROR
cd ..
echo Done.
echo.


echo Updating translation files
echo --------------------------
lupdate UniversalIndentGUI.pro -silent
IF ERRORLEVEL 1 goto ERROR
echo Done.
echo.

echo Copying the indenter uigui ini files to the release indenters dir
echo -----------------------------------------------------------------
FOR %%A IN ( uigui_astyle.ini, uigui_bcpp.ini, uigui_csstidy.ini, uigui_gnuindent.ini, uigui_greatcode.ini, uigui_jsdecoder.ini, uigui_perltidy.ini, uigui_phpCB.ini, uigui_shellindent.ini, uigui_tidy.ini, uigui_uncrustify.ini ) DO (
    if not exist .\indenters\%%A (
        echo File .\indenters\%%A not found!
        goto ERROR
    )
    copy .\indenters\%%A .\%targetname%\indenters\ >NUL
    IF ERRORLEVEL 1 goto ERROR
)
echo Done.
echo.


echo Copying the script indenters to the release indenters dir
echo ---------------------------------------------------------
FOR %%A IN ( JsDecoder.js, perltidy, PerlTidyLib.pm, shellindent.awk ) DO (
    if not exist .\indenters\%%A (
        echo File .\indenters\%%A not found!
        goto ERROR
    )
    copy .\indenters\%%A .\%targetname%\indenters\ >NUL
    IF ERRORLEVEL 1 goto ERROR
)
echo Done.
echo.


echo Copying the default highlighter ini files to the release config dir
echo -------------------------------------------------------------------
FOR %%A IN ( UiGuiSyntaxHighlightConfig.ini ) DO (
    if not exist .\config\%%A (
        echo File .\config\%%A not found!
        goto ERROR
    )
    copy .\config\%%A .\%targetname%\config\ >NUL
    IF ERRORLEVEL 1 goto ERROR
)
echo Done.
echo.

echo Copying translations, resources and src dir
echo -------------------------------------------
copy .\translations\*.ts .\%targetname%\translations\ >NUL
IF ERRORLEVEL 1 goto ERROR
copy .\resources\* .\%targetname%\resources\ >NUL
IF ERRORLEVEL 1 goto ERROR
copy .\src\* .\%targetname%\src\ >NUL
IF ERRORLEVEL 1 goto ERROR
del .\%targetname%\src\*.user >NUL
IF ERRORLEVEL 1 goto ERROR
echo Done.
echo.

echo Copying some other files (README, CHANGELOG etc)
echo ------------------------------------------------
FOR %%A IN ( CHANGELOG.txt, LICENSE.GPL, INSTALL.txt, README.txt, UniversalIndentGUI.pro, UniversalIndentGUI.sln, VS8QtRules.rules ) DO (
    if not exist .\%%A (
        echo File .\indenters\%%A not found!
        goto ERROR
    )
    copy .\%%A .\%targetname%\ >NUL
    IF ERRORLEVEL 1 goto ERROR
)
echo Done.
echo.

echo Copying XCode files
echo -------------------
rem qmake -spec macx-xcode UniversalIndentGUI.pro >NUL
rem IF ERRORLEVEL 1 goto ERROR
cd UniversalIndentGUI.xcodeproj
FOR %%A IN ( project.pbxproj, qt_makeqmake.mak, qt_preprocess.mak ) DO (
    if not exist .\%%A (
        echo File .\indenters\%%A not found!
        goto ERROR
    )
    copy .\%%A ..\%targetname%\UniversalIndentGUI.xcodeproj\ >NUL
    IF ERRORLEVEL 1 goto ERROR
)
cd ..
echo Done.
echo.

echo Copying doc and UniversalIndentGUI.exe to release dir
echo -----------------------------------------------------
copy .\doc\iniFileFormat.html .\%targetname%\doc\ >NUL
IF ERRORLEVEL 1 goto ERROR
echo Done.
echo.

echo Packing the whole release dir content
echo -------------------------------------
rem cd %targetname%
del %targetname%.tar >NUL
del %targetname%.tar.gz >NUL
7z.exe a -ttar %targetname%.tar %targetname% >NUL
7z.exe a -tgzip %targetname%.tar.gz %targetname%.tar >NUL
del %targetname%.tar >NUL
IF ERRORLEVEL 1 goto ERROR
rem cd ..
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
