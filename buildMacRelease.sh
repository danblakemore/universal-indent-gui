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
mkdir UniversalIndentGUI_mac/indenters
mkdir UniversalIndentGUI_mac/doc
mkdir UniversalIndentGUI_mac/translations
echo copying files to UniversalIndentGUI_mac
echo ----------------------------------------
cp ./CHANGELOG.txt ./UniversalIndentGUI_mac/
cp ./LICENSE.GPL ./UniversalIndentGUI_mac/
cp ./README.txt ./UniversalIndentGUI_mac/
cp ./INSTALL.txt ./UniversalIndentGUI_mac/
#cp ./indenters/.astylerc ./UniversalIndentGUI_mac/indenters/
cp ./indenters/astyle ./UniversalIndentGUI_mac/indenters/
cp ./indenters/astyle.html ./UniversalIndentGUI_mac/indenters/
#cp ./indenters/bcpp.cfg ./UniversalIndentGUI_mac/indenters/
cp ./indenters/bcpp ./UniversalIndentGUI_mac/indenters/
cp ./indenters/bcpp.txt ./UniversalIndentGUI_mac/indenters/
cp ./indenters/csstidy ./UniversalIndentGUI_mac/indenters/
#cp ./indenters/gc.cfg ./UniversalIndentGUI_mac/indenters/
cp ./indenters/gc.exe ./UniversalIndentGUI_mac/indenters/
cp ./indenters/gc.txt ./UniversalIndentGUI_mac/indenters/
cp ./indenters/htmltidy ./UniversalIndentGUI_mac/indenters/
#cp ./indenters/.indent.pro ./UniversalIndentGUI_mac/indenters/
cp ./indenters/indent ./UniversalIndentGUI_mac/indenters/
cp ./indenters/indent.html ./UniversalIndentGUI_mac/indenters/
#cp ./indenters/uncrustify.cfg ./UniversalIndentGUI_mac/indenters/
cp ./indenters/uncrustify ./UniversalIndentGUI_mac/indenters/
cp ./indenters/uncrustify.txt ./UniversalIndentGUI_mac/indenters/
cp ./indenters/uigui_astyle.ini ./UniversalIndentGUI_mac/indenters/
cp ./indenters/uigui_bcpp.ini ./UniversalIndentGUI_mac/indenters/
cp ./indenters/uigui_csstidy.ini ./UniversalIndentGUI_mac/indenters/
cp ./indenters/uigui_greatcode.ini ./UniversalIndentGUI_mac/indenters/
cp ./indenters/uigui_phpCB.ini ./UniversalIndentGUI_mac/indenters/
cp ./indenters/uigui_gnuindent.ini ./UniversalIndentGUI_mac/indenters/
cp ./indenters/uigui_uncrustify.ini ./UniversalIndentGUI_mac/indenters/
cp ./indenters/uigui_htmltidy.ini ./UniversalIndentGUI_mac/indenters/
cp ./indenters/highlighter.ini ./UniversalIndentGUI_mac/indenters/
cp ./indenters/example.cpp ./UniversalIndentGUI_mac/indenters/

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
