#!/bin/bash
echo "Making some environment settings"
echo "--------------------------------"
QTDIR=/f/Qt/qt.4.3.0_gpl_static
export QTDIR
PATH=$QTDIR/bin:$PATH
export PATH
QMAKESPEC=win32-g++
export QMAKESPEC
echo "Done"
echo ""

echo "Generating the translation binaries"
echo "-----------------------------------"
languages='de tw ja'
for i in $languages
do
    lrelease ./translations/universalindent_$i.ts -qm ./translations/universalindent_$i.qm -silent
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not create translation file \"universalindent_$i.qm\"!"
        exit 1
    fi
done
echo "Done"
echo ""

echo "Cleaning up release/debug dirs"
echo "------------------------------"
rm -r release &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not delete release dir!"
    exit 1
fi
rm -r debug &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not delete debug dir!"
    exit 1
fi
echo "Done"
echo ""

#echo "Calling svn update"
#echo "------------------"
#svn update
#if [ $? -gt 0 ]; then
#    echo "ERROR: Calling svn update failed!"
#    exit 1
#fi
#echo "Done"
#echo ""

echo "Calling qmake"
echo "-------------"
qmake &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Calling qmake failed!"
    exit 1
fi
echo "Done"
echo ""

echo "Calling make release"
echo "--------------------"
make release &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Calling make release failed!"
    exit 1
fi
echo "Done"
echo ""

echo "Delete old release dir and create new one"
echo "-----------------------------------------"
#todo check if exist and only then try to delete
rm -r UniversalIndentGUI_win32 &> /dev/null
#if [ $? -gt 0 ]; then
#    echo "ERROR: Deleting dir UniversalIndentGUI_win32 failed!"
#    exit 1
#fi
mkdir UniversalIndentGUI_win32 &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Creating dir UniversalIndentGUI_win32 failed!"
    exit 1
fi
mkdir UniversalIndentGUI_win32/data &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Creating dir data failed!"
    exit 1
fi
mkdir UniversalIndentGUI_win32/doc &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Creating dir doc failed!"
    exit 1
fi
mkdir UniversalIndentGUI_win32/translations &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Creating dir translations failed!"
    exit 1
fi
echo "Done"
echo ""

echo "Copying the indenter executables and example file to the release data dir"
echo "-------------------------------------------------------------------------"
indenters='astyle.exe astyle.html bcpp.exe bcpp.txt csstidy.exe gc.exe gc.txt htmltidy.exe htmltidy.html indent.exe indent.html uncrustify.exe uncrustify.txt example.cpp'
for i in $indenters
do
    cp ./data/$i ./UniversalIndentGUI_win32/data/ &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not copy file \"$i\"!"
        exit 1
    fi
done
echo "Done"
echo ""

echo "Copying the indenter uigui ini files to the release data dir"
echo "------------------------------------------------------------"
inifiles='uigui_astyle.ini uigui_bcpp.ini uigui_csstidy.ini uigui_gnuindent.ini uigui_greatcode.ini uigui_htmltidy.ini uigui_phpCB.ini uigui_uncrustify.ini highlighter.ini'
for i in $inifiles
do
    cp ./data/$i ./UniversalIndentGUI_win32/data/ &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not copy file \"$i\"!"
        exit 1
    fi
done
echo "Done"
echo ""

echo "Copying some other files (README, CHANGELOG etc)"
echo "------------------------------------------------"
otherfiles='CHANGELOG.txt LICENSE.GPL INSTALL.txt README.txt'
for i in $otherfiles
do
    cp ./$i ./UniversalIndentGUI_win32/ &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not copy file \"$i\"!"
        exit 1
    fi
done
echo "Done"
echo ""

echo "Copying the translation files to the release translation dir"
echo "------------------------------------------------------------"
cp $QTDIR/translations/qt_de.qm ./translations/ &> /dev/null
cp $QTDIR/translations/qt_ja_jp.qm ./translations/qt_ja.qm &> /dev/null
cp $QTDIR/translations/qt_zh_CN.qm ./translations/qt_tw.qm &> /dev/null
cp .\translations\qt_de.qm .\UniversalIndentGUI_win32\translations\ &> /dev/null
cp .\translations\qt_ja.qm .\UniversalIndentGUI_win32\translations\ &> /dev/null
cp .\translations\qt_tw.qm .\UniversalIndentGUI_win32\translations\ &> /dev/null
for i in $languages
do
    cp ./translations/universalindent_$i.qm ./UniversalIndentGUI_win32/translations/ &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not copy file \"$i\"!"
        exit 1
    fi
done
echo "Done"
echo ""

echo "Copying doc and UniversalIndentGUI.exe to release dir"
echo "-----------------------------------------------------"
cp ./doc/iniFileFormat.html ./UniversalIndentGUI_win32/doc/ &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy file \"iniFileFormat.html\"!"
    exit 1
fi
cp ./release/UniversalIndentGUI.exe ./UniversalIndentGUI_win32/ &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy file \"UniversalIndentGUI.exe\"!"
    exit 1
fi
echo "Done"
echo ""

echo "Packing the whole release dir content"
echo "-------------------------------------"
cd UniversalIndentGUI_win32
tar czf UniversalIndentGUI_0.6.1_Beta_win32.tgz *
if [ $? -gt 0 ]; then
    echo "ERROR: Could not create the archive!"
    exit 1
fi
cd ..
echo "Done"
echo ""

echo "Everything completed successfull!"
read -p "press any key to continue"
