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

CONFIG += debug_and_release

macx {
 CONFIG += x86 ppc sdk
 QMAKE-MAC_SDK = /Developer/SDKs/MacOSX10.4u.sdk
 ICON = resources/UniversalIndentGUI.icns
}

CONFIG(release, debug|release) {

win32:pipe2nul = ">NUL"
unix:pipe2nul = "&> /dev/null"
macx:pipe2nul = "&> /dev/null"
message(Updating language files)

lupdate = lupdate
unix:lupdate = lupdate-qt4
macx:lupdate = lupdate
lrelease = lrelease
unix:lrelease = lrelease-qt4
macx:lrelease = lrelease
# Update translation files
message ( Updating universalindent.ts )
system($${lupdate} src -ts ./translations/universalindent.ts -silent)
message ( Updating universalindent_de.ts )
system($${lupdate} src -ts ./translations/universalindent_de.ts -silent)
message ( Updating universalindent_fr.ts )
system($${lupdate} src -ts ./translations/universalindent_fr.ts -silent)
message ( Updating universalindent_ja_jp.ts )
system($${lupdate} src -ts ./translations/universalindent_ja_jp.ts -silent)
message ( Updating universalindent_ru.ts )
system($${lupdate} src -ts ./translations/universalindent_ru.ts -silent)
message ( Updating universalindent_uk.ts )
system($${lupdate} src -ts ./translations/universalindent_uk.ts -silent)
message ( Updating universalindent_zh_TW.ts )
system($${lupdate} src -ts ./translations/universalindent_zh_TW.ts -silent)


# Create translation binaries
message ( Creating translation binaries )
system($${lrelease} ./translations/universalindent_de.ts -qm ./translations/universalindent_de.qm -silent)
system($${lrelease} ./translations/universalindent_fr.ts -qm ./translations/universalindent_fr.qm -silent)
system($${lrelease} ./translations/universalindent_ja_jp.ts -qm ./translations/universalindent_ja_jp.qm -silent)
system($${lrelease} ./translations/universalindent_ru.ts -qm ./translations/universalindent_ru.qm -silent)
system($${lrelease} ./translations/universalindent_uk.ts -qm ./translations/universalindent_uk.qm -silent)
system($${lrelease} ./translations/universalindent_zh_TW.ts -qm ./translations/universalindent_zh_TW.qm -silent)

# Copy Qts own translation files to the local translation directory
message ( Copy Qts own translation files to the local translation directory )
qtTranslationInstallDir = $$[QT_INSTALL_TRANSLATIONS]
win32:qtTranslationInstallDir = $$replace(qtTranslationInstallDir, /, \)
unix:system(cp $${qtTranslationInstallDir}/qt_de.qm ./translations/ $$pipe2nul)
unix:system(cp $${qtTranslationInstallDir}/qt_fr.qm ./translations/ $$pipe2nul)
unix:system(cp $${qtTranslationInstallDir}/qt_ja_jp.qm ./translations/ $$pipe2nul)
unix:system(cp $${qtTranslationInstallDir}/qt_ru.qm ./translations/ $$pipe2nul)
unix:system(cp $${qtTranslationInstallDir}/qt_uk.qm ./translations/ $$pipe2nul)
unix:system(cp $${qtTranslationInstallDir}/qt_zh_TW.qm ./translations/ $$pipe2nul)
win32:system(copy $${qtTranslationInstallDir}\qt_de.qm .\translations\ /Y $$pipe2nul)
win32:system(copy $${qtTranslationInstallDir}\qt_fr.qm .\translations\ /Y $$pipe2nul)
win32:system(copy $${qtTranslationInstallDir}\qt_ja_jp.qm .\translations\ /Y $$pipe2nul)
win32:system(copy $${qtTranslationInstallDir}\qt_ru.qm .\translations\ /Y $$pipe2nul)
win32:system(copy $${qtTranslationInstallDir}\qt_uk.qm .\translations\ /Y $$pipe2nul)
win32:system(copy $${qtTranslationInstallDir}\qt_zh_TW.qm .\translations\ /Y $$pipe2nul)

# Defining files that shall be installed
########################################
# Create and install man page
exists( ./doc/universalindentgui.1* ) {
    unix:system(rm ./doc/universalindentgui.1*)
}
unix:system(cp ./doc/universalindentgui.man ./doc/universalindentgui.1)
unix:system(gzip -9 ./doc/universalindentgui.1)
unix:documentation.path = /usr/share/man/man1
unix:documentation.files = doc/universalindentgui.1.gz

# Install indenter ini files, examples and some indenters
unix:indenters.path = /usr/share/universalindentgui/indenters
unix:indenters.files = indenters/uigui_*.ini
unix:indenters.files += indenters/example.*
unix:indenters.files += indenters/JsDecoder.js
unix:indenters.files += indenters/phpStylist.php
unix:indenters.files += indenters/phpStylist.txt
unix:indenters.files += indenters/pindent.py
unix:indenters.files += indenters/pindent.txt
unix:indenters.files += indenters/rbeautify.rb
unix:indenters.files += indenters/ruby_formatter.rb
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

}

