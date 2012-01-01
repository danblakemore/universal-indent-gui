#!/bin/sh

# Command line syntax:
# buildRelease [targetsystem qtdir] [--rebuild]
# Example:
# buildRelease win32 /c/qt.4.3.2
# Default values are targetsystem=src, qtdir=""
# if "--rebuild" is set the release directory

#TODO: Parse all input parameters to enable a flexible syntax.

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

# 3. param is for setting "complete rebuild" to true
if [ -n "$3" ] && [ "$3" = "--rebuild" ]; then
    COMPLETEREBUILD="true";
else
    COMPLETEREBUILD="false";
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
else
    targetName=universalindentgui # The targetname must be identical with the targetname set in the qmake project file.
fi


# Configuration
# -------------
#TODO: get version from source code file.
version=1.2.0
doSVNUpdate=false


if [ "$targetSystem" = "src" ]; then
    targetDir=${targetName}-$version
else
    targetDir=${targetName}_$targetSystem
fi

# Set QTDIR for each platform if not given as command line parameter.
#TODO: Check whether QTDIR is valid and qmake can be executed if QTDIR hasn't been set via command line.
if [ "$targetSystem" = "win32" ] && [ ! -n "$2" ]; then
    echo "The QTDIR has not been set via command line parameter!"
    exit 1
else
    if [ "$targetSystem" = "macx" ] && [ ! -n "$2" ]; then
        TEST=`qmake -v`
        if [ "$?" -ne "0" ]; then
            echo "The QTDIR has not been set via command line parameter!"
            exit 1
        fi
        QTDIR=`echo $TEST | sed 's/.* //'`
    fi
fi

QMAKESPEC=${targetSystem}-g++

echo "Making some environment settings"
echo "--------------------------------"
export QTDIR
PATH=$QTDIR/bin:$PATH
export PATH
export QMAKESPEC
echo "QTDIR = $QTDIR"
echo "QMAKESPEC = $QMAKESPEC"
echo "pwd = `pwd`"
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

# wait until the old target dir has been really deleted
while [ -d "$targetDir" ]
do
    sleep 1
done

mkdir $targetDir &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Creating dir $targetDir failed!"
    exit 1
fi

# wait until the new target dir has been really created
while [ ! -d "$targetDir" ]
do
    sleep 1
done

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
mkdir $targetDir/doc/images &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Creating dir doc/images failed!"
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
# In case of src as target system, create additional dirs.
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


###################### source release begin ########################
if [ "$targetSystem" = "src" ]; then


echo "Updating the translation files"
echo "------------------------------"
lupdate src -ts ./translations/universalindent.ts &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not update file \"universalindent.ts\"!"
    exit 1
fi
languages="de fr ja ru uk zh_TW"
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

echo "Copying the translation files to the target translation dir"
echo "-----------------------------------------------------------"
cp ./translations/universalindent*.ts ./$targetDir/translations/ &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy file \"universalindent.ts\"!"
    exit 1
fi
echo "Done"
echo ""


