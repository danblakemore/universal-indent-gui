rd UniversalIndentGUI_src /S /Q
md UniversalIndentGUI_src
cd UniversalIndentGUI_src
md data
md resources
md src
cd ..
copy .\data\uigui_astyle.ini .\UniversalIndentGUI_src\data
copy .\data\uigui_greatcode.ini .\UniversalIndentGUI_src\data
copy .\data\uigui_gnuindent.ini .\UniversalIndentGUI_src\data
copy .\resources\* .\UniversalIndentGUI_src\resources
copy .\src\* .\UniversalIndentGUI_src\src
copy .\changelog.txt .\UniversalIndentGUI_src\
copy .\LICENSE.GPL .\UniversalIndentGUI_src\
rem copy .\makeall.bat .\UniversalIndentGUI_src\
copy .\readme.txt .\UniversalIndentGUI_src\
copy .\UniversalIndentGUI.pro .\UniversalIndentGUI_src\
copy .\UniversalIndentGUI.sln .\UniversalIndentGUI_src\
copy .\VS8QtRules.rules .\UniversalIndentGUI_src\
cd UniversalIndentGUI_src
7z.exe a -tzip UniversalIndentGUI_0.3_Beta_src.zip
cd ..
