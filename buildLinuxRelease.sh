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

echo calling rm -r universalindentgui_linux32
echo ----------------------------------------
rm -r universalindentgui_linux32

echo calling mkdir universalindentgui_linux32
echo ----------------------------------------
mkdir universalindentgui_linux32
mkdir universalindentgui_linux32/config
mkdir universalindentgui_linux32/indenters
mkdir universalindentgui_linux32/doc
mkdir universalindentgui_linux32/translations

echo copying files to universalindentgui_linux32
echo ----------------------------------------
cp ./CHANGELOG.txt ./universalindentgui_linux32/
cp ./LICENSE.GPL ./universalindentgui_linux32/
cp ./README.txt ./universalindentgui_linux32/
cp ./INSTALL.txt ./universalindentgui_linux32/

#cp ./indenters/.astylerc ./universalindentgui_linux32/indenters/
cp ./indenters/astyle ./universalindentgui_linux32/indenters/
cp ./indenters/astyle.html ./universalindentgui_linux32/indenters/
#cp ./indenters/bcpp.cfg ./universalindentgui_linux32/indenters/
cp ./indenters/bcpp ./universalindentgui_linux32/indenters/
cp ./indenters/bcpp.txt ./universalindentgui_linux32/indenters/
cp ./indenters/csstidy ./universalindentgui_linux32/indenters/
#cp ./indenters/gc.cfg ./universalindentgui_linux32/indenters/
cp ./indenters/gc.exe ./universalindentgui_linux32/indenters/
cp ./indenters/gc.txt ./universalindentgui_linux32/indenters/
#cp ./indenters/.indent.pro ./universalindentgui_linux32/indenters/
cp ./indenters/indent ./universalindentgui_linux32/indenters/
cp ./indenters/indent.html ./universalindentgui_linux32/indenters/
cp ./indenters/JsDecoder.js ./universalindentgui_linux32/indenters/
cp ./indenters/perltidy ./universalindentgui_linux32/indenters/
cp ./indenters/PerlTidyLib.pm ./universalindentgui_linux32/indenters/
cp ./indenters/shellindent.awk ./universalindentgui_linux32/indenters/
cp ./indenters/tidy ./universalindentgui_linux32/indenters/
#cp ./indenters/uncrustify.cfg ./universalindentgui_linux32/indenters/
cp ./indenters/uncrustify ./universalindentgui_linux32/indenters/
cp ./indenters/uncrustify.txt ./universalindentgui_linux32/indenters/

cp ./indenters/uigui_*.ini ./universalindentgui_linux32/indenters/

cp ./indenters/example.* ./universalindentgui_linux32/indenters/

cp ./config/UiGuiSyntaxHighlightConfig.ini ./universalindentgui_linux32/config/

# Copy the translation files.
cp $QTDIR/translations/qt_de.qm ./translations/
cp $QTDIR/translations/qt_ja_jp.qm ./translations/qt_ja_JP.qm
cp ./translations/*.qm ./universalindentgui_linux32/translations/

cp ./doc/iniFileFormat.html ./universalindentgui_linux32/doc/
cp ./release/universalindentgui ./universalindentgui_linux32/

#cd universalindentgui_linux32
echo compressing universalindentgui_linux32
echo --------------------------------------
tar czf universalindentgui_0.8.0_beta_linux32.tgz universalindentgui_linux32
#cd ..
read -p "press any key to continue"
