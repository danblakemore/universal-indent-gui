#!/bin/bash
echo "setting Qt path"
echo "---------------"
QTDIR=/Users/thomas/Downloads/qt_installed
export QTDIR
PATH=$QTDIR/bin:$PATH
export PATH
echo "calling rm -r release/debug"
echo "---------------------------"
rm -r release
rm -r debug
echo "calling svn update"
echo "------------------"
#svn update
echo "calling qmake"
echo "-------------"
qmake
echo calling lrelease release
echo ------------------------
lrelease ./translations/universalindent_de.ts -qm ./translations/universalindent_de.qm
lrelease ./translations/universalindent_zh_TW.ts -qm ./translations/universalindent_zh_TW.qm
lrelease ./translations/universalindent_ja_JP.ts -qm ./translations/universalindent_ja_JP.qm
echo calling make release
echo --------------------
make release
echo calling rm -r UniversalIndentGUI_mac
echo ----------------------------------------
rm -r UniversalIndentGUI_mac
echo calling mkdir UniversalIndentGUI_mac
echo ----------------------------------------
mkdir UniversalIndentGUI_mac
mkdir UniversalIndentGUI_mac/data
mkdir UniversalIndentGUI_mac/doc
mkdir UniversalIndentGUI_mac/translations
echo copying files to UniversalIndentGUI_mac
echo ----------------------------------------
cp ./CHANGELOG.txt ./UniversalIndentGUI_mac/
cp ./LICENSE.GPL ./UniversalIndentGUI_mac/
cp ./README.txt ./UniversalIndentGUI_mac/
cp ./INSTALL.txt ./UniversalIndentGUI_mac/
#cp ./data/.astylerc ./UniversalIndentGUI_mac/data/
cp ./data/astyle ./UniversalIndentGUI_mac/data/
cp ./data/astyle.html ./UniversalIndentGUI_mac/data/
#cp ./data/bcpp.cfg ./UniversalIndentGUI_mac/data/
cp ./data/bcpp ./UniversalIndentGUI_mac/data/
cp ./data/bcpp.txt ./UniversalIndentGUI_mac/data/
cp ./data/csstidy ./UniversalIndentGUI_mac/data/
#cp ./data/gc.cfg ./UniversalIndentGUI_mac/data/
cp ./data/gc.exe ./UniversalIndentGUI_mac/data/
cp ./data/gc.txt ./UniversalIndentGUI_mac/data/
cp ./data/htmltidy ./UniversalIndentGUI_mac/data/
#cp ./data/.indent.pro ./UniversalIndentGUI_mac/data/
cp ./data/indent ./UniversalIndentGUI_mac/data/
cp ./data/indent.html ./UniversalIndentGUI_mac/data/
#cp ./data/uncrustify.cfg ./UniversalIndentGUI_mac/data/
cp ./data/uncrustify ./UniversalIndentGUI_mac/data/
cp ./data/uncrustify.txt ./UniversalIndentGUI_mac/data/
cp ./data/uigui_astyle.ini ./UniversalIndentGUI_mac/data/
cp ./data/uigui_bcpp.ini ./UniversalIndentGUI_mac/data/
cp ./data/uigui_csstidy.ini ./UniversalIndentGUI_mac/data/
cp ./data/uigui_greatcode.ini ./UniversalIndentGUI_mac/data/
cp ./data/uigui_phpCB.ini ./UniversalIndentGUI_mac/data/
cp ./data/uigui_gnuindent.ini ./UniversalIndentGUI_mac/data/
cp ./data/uigui_uncrustify.ini ./UniversalIndentGUI_mac/data/
cp ./data/uigui_htmltidy.ini ./UniversalIndentGUI_mac/data/
cp ./data/highlighter.ini ./UniversalIndentGUI_mac/data/
cp ./data/example.cpp ./UniversalIndentGUI_mac/data/

# Copy the translation files.
cp $QTDIR/translations/qt_de.qm ./translations/
cp $QTDIR/translations/qt_ja_jp.qm ./translations/qt_ja_JP.qm
cp ./translations/*.qm ./UniversalIndentGUI_mac/translations/

cp ./doc/iniFileFormat.html ./UniversalIndentGUI_mac/doc/
cp -r ./release/UniversalIndentGUI.app ./UniversalIndentGUI_mac/
cd UniversalIndentGUI_mac
echo compressing UniversalIndentGUI_mac
echo ----------------------------------------
tar czf UniversalIndentGUI_0.7.1_Beta_mac.tgz *
cd ..
read -p "press any key to continue"
