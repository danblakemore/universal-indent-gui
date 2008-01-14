#!/bin/bash
echo "setting Qt path to /usr/local/Trolltech/Qt-4.2.1/bin"
echo "----------------------------------------------------"
QTDIR=/usr/local/Trolltech/Qt-4.2.1
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
echo calling rm -r UniversalIndentGUI_linux32
echo ----------------------------------------
rm -r UniversalIndentGUI_linux32
echo calling mkdir UniversalIndentGUI_linux32
echo ----------------------------------------
mkdir UniversalIndentGUI_linux32
mkdir UniversalIndentGUI_linux32/indenters
mkdir UniversalIndentGUI_linux32/doc
mkdir UniversalIndentGUI_linux32/translations
echo copying files to UniversalIndentGUI_linux32
echo ----------------------------------------
cp ./CHANGELOG.txt ./UniversalIndentGUI_linux32/
cp ./LICENSE.GPL ./UniversalIndentGUI_linux32/
cp ./README.txt ./UniversalIndentGUI_linux32/
cp ./INSTALL.txt ./UniversalIndentGUI_linux32/
#cp ./indenters/.astylerc ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/astyle ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/astyle.html ./UniversalIndentGUI_linux32/indenters/
#cp ./indenters/bcpp.cfg ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/bcpp ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/bcpp.txt ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/csstidy ./UniversalIndentGUI_linux32/indenters/
#cp ./indenters/gc.cfg ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/gc.exe ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/gc.txt ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/tidy ./UniversalIndentGUI_linux32/indenters/
#cp ./indenters/.indent.pro ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/indent ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/indent.html ./UniversalIndentGUI_linux32/indenters/
#cp ./indenters/uncrustify.cfg ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/uncrustify ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/uncrustify.txt ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/uigui_astyle.ini ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/uigui_bcpp.ini ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/uigui_csstidy.ini ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/uigui_greatcode.ini ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/uigui_phpCB.ini ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/uigui_gnuindent.ini ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/uigui_uncrustify.ini ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/uigui_tidy.ini ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/highlighter.ini ./UniversalIndentGUI_linux32/indenters/
cp ./indenters/example.cpp ./UniversalIndentGUI_linux32/indenters/

# Copy the translation files.
cp $QTDIR/translations/qt_de.qm ./translations/
cp $QTDIR/translations/qt_ja_jp.qm ./translations/qt_ja_JP.qm
cp ./translations/*.qm ./UniversalIndentGUI_linux32/translations/

cp ./doc/iniFileFormat.html ./UniversalIndentGUI_linux32/doc/
cp ./release/UniversalIndentGUI ./UniversalIndentGUI_linux32/
cd UniversalIndentGUI_linux32
echo compressing UniversalIndentGUI_linux32
echo ----------------------------------------
tar czf UniversalIndentGUI_0.6.1_Beta_linux32.tgz *
cd ..
read -p "press any key to continue"
