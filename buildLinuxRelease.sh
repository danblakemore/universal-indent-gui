#!/bin/bash
echo setting Qt path to /usr/local/Trolltech/Qt-4.2.2/bin
echo ----------------------------------------------------
PATH=/usr/local/Trolltech/Qt-4.2.2/bin:$PATH
export PATH
echo calling rm -r release/debug
echo ---------------------------
rm -r release
rm -r debug
echo calling svn update
echo ------------------
#svn update
echo calling qmake
echo -------------
qmake
echo calling lrelease release
echo ------------------------
lrelease ./translations/universalindent_de.ts -qm ./translations/universalindent_de.qm
lrelease ./translations/universalindent_tw.ts -qm ./translations/universalindent_tw.qm
echo calling make release
echo --------------------
make release
echo calling rm -r UniversalIndentGUI_linux32
echo ----------------------------------------
rm -r UniversalIndentGUI_linux32
echo calling mkdir UniversalIndentGUI_linux32
echo ----------------------------------------
mkdir UniversalIndentGUI_linux32
mkdir UniversalIndentGUI_linux32/data
mkdir UniversalIndentGUI_linux32/doc
mkdir UniversalIndentGUI_linux32/translations
echo copying files to UniversalIndentGUI_linux32
echo ----------------------------------------
cp ./CHANGELOG.txt ./UniversalIndentGUI_linux32/
cp ./LICENSE.GPL ./UniversalIndentGUI_linux32/
cp ./README.txt ./UniversalIndentGUI_linux32/
cp ./INSTALL.txt ./UniversalIndentGUI_linux32/
cp ./data/.astylerc ./UniversalIndentGUI_linux32/data/
cp ./data/astyle ./UniversalIndentGUI_linux32/data/
cp ./data/astyle.html ./UniversalIndentGUI_linux32/data/
cp ./data/bcpp.cfg ./UniversalIndentGUI_linux32/data/
cp ./data/bcpp ./UniversalIndentGUI_linux32/data/
cp ./data/bcpp.txt ./UniversalIndentGUI_linux32/data/
cp ./data/csstidy ./UniversalIndentGUI_linux32/data/
cp ./data/gc.cfg ./UniversalIndentGUI_linux32/data/
cp ./data/gc.exe ./UniversalIndentGUI_linux32/data/
cp ./data/gc.txt ./UniversalIndentGUI_linux32/data/
cp ./data/.indent.pro ./UniversalIndentGUI_linux32/data/
cp ./data/indent ./UniversalIndentGUI_linux32/data/
cp ./data/indent.html ./UniversalIndentGUI_linux32/data/
cp ./data/uncrustify.cfg ./UniversalIndentGUI_linux32/data/
cp ./data/uncrustify ./UniversalIndentGUI_linux32/data/
cp ./data/uncrustify.txt ./UniversalIndentGUI_linux32/data/
cp ./data/uigui_astyle.ini ./UniversalIndentGUI_linux32/data/
cp ./data/uigui_bcpp.ini ./UniversalIndentGUI_linux32/data/
cp ./data/uigui_csstidy.ini ./UniversalIndentGUI_linux32/data/
cp ./data/uigui_greatcode.ini ./UniversalIndentGUI_linux32/data/
cp ./data/uigui_gnuindent.ini ./UniversalIndentGUI_linux32/data/
cp ./data/uigui_uncrustify.ini ./UniversalIndentGUI_linux32/data/
cp ./data/example.cpp ./UniversalIndentGUI_linux32/data/
cp ./translations/*.qm ./UniversalIndentGUI_linux32/translations/
cp ./doc/iniFileFormat.html ./UniversalIndentGUI_linux32/doc/
cp ./release/UniversalIndentGUI ./UniversalIndentGUI_linux32/
cd UniversalIndentGUI_linux32
echo compressing UniversalIndentGUI_linux32
echo ----------------------------------------
tar czf UniversalIndentGUI_0.4.2_Beta_linux32.tgz *
cd ..
read -p "press any key to continue"
