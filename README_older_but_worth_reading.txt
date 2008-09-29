UniversalIndentGUI
==================

This application is released under the GPL version 2. Read the file "LICENSE.GPL" in the same directory as this readme for more information or have a look at http://www.gnu.org/licenses/gpl.html!


1. About UniversalIndentGUI
---------------------------

The UniversalIndentGUI is a universal graphical user interface for source code respective text formatters, beautifiers and indenters. This is mainly achieved by a configuration file for each supported indenter.

One of the main features and the reason why this tool was (better is being) developed is to see how the indent parameters affect the source code style, directly while changing the parameters. It always bothered me to change one option of a tool like GNU Indent or GreatCode and have to run it to see what my code would look like afterward. Often the result was not what I expected from the parameters description. So these times are gone. Toggle a parameter and see what it does.


2. Features
-----------
- Supports the following indenters:
  * Artistic Styler
  * BCPP
  * Cobol Beautifier
  * CSS Tidy
  * GNU Indent
  * GreatCode
  * HIndent
  * HTB
  * JsDecoder
  * Perl Tidy
  * PHP Beautifier
  * phpCB
  * PHP Stylist
  * Ruby Beautify
  * Ruby Formatter
  * Shell Indent
  * (HTML) Tidy
  * Uncrustify

- By these indenters the following programming languages are supported:
  C, C++, C#, Cobol, CSS, D, HTML, JAVA, Javascript, ObjectiveC, Pawn, Perl, PHP, Ruby, Shellscript, VALA, XML and XSL.

- Directly see how the code/text is affected by a indenter parameter

- Load a source code file of your choice as test file

- Indenter settings can be saved into the indenters own config file format (in case of GreatCode this is for example the file "gc.cfg")

- Create shell/batch scripts which make it easy to format whole directories with the selected indenters settings. Also these scripts can be called from within every application.

- Not yet known indenters can be added by creating a ini file for them, that is used by UniversalIndentGUI (this file has the prefix "uigui_" and ends with ".ini"). Have a look at the file "doc/iniFileFormat.html" for explanations about the ini file format. If you want to extend an existing or create a new ini file for a new indenter, you are sincerely invited to do so and I would be thankful for telling me about it.


3. Starting UniversalIndentGUI
------------------------------

UniversalIndentGUI does not need any further libs or dlls if you got one of the binary archives, because it is linked statically. Thats the reason why the binary file is rather big.

These binary archives (I mean the zip and tgz files) contain all supported indenters except for phpCB, because it must be obtained with acceptance of its owners.

Right now the windows version of GreatCode is shipped along with the Linux version of UniversalIndentGUI, because I was unable to compile GreatCode under Linux. UniversalIndentGUI tries to start GreatCode via wine. So wine has to be installed in order to use GreatCode. This applies to any new indenter, where only a windows executable exists, too.

3.1 Parameters
UniversalIndentGUI can be started with different parameters. The mentioned server component is currently in a very early development state. Its intention is to open UiGUI for plugins that can communicate from for example any editor via TCP/IP with the indenting component. Later more about that.

The following parameters can be used:
    No parameters starts without server and full gui.
    -f filename --file filename   : Opens the by filename defined file on start.
    -p --plugin  : Run as plugin. Server will be startet with a simplified gui.
    -s --server  : Run as server only without gui.
    If -p and -s are set, -p will be used.

3.2 Portable mode
UniversalIndentGUI can be run in a portable mode, so it won't make any modifications on the system, except for the media it is started from. The things UniversalIndentGUI writes to the for example USB drive are the user settings of the application and the set parameters of the indenters. Further a temporary dir is used, to feed the indenters with input files and grab their output.
The portable mode is being used, if the sub dir "indenters" is found in the same dir as the executable. Thats the case for the before named binary archives.