#######################
# remove linker flag "-mthreads" so the mingwm10.dll is no longer needed
#######################
win32 {
    message ( remove linker flag "-mthreads" so the mingwm10.dll is no longer needed )
    #message(old flags:$${QMAKE_LFLAGS})
    parameters = $${QMAKE_LFLAGS}
    newFlags =
    for(parameter, parameters) {
        !contains(parameter, -mthreads) {
            newFlags += $${parameter}
        }
    }
    QMAKE_LFLAGS = $${newFlags}
    #message(new flags:$${QMAKE_LFLAGS})

    parameters = $${QMAKE_LFLAGS_EXCEPTIONS_ON}
    newFlags =
    for(parameter, parameters) {
        !contains(parameter, -mthreads) {
            newFlags += $${parameter}
        }
    }
    QMAKE_LFLAGS_EXCEPTIONS_ON = $${newFlags}

    parameters = $${QMAKE_CXXFLAGS_EXCEPTIONS_ON}
    newFlags =
    for(parameter, parameters) {
        !contains(parameter, -mthreads) {
            newFlags += $${parameter}
        }
    }
    QMAKE_CXXFLAGS_EXCEPTIONS_ON = $${newFlags}
}
#######################



CONFIG(debug, debug|release) {
    DESTDIR = ./debug
    DEFINES += _DEBUG DEBUG
} else {
    DESTDIR = ./release
}

MOC_DIR = $${DESTDIR}/moc
UI_DIR = $${DESTDIR}/uic
OBJECTS_DIR = $${DESTDIR}/obj
RCC_DIR = $${DESTDIR}/qrc

#message ( destdir is $${DESTDIR}. uic is $${UI_DIR}. moc is $${MOC_DIR})

# Input
HEADERS += src/AboutDialog.h \
           src/AboutDialogGraphicsView.h \
           src/IndentHandler.h \
           src/MainWindow.h \
           src/SettingsPaths.h \
           src/TemplateBatchScript.h \
           src/UiGuiErrorMessage.h \
           src/UiGuiHighlighter.h \
           src/UiGuiIndentServer.h \
           src/UiGuiIniFileParser.h \
           src/UiGuiLogger.h \
           src/UiGuiSettings.h \
           src/UiGuiSettingsDialog.h \
           src/UiGuiSystemInfo.h \
           src/UiGuiVersion.h \
           src/UpdateCheckDialog.h


FORMS += src/MainWindow.ui \
         src/ToolBarWidget.ui \
         src/UiGuiLoggerDialog.ui \
         src/UiGuiSettingsDialog.ui \
         src/AboutDialog.ui \
         src/UpdateCheckDialog.ui

SOURCES += src/AboutDialog.cpp \
           src/AboutDialogGraphicsView.cpp \
           src/IndentHandler.cpp \
           src/main.cpp \
           src/MainWindow.cpp \
           src/SettingsPaths.cpp \
           src/TemplateBatchScript.cpp \
           src/UiGuiErrorMessage.cpp \
           src/UiGuiHighlighter.cpp \
           src/UiGuiIndentServer.cpp \
           src/UiGuiIniFileParser.cpp \
           src/UiGuiLogger.cpp \
           src/UiGuiSettings.cpp \
           src/UiGuiSettingsDialog.cpp \
           src/UiGuiSystemInfo.cpp \
           src/UiGuiVersion.cpp \
           src/UpdateCheckDialog.cpp

RESOURCES += resources/Icons.qrc
RC_FILE    = resources/programicon.rc
