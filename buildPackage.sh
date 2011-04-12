#!/bin/sh

# Call this script to create the source code archive.
./buildRelease.sh

sourcedirname=universalindentgui-1.2.0
sourcefilename=$sourcedirname.tar.gz
sourcefilenameorig=universalindentgui_1.2.0.orig.tar.gz

targetDir=packaging

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


echo "Copying the original source files"
echo "---------------------------------"
cp ./$sourcefilename ./$targetDir/ &> /dev/null
if [ $? -gt 0 ]; then
echo "ERROR: Could not copy file!"
exit 1
fi
cp ./$sourcefilename ./$targetDir/$sourcefilenameorig &> /dev/null
if [ $? -gt 0 ]; then
echo "ERROR: Could not copy file!"
exit 1
fi
echo "Done"
echo ""

# wait until the copied files have been really created
while [ ! -f "./$targetDir/$sourcefilename" ]
do
    sleep 1
done
while [ ! -f "./$targetDir/$sourcefilenameorig" ]
do
    sleep 1
done

cd $targetDir

echo "Unpacking the source archive"
echo "----------------------------"
tar -xzvf $sourcefilenameorig
echo ""

# wait until the files have been really extracted
while [ ! -f "./$sourcedirname/src/UpdateCheckDialog.ui" ]
do
    sleep 1
done
while [ ! -f "./$sourcedirname/resources/universalIndentGUI.svg" ]
do
    sleep 1
done


echo "Copying the debian package build files"
echo "--------------------------------------"
cp -r ../debian ./$sourcedirname

cd $sourcedirname/debian
rm -Rfv .svn
echo ""

echo "Creating the debian source package (devscripts and quilt needed)"
echo "----------------------------------------------------------------"
debuild -S -us -uc
echo ""

echo "Creating the debian binary package for the current platform"
echo "-----------------------------------------------------------"
debuild -us -uc
echo ""

echo "Everything completed successfully!"
if [ "$WARNINGOCCURRED" = "true" ]; then
    echo "====> But there were some warnings. Please check that!"
fi
#read -p "press any key to continue"