echo "Copying the resources and src files to the target dir"
echo "-----------------------------------------------------"
cp ./resources/* ./$targetDir/resources/ &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy dir \"resources\"!"
    exit 1
fi
cp -R ./src/* ./$targetDir/src/ &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy dir \"src\"!"
    exit 1
fi
# wait until the files have been really extracted
while [ ! -f "./$targetDir/src/UniversalIndentGUI_NPP/UniversalIndentGUI_NPPDialog.rc" ]
do
    sleep 1
done
rm -Rf ./$targetDir/src/UniversalIndentGUI_NPP &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not delete dir \"UniversalIndentGUI_NPP\"!"
    exit 1
fi
# Deleting backup files
rm ./$targetDir/src/*.*~ &> /dev/null
echo "Done"
echo ""


echo "Copying the indenter project files to the target dir"
echo "----------------------------------------------------"
projectfiles="UniversalIndentGUI.pro UniversalIndentGUI.sln VS8QtRules.rules UniversalIndentGUI.xcodeproj"
for i in $projectfiles
do
    cp -r ./$i ./$targetDir/ &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not copy file \"$i\"!"
        exit 1
    fi
done
echo "Done"
echo ""


echo "Copying man page to target dir"
echo "------------------------------"
cp ./doc/universalindentgui.man ./$targetDir/doc/ &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy file \"/doc/universalindentgui.man\"!"
    exit 1
fi
echo "Done"
echo ""

###################### source release end ########################
else
###################### binary release begin ########################

if [ -d "release" ] && [ "$COMPLETEREBUILD" = "true" ]; then
    echo "Cleaning up release dirs and deleting makefiles"
    echo "-----------------------------------------------"
    rm -r release &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not delete release dir!"
        exit 1
    fi
    rm Makefile* &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not delete the makefiles!"
        exit 1
    fi
    echo "Done"
    echo ""
fi

echo "Calling qmake"
echo "-------------"
qmake UniversalIndentGUI.pro
if [ $? -gt 0 ]; then
    echo "ERROR: Calling qmake failed!"
    exit 1
fi
echo "Done"
echo ""


echo "Calling make release"
echo "--------------------"
if [ -f "Makefile.Release" ]; then
    echo "calling \"make release --silent\""
    make release --silent
else
    echo "calling \"make --silent\""
    make --silent
fi
if [ $? -gt 0 ]; then
    echo "ERROR: Calling make release failed!"
    exit 1
fi
echo "Done"
echo ""

if [ "$targetSystem" = "macx" ]; then
    echo "Executing macdeployqt ./release/$targetName.app"
    echo "-----------------------------------------------"
    macdeployqt ./release/$targetName.app
    cp -rf $QTDIR/Resources/qt_menu.nib ./release/UniversalIndentGUI.app/Contents/Resources/
    echo "Done"
    echo ""
fi

echo "Copying ${targetName}$ext to target dir"
echo "--------------------------------------------"
if [ "$targetSystem" = "macx" ]; then
    cp -R ./release/$targetName.app ./$targetDir/ &> /dev/null
else
    cp ./release/$targetName$ext ./$targetDir/ &> /dev/null
fi

if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy file \"$targetName$ext\" \"./$targetDir/\"!"
    exit 1
fi

# Try to compress the executable with UPX.
if [ "$targetSystem" = "win32" ] || [ "$targetSystem" = "linux" ]; then
    echo "Trying to compress the executalbe using UPX."
    upx$ext --best ./$targetDir/$targetName$ext &> /dev/null
fi
if [ $? -gt 0 ]; then
    echo "Compressing the executable using UPX failed. Perhaps UPX doesn't exist."
else
    echo "Compressed the executable using UPX."
fi
echo "Done"
echo ""


echo "Copying the indenter executable files to the target indenters dir"
echo "-----------------------------------------------------------------"
# This list does not include script based indenters. These are copied somewhere below.
indenters="astyle$ext astyle.html uncrustify$ext uncrustify.txt xmlindent$ext xmlindent.txt"
# For win32 and Linux add some indenters that do not run or exist under MaxOSX
if [ "$targetSystem" = "win32" ] || [ "$targetSystem" = "linux" ]; then
    indenters="$indenters bcpp$ext bcpp.txt csstidy$ext f90ppr.exe f90ppr.txt greatcode.exe greatcode.txt htb.exe htb.html indent$ext indent.html phpCB.html phpCB.exe psti_license.txt psti_manual.html psti.exe tidy$ext tidy.html vbsbeaut_keywords_indent.txt vbsbeaut_keywords.txt vbsbeaut.bat vbsbeaut.exe"
fi
   

if [ "$ext" = ".exe" ]; then
    indenters="$indenters libiconv-2.dll libintl-2.dll"
fi
for i in $indenters
do
    cp ./indenters/$i ./$targetDir/indenters/ &> /dev/null
    if [ $? -gt 0 ]; then
        echo "WARNING: Could not copy indenter file \"$i\"!"
        WARNINGOCCURRED=true
    fi
done
echo "Done"
echo ""

if [ "$ext" = ".exe" ]; then
	echo "Copying the Qt Dlls to the target dir"
	echo "-------------------------------------"

    qtdlls="QtCore4.dll QtGui4.dll QtNetwork4.dll QtScript4.dll qscintilla2.dll"
    for i in $qtdlls
	do
	    cp $QTDIR/lib/$i ./$targetDir/ &> /dev/null
	    if [ $? -gt 0 ]; then
	        echo "WARNING: Could not copy Qt dll \"$i\"!"
	        WARNINGOCCURRED=true
	    fi
	done
	
	echo "Copying the mingw Dlls to the target dir"
	echo "----------------------------------------"

    qtdlls="mingwm10.dll libgcc_s_dw2-1.dll"
    for i in $qtdlls
	do
	    cp $QTDIR/../../../../mingw/bin/$i ./$targetDir/ &> /dev/null
	    if [ $? -gt 0 ]; then
	        echo "WARNING: Could not copy Qt dll \"$i\"!"
	        WARNINGOCCURRED=true
	    fi
	done
	echo "Done"
	echo ""
fi


echo "Copying the translation binaries to the target translation dir"
echo "--------------------------------------------------------------"
cp ./translations/qt_*.qm ./$targetDir/translations/ &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy file \"qt_*.qm\"!"
    exit 1
fi
cp ./translations/universalindent_*.qm ./$targetDir/translations/ &> /dev/null    
if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy file \"universalindent_*.qm\"!"
    exit 1
fi
echo "Done"
echo ""

fi
###################### binary release end ########################

###################### Steps to be done for all ########################

echo "Copying the script based indenters to the target indenters dir"
echo "--------------------------------------------------------------"
indenters="hindent hindent.html JsDecoder.js perltidy phpStylist.php phpStylist.txt pindent.py pindent.txt rbeautify.rb ruby_formatter.rb shellindent.awk"
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
otherfiles="CHANGELOG.txt LICENSE.GPL INSTALL.txt readme.html"
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
cp ./doc/images/* ./$targetDir/doc/images/ &> /dev/null
if [ $? -gt 0 ]; then
    echo "ERROR: Could not copy files from directory doc/images !"
    exit 1
fi
echo "Done"
echo ""


echo "Packing the whole target dir"
echo "----------------------------"
if [ "$ext" = ".exe" ]; then
	echo "Doing: zip -r9 ${targetName}_${version}_$targetSystem.zip $targetDir"
    zip -r9 ${targetName}_${version}_$targetSystem.zip $targetDir &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not create archive \"${targetName}_${version}_$targetSystem.zip\"!"
        exit 1
    fi
else
    if [ "$targetSystem" = "src" ]; then
        targetArchiveName=${targetName}-${version}.tar
    else
        targetArchiveName=${targetName}_${version}_$targetSystem.tar
    fi
    tar cf $targetArchiveName $targetDir &> /dev/null
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not create archive \"$targetArchiveName\"!"
        exit 1
    fi
    # wait until the tar file has been created
    while [ ! -f "$targetArchiveName" ]
    do
        sleep 1
    done
    gzip -9 -f $targetArchiveName
    if [ $? -gt 0 ]; then
        echo "ERROR: Could not create archive \"$targetArchiveName.gz\"!"
        exit 1
    fi
fi
echo "Done"
echo ""


echo "Everything completed successfully!"
if [ "$WARNINGOCCURRED" = "true" ]; then
    echo "====> But there were some warnings. Please check that!"
fi
#read -p "press any key to continue"
