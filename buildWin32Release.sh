#!/bin/sh
#!/bin/bash
# 1. param is target system
if [ -n "$1" ]; then
    targetSystem=$1
else
    targetSystem="src"
fi


# Configuration
# -------------
ext=.exe
targetDir=UniversalIndentGUI_$targetSystem
version=0.6.1_Beta
doSVNUpdate=false
languages="de zh_TW ja_JP"

# Qt specific settings
#QTDIR=/c/Programmierung/qt.4.3.0_gcc_static
QTDIR=/f/Qt/qt.4.3.2_gpl_static
QMAKESPEC=win32-g++

echo "Making some environment settings"
echo "--------------------------------"
export QTDIR
PATH=$QTDIR/bin:$PATH
export PATH
export QMAKESPEC
echo "Done"
echo ""


echo "Delete old target dir and create new one"
echo "----------------------------------------"
if [ -d "$targetDir" ]; then
    rm -r $targetDir &> /dev/null
fi
if [ $? -gt 0 ]; then
    echo "ERROR: Deleting dir $targetDir failed!"
    exit 1
fi
mkdir $targetDir &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Creating dir $targetDir failed!"
    exit 1
fi
mkdir $targetDir/data &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Creating dir data failed!"
    exit 1
fi
mkdir $targetDir/doc &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Creating dir doc failed!"
    exit 1
fi
mkdir $targetDir/translations &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Creating dir translations failed!"
    exit 1
fi
# In case of source files as target system, create additional dir.
if [ "$targetSystem" = "src" ]; then
    mkdir $targetDir/resources &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Creating dir resources failed!"
        exit 1
    fi
    mkdir $targetDir/src &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Creating dir src failed!"
        exit 1
    fi
fi
echo "Done"
echo ""


if [ "$doSVNUpdate" = "true" ]; then
    echo "Calling svn update"
    echo "------------------"
    svn update
    if [ $? -gt 0 ]; then
        echo "ERROR: Calling svn update failed!"
        exit 1
    fi
    echo "Done"
    echo ""
fi

echo "Updating the translation files"
echo "------------------------------"
lupdate src -ts ./translations/universalindent.ts &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not update file \"universalindent.ts\"!"
    exit 1
fi
for i in $languages
do
    lupdate src -ts ./translations/universalindent_$i.ts &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not update file \"universalindent_$i.ts\"!"
        exit 1
    fi
done
echo "Done"
echo ""

###################### source release begin ########################
if [ "$targetSystem" = "src" ]; then

echo "Copying the translation files to the target translation dir"
echo "-----------------------------------------------------------"
cp ./translations/universalindent.ts ./$targetDir/translations/ &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy file \"universalindent.ts\"!"
    exit 1
fi
for i in $languages
do
    cp ./translations/universalindent_$i.ts ./$targetDir/translations/ &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not copy file \"universalindent_$i.ts\"!"
        exit 1
    fi
done
echo "Done"
echo ""


echo "Copying the resources and src files to the target dir"
echo "-----------------------------------------------------"
cp ./resources/* ./$targetDir/resources/ &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy dir \"resources\"!"
    exit 1
fi
cp ./src/* ./$targetDir/src/ &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy dir \"src\"!"
    exit 1
fi
echo "Done"
echo ""

###################### source release end ########################
else
###################### binary release begin ########################

echo "Cleaning up release/debug dirs"
echo "------------------------------"
if [ -d "release" ]; then
    rm -r release &> /dev/null
fi
if [ $? -gt 0 ]; then
    echo "ERROR: Could not delete release dir!"
    exit 1
fi
if [ -d "debug" ]; then
    rm -r debug &> /dev/null
fi
if [ $? -gt 0 ]; then
    echo "ERROR: Could not delete debug dir!"
    exit 1
fi
echo "Done"
echo ""


echo "Calling qmake"
echo "-------------"
qmake
if [ $? -gt 0 ]; then
    echo "ERROR: Calling qmake failed!"
    exit 1
fi
echo "Done"
echo ""


echo "Calling make release"
echo "--------------------"
make release --silent
if [ $? -gt 0 ]; then
    echo "ERROR: Calling make release failed!"
    exit 1
fi
echo "Done"
echo ""


echo "Copying UniversalIndentGUI$ext to target dir"
echo "--------------------------------------------"
cp ./release/UniversalIndentGUI$ext ./$targetDir/ &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy file \"UniversalIndentGUI$ext\"!"
    exit 1
fi
echo "Done"
echo ""


echo "Copying the indenter executables and example file to the target data dir"
echo "------------------------------------------------------------------------"
indenters="astyle$ext astyle.html bcpp$ext bcpp.txt csstidy$ext gc.exe gc.txt htmltidy$ext htmltidy.html indent$ext indent.html uncrustify$ext uncrustify.txt example.cpp"
if [ "$ext" = ".exe" ]; then
    indenters="$indenters libiconv-2.dll libintl-2.dll"
fi
for i in $indenters
do
    cp ./data/$i ./$targetDir/data/ &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not copy file \"$i\"!"
        exit 1
    fi
done
echo "Done"
echo ""


echo "Generating the translation binaries"
echo "-----------------------------------"
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


echo "Copying the translation binaries to the target translation dir"
echo "--------------------------------------------------------------"
cp $QTDIR/translations/qt_de.qm ./$targetDir/translations/ &> /dev/null
cp $QTDIR/translations/qt_ja_jp.qm ./$targetDir/translations/qt_ja_JP.qm &> /dev/null
for i in $languages
do
    cp ./translations/universalindent_$i.qm ./$targetDir/translations/ &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not copy file \"universalindent_$i.qm\"!"
        exit 1
    fi
done
echo "Done"
echo ""

fi
###################### binary release end ########################

echo "Copying the indenter uigui ini files to the target data dir"
echo "-----------------------------------------------------------"
inifiles="uigui_astyle.ini uigui_bcpp.ini uigui_csstidy.ini uigui_gnuindent.ini uigui_greatcode.ini uigui_htmltidy.ini uigui_phpCB.ini uigui_uncrustify.ini highlighter.ini"
for i in $inifiles
do
    cp ./data/$i ./$targetDir/data/ &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not copy file \"$i\"!"
        exit 1
    fi
done
echo "Done"
echo ""


echo "Copying some other files (README, CHANGELOG etc)"
echo "------------------------------------------------"
otherfiles="CHANGELOG.txt LICENSE.GPL INSTALL.txt README.txt"
for i in $otherfiles
do
    cp ./$i ./$targetDir/ &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not copy file \"$i\"!"
        exit 1
    fi
done
echo "Done"
echo ""


echo "Copying doc to target dir"
echo "-------------------------"
cp ./doc/iniFileFormat.html ./$targetDir/doc/ &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy file \"iniFileFormat.html\"!"
    exit 1
fi
echo "Done"
echo ""


echo "Packing the whole target dir content"
echo "------------------------------------"
cd $targetDir
tar czf UniversalIndentGUI_$version_$targetSystem.tgz *
if [ $? -gt 0 ]; then
    echo "ERROR: Could not create the archive \"UniversalIndentGUI_$version_$targetSystem.tgz\"!"
    exit 1
fi
cd ..
echo "Done"
echo ""


echo "Everything completed successfull!"
#read -p "press any key to continue"
