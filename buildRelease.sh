#!/bin/sh

# Command line syntax:
# buildRelease [targetsystem qtdir]
# Example:
# buildRelease win32 /c/qt.4.3.2
# Default values are targetsystem=src, qtdir=""

# 1. param is target system
if [ -n "$1" ]; then
    targetSystem=$1
else
    targetSystem="src"
fi

# 2. param, which is QTDIR
if [ -n "$2" ]; then
    QTDIR=$2
fi

# Only allow the build targets win32, macx and linux.
if [ -n "$1" ] && [ ! "$1" = "win32" ] && [ ! "$1" = "macx" ]  && [ ! "$1" = "linux" ] && [ ! "$1" = "src" ]; then
    echo "ERROR: Build target \"$1\" not supported! Supported are win32, macx, linux and src."
    exit 1
fi

if [ "$targetSystem" = "win32" ]; then
    ext=".exe"
else
    ext=""
fi

if [ "$targetSystem" = "win32" ] || [ "$targetSystem" = "macx" ]; then
    targetName=UniversalIndentGUI # The targetname must be identical with the targetname set in the qmake project file.
    QMAKESPEC=${targetSystem}-g++
else
    targetName=universalindentgui # The targetname must be identical with the targetname set in the qmake project file.
fi
targetDir=${targetName}_$targetSystem

# Configuration
# -------------
version=0.8.0
doSVNUpdate=false
languages="de zh_TW ja_JP"

# Set QTDIR and QMAKESPEC for each platform
if [ "$targetSystem" = "win32" ] && [ ! -n "$2" ]; then
    QTDIR=/c/Programmierung/qt.4.3.2_gcc
else
    if [ "$targetSystem" = "macx" ] && [ ! -n "$2" ]; then
        QTDIR=/Users/thomas/Documents/Informatik/qt-static-release
    fi
fi

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

# wait a second for the old target dir to be really deleted
sleep 3

mkdir $targetDir &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Creating dir $targetDir failed!"
    exit 1
fi
mkdir $targetDir/indenters &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Creating dir indenters failed!"
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
mkdir $targetDir/config &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Creating dir config failed!"
    exit 1
fi
# In case of source files as target system, create additional dirs.
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


# Maybe do a SubVersion update.
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


echo "Copying ${targetName}$ext to target dir"
echo "--------------------------------------------"
cp ./release/$targetName$ext ./$targetDir/ &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy file \"$targetName$ext\"!"
    exit 1
fi
echo "Done"
echo ""


echo "Copying the indenter executable files to the target indenters dir"
echo "-----------------------------------------------------------------"
indenters="astyle$ext astyle.html bcpp$ext bcpp.txt csstidy$ext greatcode.exe greatcode.txt indent$ext indent.html tidy$ext tidy.html uncrustify$ext uncrustify.txt"
if [ "$ext" = ".exe" ]; then
    indenters="$indenters libiconv-2.dll libintl-2.dll"
fi
for i in $indenters
do
    cp ./indenters/$i ./$targetDir/indenters/ &> /dev/null
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
    lrelease ./translations/universalindent_$i.ts -qm ./translations/universalindent_$i.qm
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not create translation file \"universalindent_$i.qm\"!"
        exit 1
    fi
done
echo "Done"
echo ""


echo "Copying the translation binaries to the target translation dir"
echo "--------------------------------------------------------------"
cp ./translations/qt_de.qm ./$targetDir/translations/ &> /dev/null
cp ./translations/qt_ja_jp.qm ./$targetDir/translations/qt_ja_JP.qm &> /dev/null
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

###################### Steps to be done for all ########################

echo "Copying the script based indenters to the target indenters dir"
echo "--------------------------------------------------------------"
indenters="JsDecoder.js perltidy PerlTidyLib.pm shellindent.awk"
for i in $indenters
do
    cp ./indenters/$i ./$targetDir/indenters/ &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not copy file \"$i\"!"
        exit 1
    fi
done
echo "Done"
echo ""

echo "Copying the indenter example files to the target indenters dir"
echo "--------------------------------------------------------------"
cp ./indenters/example.* ./$targetDir/indenters/ &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy the example.* files!"
    exit 1
fi
echo "Done"
echo ""


echo "Copying the indenter uigui ini files to the target indenters dir"
echo "----------------------------------------------------------------"
cp ./indenters/uigui*.ini ./$targetDir/indenters/ &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy the uigui*.ini files!"
    exit 1
fi
echo "Done"
echo ""


echo "Copying the default syntax highlight ini file to the target config dir"
echo "----------------------------------------------------------------------"
cp ./config/UiGuiSyntaxHighlightConfig.ini ./$targetDir/config/ &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy the UiGuiSyntaxHighlightConfig.ini file!"
    exit 1
fi
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
docfiles="iniFileFormat.html"
if [ ! "$ext" = ".exe" ]; then
    indenters="$docfiles universalindentgui.man"
fi
for i in $docfiles
do
    cp ./doc/$i ./$targetDir/doc/ &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not copy file \"$i\"!"
        exit 1
    fi
done
echo "Done"
echo ""


echo "Packing the whole target dir"
echo "----------------------------"
if [ "$ext" = ".exe" ]; then
    zip -r9 ${targetName}_${version}_$targetSystem.zip $targetDir &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not create archive \"${targetName}_${version}_$targetSystem.zip\"!"
        exit 1
    fi
else
    tar czf ${targetName}_${version}_$targetSystem.tar.gz $targetDir &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not create archive \"${targetName}_${version}_$targetSystem.tar.gz\"!"
        exit 1
    fi
fi
echo "Done"
echo ""


echo "Everything completed successfully!"
#read -p "press any key to continue"
