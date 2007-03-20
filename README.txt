UniversalIndentGUI
==================

This application is released under the GPL. Read the file "LICENSE.GPL" in the same directory as this readme for more information!


1. About UniversalIndentGUI
---------------------------

The UniversalIndentGUI is a universal graphical user interface for source code, respective text, formatter, beautifier and indenter. This is mainly achieved by a configuration file for each supported indenter.

One of the main features and the reason why this tool was (better is being right now) developed is to see how the indent parameter affects the source code style directly while changing the parameters. It always bothered me to change one option of a tool like GNU Indent or GreatCode and have to run it to see what my code would look like afterward. Often the result was not what I expected from the parameters description. So these times are gone. Toggle a parameter and see what it does.


2. Features
-----------

- Directly see how the code/text is affected by a indenter parameter

- Load a source code file of your choice as test file

- Indenter settings can be saved into the indenters own config file format (in case of GreatCode this is for example the file "gc.cfg")

- Not yet known indenters can be added by creating a ini file for them, that is used by UniversalIndentGUI (this file has the prefix "uigui_" and ends with ".ini"). Have a look at the file "doc/iniFileFormat.html" for explanations about the ini file format. If you want to extend an existing or create a new ini file for a new indenter, you are sincerely invited to do so and I would be thankful for telling me about it.


3. Starting UniversalIndentGUI
------------------------------

UniversalIndentGUI does not need any further files, because it is linked to Qt statically. Thats why the executable is nearly 7 mb large. This avoids version conflicts with older installed Qt versions. Currently Qt 4.2.1 is being used.

Right now the windows version of GreatCode is shipped along with the Linux version of UniversalIndentGUI, because I was unable to compile GreatCode under Linux. UniversalIndentGUI tries to start GreatCode via wine. So wine has to be installed in order to use GreatCode. This applies to any new indenter, where only a windows executable exists, too.

Since I have no access to any other systems than windows XP and Linux (Ubuntu Edgy Eft), I would be thankful for info how to run it there.


4. Compile UniversalIndentGUI from source
-----------------------------------------

The latest version of the project can be downloaded via subversion.
Server: https://universalindent.svn.sourceforge.net
Repository: svnroot/universalindent
Directory: trunk
As one path: https://universalindent.svn.sourceforge.net/svnroot/universalindent/trunk

The Qt Framework Version >=4.2.x with source code is needed for compiling. It is available at www.trolltech.com. Please look there for more information. And pay attention, that there exist a commercial and a free GPL version of the Qt framework.

The second needed library comes from the QScintilla project (http://www.riverbankcomputing.co.uk/qscintilla/). Since I am using Qt4 there is also need to use the Qt4 version of QScintilla which is (or will be) called QScintilla v2 and is currently in beta state. The least needed version is the one released on 03.18.2007. The latest version is allways available at http://www.riverbankcomputing.com/Downloads/Snapshots/QScintilla2/.

For the Windows Visual Studio 2005 a solution file is included, named "UniversalIndentGUI.sln". Additional include and lib directories for Qt have to be set in the preferences. Have a look at the homepage "http://wiki.qgis.org/qgiswiki/Building_QT_4_with_Visual_C++_2005", where zou will find a good guide how to make Qt's open source edition compileabel with Visual Studio. Even though it is written for Qt 4.1.1 it also fits for the current version 4.2.3.

Instead of using Visual Studio the qmake project file can be used to compile on any platform. Just run "qmake" in the projects main directory (where the file "UniversalIndentGUI.pro" resides). After this run "make debug" or "make release". That should do it.

When compiling has successfully completed you also need to "compile" the existing language translation files in the directory "translations". This is done by calling "lrelease filename.ts -qm filename.qm". "filename" has to be replaced by the real existing filename.

In the same directory where the UniversalIndentGUI executable is called from, the directories "data" and "translations" have to exist (translations only if you wish to have a translation). Inside of the "date" directory the uigui ini files are placed along with the wanted indenter executables for your platform.


5. Some words at the end
------------------------

Feel happily invited to try out and comment UniversalIndentGUI. Good and bad critics are welcome, if they are helpful of course ;-)


6. Disclaimer
-------------

You may use this software on your own risk. I am not responsible for any system damage or loss of data. Respect the GPL!


So long,

   Thomas
