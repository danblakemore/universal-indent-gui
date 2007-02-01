@echo off
set QTDIR=%QTDIR%_static
set PATH=%QTDIR%\bin
set PATH=%PATH%;D:\Programme\Informat\MinGW\bin;D:\Programme\Tools\7-Zip
set PATH=%PATH%;%SystemRoot%\System32
set QMAKESPEC=win32-g++
lrelease .\translations\universalindent_de.ts -qm .\translations\universalindent_de.qm
qmake
make release
rd UniversalIndentGUI_win32 /S /Q
md UniversalIndentGUI_win32
cd UniversalIndentGUI_win32
md translations
md data
md doc
cd ..
copy .\data\.astylerc .\UniversalIndentGUI_win32\data
copy .\data\astyle.exe .\UniversalIndentGUI_win32\data
copy .\data\astyle.html .\UniversalIndentGUI_win32\data
copy .\data\indent.exe .\UniversalIndentGUI_win32\data
copy .\data\indent.html .\UniversalIndentGUI_win32\data
copy .\data\.indent.pro .\UniversalIndentGUI_win32\data
copy .\data\bcpp.cfg .\UniversalIndentGUI_win32\data
copy .\data\bcpp.exe .\UniversalIndentGUI_win32\data
copy .\data\bcpp.txt .\UniversalIndentGUI_win32\data
copy .\data\gc.cfg .\UniversalIndentGUI_win32\data
copy .\data\gc.exe .\UniversalIndentGUI_win32\data
copy .\data\gc.txt .\UniversalIndentGUI_win32\data
copy .\data\uigui_astyle.ini .\UniversalIndentGUI_win32\data
copy .\data\uigui_bcpp.ini .\UniversalIndentGUI_win32\data
copy .\data\uigui_gnuindent.ini .\UniversalIndentGUI_win32\data
copy .\data\uigui_greatcode.ini .\UniversalIndentGUI_win32\data
copy .\data\example.cpp .\UniversalIndentGUI_win32\data
copy .\CHANGELOG.txt .\UniversalIndentGUI_win32\
copy .\LICENSE.GPL .\UniversalIndentGUI_win32\
copy .\INSTALL.txt .\UniversalIndentGUI_win32\
copy .\README.txt .\UniversalIndentGUI_win32\
copy .\translations\*.qm .\UniversalIndentGUI_win32\translations\
copy .\doc\iniFileFormat.html .\UniversalIndentGUI_win32\doc\
copy .\release\UniversalIndentGUI.exe .\UniversalIndentGUI_win32\
cd UniversalIndentGUI_win32
7z.exe a -tzip UniversalIndentGUI_0.4_Beta_win32.zip
cd ..
pause
