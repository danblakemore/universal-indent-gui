rd UniversalIndentGUI_src /S /Q
md UniversalIndentGUI_src
cd UniversalIndentGUI_src
md data
md resources
md src
md doc
cd ..
copy .\data\uigui_astyle.ini .\UniversalIndentGUI_src\data
copy .\data\uigui_greatcode.ini .\UniversalIndentGUI_src\data
copy .\data\uigui_gnuindent.ini .\UniversalIndentGUI_src\data
copy .\data\uigui_bcpp.ini .\UniversalIndentGUI_src\data
copy .\resources\* .\UniversalIndentGUI_src\resources
copy .\src\* .\UniversalIndentGUI_src\src
copy .\CHANGELOG.txt .\UniversalIndentGUI_src\
copy .\LICENSE.GPL .\UniversalIndentGUI_src\
copy .\README.txt .\UniversalIndentGUI_src\
copy .\INSTALL.txt .\UniversalIndentGUI_src\
copy .\UniversalIndentGUI.pro .\UniversalIndentGUI_src\
copy .\UniversalIndentGUI.sln .\UniversalIndentGUI_src\
copy .\VS8QtRules.rules .\UniversalIndentGUI_src\
copy .\doc\iniFileFormat.html .\UniversalIndentGUI_src\doc\
cd UniversalIndentGUI_src
7z.exe a -tzip UniversalIndentGUI_0.4_Beta_src.zip
cd ..