3.3 Multiuser mode
Especially Unix based systems strictly make use of user restrictments, but also Windows can be used that way, even if many dislike that due to lack of comfort. To be able to run UniversalIndentGUI on multiuser systems the needed files are spread over different directories. On Unix based these are:
/usr/bin  for the binary.
/etc/UniversalIndentGUI  for global configuration and the essential indenter ini files.
/usr/share/man/man1  for the zipped man page.
~/.config/UniversalIndentGUI  for the users application and indenter settings.

Similar on Windows:
%ProgramFiles%\UniversalIndentGUI  for the binary and all other needed files, like global configuration and the essential indenter ini files.
%APPDATA%\UniversalIndentGUI  for the users application and indenter settings.

The multiuser mode is used by default, if no "indenters" sub dir is found in the same dir as the executable.


4. Compile UniversalIndentGUI from source
-----------------------------------------

The latest version of the project can be downloaded via subversion.
Server: https://universalindent.svn.sourceforge.net
Repository: svnroot/universalindent
Directory: trunk
As one path: https://universalindent.svn.sourceforge.net/svnroot/universalindent/trunk

The Qt Framework Version >=4.4.x with source code is needed for compiling. It is available at www.trolltech.com. Please look there for more information. And pay attention, that there exist a commercial and a free GPL version of the Qt framework.

The second needed library comes from the QScintilla project (http://www.riverbankcomputing.co.uk/qscintilla/). Since I am using Qt4 there is also need to use the Qt4 version of QScintilla which is called QScintilla v2. The latest version is allways available at http://www.riverbankcomputing.com/software/qscintilla/download and most of the time there is no problem using that snapshot.

For the Windows Visual Studio 2005 a solution file is included, named "UniversalIndentGUI.sln". Additional include and lib directories for Qt have to be set in the preferences. Have a look at the homepage "http://wiki.qgis.org/qgiswiki/Building_QT_4_with_Visual_C++_2005", where you will find a good guide how to make Qt's open source edition compileable with Visual Studio. Even though it is written for Qt 4.1.1 it also fits for the current version 4.x.

Instead of using Visual Studio the qmake project file can be used to compile on any platform. Just run "qmake" in the projects main directory (where the file "UniversalIndentGUI.pro" resides). After this run "make debug" or "make release". That should do it.

The make directive to install UniversalIndentGUI is right now only completed for Unix based systems and can be called by "sudo make install".

When compiling has successfully completed you also need to "compile" the existing language translation files in the directory "translations". This is done by calling "lrelease filename.ts -qm filename.qm". "filename" has to be replaced by the real existing filename.


5. How to integrate an indenter written in JavaScript
-----------------------------------------------------
Since version 0.8.0 it is possible to use indenters that are written in JavaScript. These are executed by UniversalIndentGUIs internal JavaScript interpreter.
To write an own indenter in JavaScript that can be executed by UniversalIndentGUI, the indenter must provide two variables, with the names "unformattedCode" and "formattedCode". The exact names are mandatory. The first one must not be defined by the indenter itself, but is created by UniversalIndentGUI and is the input variable for the indenter. When called, it contains the unformatted source code. The second variable must be defined in the indenter and is the variable that returns the formatted source code.
In case of the indenter JsDecoder, that is shipped along with UniversalIndentGUI, the important code lines at its end look like this:
var jsdecoder = new JsDecoder();
var formattedCode;

// Feed the indenter with unformatted code.
jsdecoder.s = unformattedCode;
// Get the formatted code from the indenter.
formattedCode = jsdecoder.decode();

return formattedCode;


6. Future plans
---------------
- As mentioned above a server mode is in preparation. The protocol that will be documented when its finished.

- Te whole architectue will be refactored so that the code viewing component can be flexible replaced. This is needed because I plan to integrate a diff view where the difference between original and formatted code is directly highlighted.


7. Some words at the end
------------------------

Feel happily invited to try out and comment UniversalIndentGUI. Good and bad critics are welcome, if they are helpful of course ;-)


8. Disclaimer
-------------

You may use this software on your own risk. I am not responsible for any system damage or loss of data. Respect the GPL!


So long,

   Thomas
