CONFIG -= debug
CONFIG -= release
CONFIG += debug_and_release
TEMPLATE = app
QT += network
QT += script 

unix:TARGET = universalindentgui
win32:TARGET = UniversalIndentGUI
macx:TARGET = UniversalIndentGUI

DEPENDPATH += resources \
              src \
              debug \
              release

INCLUDEPATH += src

LIBS += -lqscintilla2

macx {
 CONFIG += x86 ppc sdk
 QMAKE-MAC_SDK = /Developer/SDKs/MacOSX10.4u.sdk
}


win32:pipe2nul = ">NUL"
unix:pipe2nul = "&> /dev/null"
macx:pipe2nul = "&> /dev/null"
message(Updating language files)

lupdate = lupdate
unix:lupdate = lupdate-qt4
lrelease = lrelease
unix:lrelease = lrelease-qt4
# Update translation files
system($${lupdate} src -ts ./translations/universalindent.ts -silent)
system($${lupdate} src -ts ./translations/universalindent_de.ts -silent)
system($${lupdate} src -ts ./translations/universalindent_zh_TW.ts -silent)
system($${lupdate} src -ts ./translations/universalindent_ja_JP.ts -silent)
system($${lupdate} src -ts ./translations/universalindent_ru.ts -silent)
system($${lupdate} src -ts ./translations/universalindent_uk.ts -silent)

# Create translation binaries
system($${lrelease} ./translations/universalindent_de.ts -qm ./translations/universalindent_de.qm -silent)
system($${lrelease} ./translations/universalindent_zh_TW.ts -qm ./translations/universalindent_zh_TW.qm -silent)
system($${lrelease} ./translations/universalindent_ja_JP.ts -qm ./translations/universalindent_ja_JP.qm -silent)
system($${lrelease} ./translations/universalindent_ru.ts -qm ./translations/universalindent_ru.qm -silent)
system($${lrelease} ./translations/universalindent_uk.ts -qm ./translations/universalindent_uk.qm -silent)

# Copy Qts own translation files to the local translation directory
qtTranslationInstallDir = $$[QT_INSTALL_TRANSLATIONS]
win32:qtTranslationInstallDir = $$replace(qtTranslationInstallDir, /, \)
unix:system(cp $${qtTranslationInstallDir}/qt_de.qm ./translations/ $$pipe2nul)
unix:system(cp $${qtTranslationInstallDir}/qt_ja_jp.qm ./translations/qt_ja_JP.qm $$pipe2nul)
unix:system(cp $${qtTranslationInstallDir}/qt_ru.qm ./translations/ $$pipe2nul)
unix:system(cp $${qtTranslationInstallDir}/qt_uk.qm ./translations/ $$pipe2nul)
win32:system(xcopy $${qtTranslationInstallDir}\qt_de.qm .\translations\ /Y $$pipe2nul)
win32:system(xcopy $${qtTranslationInstallDir}\qt_ja_jp.qm .\translations\qt_ja_JP.qm /Y $$pipe2nul)
win32:system(xcopy $${qtTranslationInstallDir}\qt_ru.qm .\translations\ /Y $$pipe2nul)
win32:system(xcopy $${qtTranslationInstallDir}\qt_uk.qm .\translations\ /Y $$pipe2nul)

# Defining files that shall be installed
########################################
# Create and install man page
exists( ./doc/universalindentgui.1* ) {
    unix:system(rm ./doc/universalindentgui.1*)
}
unix:system(cp ./doc/universalindentgui.man ./doc/universalindentgui.1)
unix:system(gzip ./doc/universalindentgui.1)
unix:documentation.path = /usr/share/man/man1
unix:documentation.files = doc/universalindentgui.1.gz

# Install indenter ini files, examples and some indenters
unix:indenters.path = /usr/share/universalindentgui/indenters
unix:indenters.files = indenters/uigui_*.ini
unix:indenters.files += indenters/example.*
unix:indenters.files += indenters/JsDecoder.js
unix:indenters.files += indenters/shellindent.awk

# Install translation files
unix:translation.path = /usr/share/universalindentgui/translations
unix:translation.files = translations/*.qm

# Install highlighter default config
unix:highlighterconfig.path = /usr/share/universalindentgui/config
unix:highlighterconfig.files = config/UiGuiSyntaxHighlightConfig.ini

# Install binary
unix:target.path = /usr/bin

# Set everything that shall be installed
unix:INSTALLS += target \
                 highlighterconfig \
                 indenters \
                 translation \
	             documentation



#######################
# remove linker flag "-mthreads" so the mingwm10.dll is no longer needed
#######################
#message(old flags:$${QMAKE_LFLAGS})
win32:parameters = $${QMAKE_LFLAGS}
newFlags =
for(parameter, parameters) {
    !contains(parameter, -mthreads) {
        newFlags += $${parameter}
    }
}
win32:QMAKE_LFLAGS = $${newFlags}
#message(new flags:$${QMAKE_LFLAGS})

win32:parameters = $${QMAKE_LFLAGS_EXCEPTIONS_ON}
newFlags =
for(parameter, parameters) {
    !contains(parameter, -mthreads) {
        newFlags += $${parameter}
    }
}
win32:QMAKE_LFLAGS_EXCEPTIONS_ON = $${newFlags}

win32:parameters = $${QMAKE_CXXFLAGS_EXCEPTIONS_ON}
newFlags =
for(parameter, parameters) {
    !contains(parameter, -mthreads) {
        newFlags += $${parameter}
    }
}
win32:QMAKE_CXXFLAGS_EXCEPTIONS_ON = $${newFlags}
#######################



CONFIG(debug, debug|release) {
    DESTDIR = ./debug
} else {
    DESTDIR = ./release
}

MOC_DIR = $${DESTDIR}/moc
UI_DIR = $${DESTDIR}/uic
OBJECTS_DIR = $${DESTDIR}/obj
RCC_DIR = $${DESTDIR}/qrc

#message ( destdir is $${DESTDIR}. uic is $${UI_DIR}. moc is $${MOC_DIR})

# Input
HEADERS += src/aboutdialog.h \
           src/highlighter.h \
           src/indenthandler.h \
           src/mainwindow.h \
           src/uiguierrormessage.h \
           src/uiguisettings.h \
           src/uiguisettingsdialog.h \
           src/updatecheckdialog.h

FORMS += src/indentgui.ui \
         src/toolBarWidget.ui \
         src/UiGuiSettingsDialog.ui \
         src/aboutdialog.ui \
         src/UpdateCheckDialog.ui

SOURCES += src/aboutdialog.cpp \
           src/highlighter.cpp \
           src/indenthandler.cpp \
           src/main.cpp \
           src/mainwindow.cpp \
           src/uiguierrormessage.cpp \
           src/uiguisettings.cpp \
           src/uiguisettingsdialog.cpp \
           src/updatecheckdialog.cpp

RESOURCES += resources/Icons.qrc
RC_FILE    = resources/programicon.rc
