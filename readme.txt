UniversalIndentGUI
==================

This application is released under the GPL. Read the file "license.gpl" in the same directory as this readme for more information!


1. About UniversalIndentGUI
---------------------------

The UniversalIndentGUI is supposed to be a universal graphical user interface for source code respective text formatter, beautifier and indenter. This is manly reached by a flexible ini file for each indenter.

One of the main features and the reason why this tool was (better is being right now) developed is to see how the indent parameter affects the source code style directly while changing the parameter. It always bothered me to change one option of a tool like GNU Indent or GreatCode and have to run it to see what my code would look like afterward. Often the result was not what I expected from the parameters description. So these times are gone.


2. Features (also planned features)
-----------------------------------

- Directly see how the code/text is affected by a indenter parameter

- Load a source code file of your choice as test file

- Indenter settings are directly saved into its very own config file (in case of GreatCode that is the file "gc.cfg")

- Not yet known indenters can be added by creating a ini file for it, that is used by UniversalIndentGUI (this file has the prefix "uigui_" and ends with ".ini"). Have a look at the file "iniFileFormat.txt" for explanations about the ini file format. If you want to extend or create a new ini file for a new indenter, you are sincerely invited to do so and I would be thankful for telling me.


3. Starting UniversalIndentGUI (needed files)
---------------------------------------------

The windows version does not need any further files, because it is linked to Qt statically. Thats why the executalbe is nearly 7 mb large.

Under Linux the files libQtCore.so and libQtGui.so are needed. These come along with the package "libqt4-core". Download and install this package with your preferred package manager.
Right now the windows version of GreatCode is shipped along with the Linux version of UniversalIndentGUI, because I was unable to compile GreatCode under Linux. UniversalIndentGUI tries to start GreatCode via wine. So wine has to be installed in order to use GreatCode.

Since I have no access to any other systems I would be thankful for info how to run it there.


4. Compile UniversalIndentGUI
-----------------------------

The latest version of the project can be downloaded via subversion.
Server: https://svn.sourceforge.net
Repository: svnroot/universalindent
Directory: trunk
As one path: https://svn.sourceforge.net/svnroot/universalindent/trunk

The Qt Framework Version >=4.x with source code is needed to compile. It is available at www.trolltech.com. Please look there for more info.

For the Windows Visual Studio 2005 a solution file is included, named "UniversalIndentGUI.sln". Additional include and lib directories for Qt have to be set in the preferences. Look at the trolltechs homepage or google for these settings right now. Later I will write them down here.

Alternatively the qmake project file can be used to compile on any platform. Just run "qmake" in the projects main directory (where the file "UniversalIndentGUI.pro" resides). After this run "make debug" or "make release". That should do it.
Also for ease you can run the batch file makeall.bat under windows if you use Visual Studio 2005. It also embeds the manifest file into the executable.

ATTENTION! If you have successfully compiled the project, you have to put Astyle.exe or GC.exe into the "data" directory in Order to run the program with normal functionality.


5. Some end words
-----------------

Feel happily invited to try out and comment UniversalIndentGUI. Good and bad critics are welcome, if they are helpful of course ;-)


6. Disclaimer
-------------

You may use this software on your own risk. I am not responsible for any system damage or loss of data. Respect the GPL!


So long,

   Thomas
