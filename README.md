# UniversalIndentGUI

Ever concerned about how your code looks like?

Ever heard of different indenting styles, for example K&amp;R?

Ever received code from someone else who didn't care about code formatting?

Ever tried to configure a code indenter to convert such code to your coding style?

Ever got bored by that tedious "changing a parameter"-"call the indeter"-"try and error" procedure?

Help is close to you. UniversalIndentGUI offers a live preview for setting the parameters of nearly any indenter. You change the value of a parameter and directly see how your reformatted code will look like. Save your beauty looking code or create an anywhere usable batch/shell script to reformat whole directories or just one file even out of the editor of your choice that supports external tool calls.

Many free available code beautifier, formatter and indenter are currently supported, like GNU Indent, Uncrustify, Artistic Styler, PHP Stylist, Ruby Beautify, HTML Tidy and many other (look at features for complete list). Currently not supported indenters can be easyly added by creating a configuration file for them.

Thus UniversalIndentGUI is open for nearly any new indenter and programming languages. Give it a try. Perhaps you'll also find an indenter for your programming language that you even didn't know that it exists.

### Features

*   Live Preview: change an indenter parameter and directly see how your formatted code will look like.
*   Support for nearly any existing indenter possible. Currently supported are:

![UiGUI Screenshot](doc/images/screenshot8.jpg)

