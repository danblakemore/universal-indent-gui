#!/bin/bash
echo "setting Qt path"
echo "---------------"
QTDIR=/Users/thomas/Documents/Informatik/qt-static-release
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
lrelease ./translations/universalindent_ru.ts -qm ./translations/universalindent_ru.qm
lrelease ./translations/universalindent_uk.ts -qm ./translations/universalindent_uk.qm

echo calling make release
echo --------------------
make release

echo calling rm -r UniversalIndentGUI_mac
echo ----------------------------------------
rm -r UniversalIndentGUI_mac

echo calling mkdir UniversalIndentGUI_mac
echo ----------------------------------------
mkdir UniversalIndentGUI_mac
mkdir UniversalIndentGUI_mac/config
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
#cp ./indenters/.indent.pro ./UniversalIndentGUI_mac/indenters/
cp ./indenters/hindent ./UniversalIndentGUI_mac/indenters/
cp ./indenters/hindent.html ./UniversalIndentGUI_mac/indenters/
cp ./indenters/indent ./UniversalIndentGUI_mac/indenters/
cp ./indenters/indent.html ./UniversalIndentGUI_mac/indenters/
cp ./indenters/JsDecoder.js ./UniversalIndentGUI_mac/indenters/
cp ./indenters/perltidy ./UniversalIndentGUI_mac/indenters/
cp ./indenters/PerlTidyLib.pm ./UniversalIndentGUI_mac/indenters/
cp ./indenters/php_beautifier.html ./UniversalIndentGUI_mac/indenters/
cp ./indenters/phpStylist.php ./UniversalIndentGUI_mac/indenters/
cp ./indenters/phpStylist.txt ./UniversalIndentGUI_mac/indenters/
cp ./indenters/shellindent.awk ./UniversalIndentGUI_mac/indenters/
cp ./indenters/tidy ./UniversalIndentGUI_mac/indenters/
#cp ./indenters/uncrustify.cfg ./UniversalIndentGUI_mac/indenters/
cp ./indenters/uncrustify ./UniversalIndentGUI_mac/indenters/
cp ./indenters/uncrustify.txt ./UniversalIndentGUI_mac/indenters/

cp ./indenters/uigui_*.ini ./UniversalIndentGUI_mac/indenters/

cp ./indenters/example.* ./UniversalIndentGUI_mac/indenters/

cp ./config/UiGuiSyntaxHighlightConfig.ini ./UniversalIndentGUI_mac/config/

# Copy the translation files.
cp $QTDIR/translations/qt_de.qm ./translations/
cp $QTDIR/translations/qt_ja_jp.qm ./translations/qt_ja_JP.qm
cp $QTDIR/translations/qt_ru.qm ./translations/
cp $QTDIR/translations/qt_uk.qm ./translations/
cp ./translations/*.qm ./UniversalIndentGUI_mac/translations/

cp ./doc/iniFileFormat.html ./UniversalIndentGUI_mac/doc/
cp -r ./release/universalindentgui.app ./UniversalIndentGUI_mac/

#cd UniversalIndentGUI_mac
echo compressing UniversalIndentGUI_mac
echo --------------------------------------
tar czf universalindentgui_0.8.2_mac.tgz UniversalIndentGUI_mac
#cd ..
read -p "press any key to continue"
