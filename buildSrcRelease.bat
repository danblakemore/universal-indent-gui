@echo off

echo Making some environment settings
echo --------------------------------
set PATH=%PATH%;D:\Programme\Tools\7-Zip
echo Done.
echo.

echo Delete old release dir and create new one
echo -----------------------------------------
rd UniversalIndentGUI_src /S /Q
IF ERRORLEVEL 1 goto ERROR
md UniversalIndentGUI_src
IF ERRORLEVEL 1 goto ERROR
cd UniversalIndentGUI_src
IF ERRORLEVEL 1 goto ERROR
md data
IF ERRORLEVEL 1 goto ERROR
md resources
IF ERRORLEVEL 1 goto ERROR
md translations
IF ERRORLEVEL 1 goto ERROR
md src
IF ERRORLEVEL 1 goto ERROR
md doc
IF ERRORLEVEL 1 goto ERROR
cd ..
echo Done.
echo.

echo Copying the indenter uigui ini files to the release data dir
echo ------------------------------------------------------------
FOR %%A IN ( uigui_astyle.ini, uigui_bcpp.ini, uigui_csstidy.ini, uigui_gnuindent.ini, uigui_greatcode.ini, uigui_htmltidy.ini, uigui_phpCB.ini, uigui_uncrustify.ini, highlighter.ini ) DO (
    if not exist .\data\%%A (
        echo File .\data\%%A not found!
        goto ERROR
    )
    copy .\data\%%A .\UniversalIndentGUI_src\data\ >NUL
    IF ERRORLEVEL 1 goto ERROR
)
echo Done.
echo.

echo Copying translations, resources and src dir
echo -------------------------------------------
copy .\translations\*.ts .\UniversalIndentGUI_src\translations\ >NUL
IF ERRORLEVEL 1 goto ERROR
copy .\resources\* .\UniversalIndentGUI_src\resources\ >NUL
IF ERRORLEVEL 1 goto ERROR
copy .\src\* .\UniversalIndentGUI_src\src\ >NUL
IF ERRORLEVEL 1 goto ERROR
del .\UniversalIndentGUI_src\src\*.user >NUL
IF ERRORLEVEL 1 goto ERROR
echo Done.
echo.

echo Copying some other files (README, CHANGELOG etc)
echo ------------------------------------------------
FOR %%A IN ( CHANGELOG.txt, LICENSE.GPL, INSTALL.txt, README.txt, UniversalIndentGUI.pro, UniversalIndentGUI.sln, VS8QtRules.rules ) DO (
    if not exist .\%%A (
        echo File .\data\%%A not found!
        goto ERROR
    )
    copy .\%%A .\UniversalIndentGUI_src\ >NUL
    IF ERRORLEVEL 1 goto ERROR
)
echo Done.
echo.

echo Copying doc and UniversalIndentGUI.exe to release dir
echo -----------------------------------------------------
copy .\doc\iniFileFormat.html .\UniversalIndentGUI_src\doc\ >NUL
IF ERRORLEVEL 1 goto ERROR
echo Done.
echo.

echo Packing the whole release dir content
echo -------------------------------------
cd UniversalIndentGUI_src
7z.exe a -tzip UniversalIndentGUI_0.6.1_Beta_src.zip >NUL
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