*   [Artistic Styler](http://astyle.sourceforge.net/)
*   [BCPP](http://invisible-island.net/bcpp/)
*   [ClangFormat](https://releases.llvm.org/17.0.0/tools/clang/docs/ClangFormat.html)
*   [Cobol Beautify](http://www.siber.com/sct/tools/cbl-beau.html)
*   [CSSTidy](http://csstidy.sourceforge.net/)
*   [Fortran 90 PPR](ftp://ftp.ifremer.fr/ifremer/ditigo/fortran90/)
*   [GNU Indent](http://www.gnu.org/software/indent/)
*   [GreatCode](http://sourceforge.net/projects/gcgreatcode/)
*   [hindent](http://packages.debian.org/de/lenny/hindent)
*   [HTB](http://www.digital-mines.com/htb/)
*   [Javascript Decoder](http://code.gosu.pl/)
*   [JSPPP](http://jsppp.sourceforge.net/)
*   [Perl Tidy](http://perltidy.sourceforge.net/)
*   [PHP_Beautifier](http://pear.php.net/package/PHP_Beautifier)
*   [PHP Code Beautifier](http://www.waterproof.fr/products/phpCodeBeautifier/)
*   [PHP Stylist](http://sourceforge.net/projects/phpstylist/)
*   [pindent](http://coverage.livinglogic.de/Tools/scripts/pindent.py.html)
*   [Pl/Sql tidy](http://psti.equinoxbase.com/)
*   [Ruby Beautify](http://www.arachnoid.com/ruby/rubyBeautifier.html)
*   [Ruby Formatter](http://raa.ruby-lang.org/project/ruby_formatter/)
*   [Shell Indent](http://www.bolthole.com/AWK.html)
*   [(HTML) Tidy](http://tidy.sourceforge.net/)
*   [Uncrustify](http://uncrustify.sourceforge.net/)
*   [VBSBeautifier](http://www.daansystems.com/vbsbeaut/)
*   [XML Indent](http://xmlindent.sourceforge.net/)


Easy adding of new indenters: just create a parameter definition file for the new indenter.

Load and save different indenter configurations.

Reset to indenters default parameters.

By the above named indenters currently supported programming languages:

![UiGUI Screenshot](doc/images/screenshot6.jpg)

*   C, C++
*   C#
*   Cobol
*   CSS
*   D
*   Fortran
*   HTML
*   Java
*   JavaScript
*   JSP
*   Objective-C
*   Pawn
*   Perl
*   PHP
*   Pl/Sql
*   Python
*   Ruby
*   Shellscript
*   VALA
*   Visual Basic
*   XML
*   XSL

Syntax highlighting for all of these languages except for Pawn and VALA

Really easy to handle user interface.

Tooltips for each indenter parameter.

Creation of batch/shell scripts.

HTML and PDF export of your code.

PortableMode and MultiUserMode: In portable mode only uses its own subdirectories for temporary writing.

Multiple languages: English, German, Traditional Chinese, Russian, Ukrainian, partly Japanese.
Drag'n Drop of source code files.

Support for many different file encodings, like Korean KOI8-R or Chinese BIG5.

Possibility to edit your code while live preview is turned on. Yeah, thats really live! (but positions cursor wrong sometimes depending on the used indenter :-( )

Code completion.

Automatic update check. Does check only once a day and can be disabled.

A nice about dialog. Errrmm, ok beneath all the mean stuff this is somehow the programmers playground ;-)

Also a [Notepad++](http://notepad-plus.sourceforge.net/) plugin version is available.
The programming project for that is currently only available as Visual Studio C++ 2005 project file.

Also this plugin has some problems with its event handling, because it is running as a DLL inside of Notepad++ event loop.

This will be replaced with the upcoming UiGUI server functionality. See future plans for more about that.

### Supported and tested platforms

*   Windows 32 bit
*   Linux 32 and 64 bit
*   Mac OS X >= 10.4 (currently Intel only. PPC produced mysterious linker error)

### How to install / build UniversalIndentGUI

If you downloaded a complete binary package/archive for your system from SourceForge, you only need to unpack it and can run it out of the box. Also all free available indenters for your platform are included. Doing it that way,

UiGUI will run in portable mode.

But if you'd like to build UiGUI from source, follow these steps:

1.  Download, unpack, configure and compile
            [Qt](http://www.qtsoftware.com/downloads/opensource/appdev)
            >= 4.4.0. Make your QTDIR and QMAKESPEC settings. Or install Qt via a package manager.
2.  Download, unpack, compile and install
            [QScintilla](http://www.riverbankcomputing.com/software/qscintilla/download)
            >= 2.2.
3.  Checkout UiGUI: git clone git@github.com:danblakemore/universal-indent-gui.git  
    ~~svn co https://universalindent.svn.sourceforge.net/svnroot/universalindent/trunk universalindentgui~~
4.  In the checked out directory run "qmake UniversalIndentGUI.pro".
5.  Run "make release".
6.  Install it
        **Indenter binary packages** can be downloaded from the project at SourceForge
        [here](http://sourceforge.net/project/showfiles.php?group_id=167482&package_id=293094). **ClangFormat** is not part of them (yet), and you can download it separately at [the LLVM Builds page](http://llvm.org/builds/). (Rename it to `clang-format.exe` after downloading if needed.)

Beneath the possibility to build UiGUI using qmake, also project files for Visual Studio 2005
        and XCode are included.

### Used Qt techniques

This list shows some selected functionalities that Qt offers and that I use with UiGUI.

*   Translations are done with
        [QTranslator](http://doc.trolltech.com/4.4/qtranslator.html),
        [QLocale](http://doc.trolltech.com/4.4/qlocale.html) and Linguist.
*   File encodings are supported by using
        [QTextCodec](http://doc.trolltech.com/4.4/qtextcodec.html)
        and [QTextStream](http://doc.trolltech.com/4.4/qtextstream.html).
*   [QScriptEngine](http://doc.trolltech.com/4.4/qscriptengine.html)
        and [QScriptValue](http://doc.trolltech.com/4.4/qscriptvalue.html)
        is included for executing JavaScript files used as indenters.
*   [QGraphicsView](http://doc.trolltech.com/4.4/qgraphicsview.html)
        and [QGraphicsProxyWidget](http://doc.trolltech.com/4.4/qgraphicsproxywidget.html)
        for creating an animated 3D about dialog, simulating that is done on the whole desktop by using
        the screenshot capability of Qt.
*   Stylesheet settings give the about dialog a special look. Also gradients are used.
*   [QHttp](http://doc.trolltech.com/4.4/qhttp.html)
        and [QUrl](http://doc.trolltech.com/4.4/qurl.html)
        are used for the update check.
*   [QSettings](http://doc.trolltech.com/4.4/qsettings.html)
        is responsible for storing the application and syntax highlighter settings.
*   [QTcpServer](http://doc.trolltech.com/4.4/qtcpserver.html)
        and [QTcpSocket](http://doc.trolltech.com/4.4/qtcpsocket.html)
        build the base for the UiGUI Server.
*   Of course I use the Qt tools qmake, lupdate, lrelease, Designer, Linguist and my very best friend the Assistant.

### Future plans

*   Exchangeable source code view. Mainly adding a "Live Diff View" where you can see the unformatted
        code and the formatted one side by side with changes highlighted.
*   Bring functionality to the UiGUI server so that he can be run anywhere and a client plugin in any editor
        like Eclipse or Notepad++ can communicate with it. Thus the client plugin can be written in any language
        supporting TCP/IP connections and send to be formatted code to the server. Also some settings might be
        made via that conncection. Plans are going on, so stay tuned.
*   Batch/Multiple file and directory indenting, so that the user can throw a bunch of files or directories
        into a list and click on "Do it", so they all will get formatted.

### Thanks

Here I'd like to say "thank you" to all those guys, who helped me improving UiGUI. May it be doing some translations, creating packages for Linux, letting me know about bugs or ways to improve or just saying that they found my application helpful and that they like it. So:

**Thank you all out there!!**

### Disclaimer

You may use this software on your own risk. I am not responsible for any system damage or loss of data.

Respect the GPL! UiGUI is being released under [GPL 2](http://www.gnu.org/licenses/old-licenses/gpl-2.0.html). You will also find the license in the included file "LICENSE.GPL".