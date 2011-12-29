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

CONFIG += debug_and_release

macx {
 # If using as framework qscintilla needs to be build with:
 # qmake -spec macx-g++ CONFIG+=sdk CONFIG+=x86_64 CONFIG+=x86 CONFIG+=lib_bundle qscintilla.pro && make && sudo make install
 #LIBS += -framework qscintilla2
 LIBS += -lqscintilla2
 ICON = resources/UniversalIndentGUI.icns
}
else {
 LIBS += -lqscintilla2
}

CONFIG(release, debug|release) {

win32:pipe2nul = ">NUL"
unix:pipe2nul = "&> /dev/null"
macx:pipe2nul = "&> /dev/null"


# Language file processing
##########################
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
message ( Updating universalindent_ja.ts )
system($${lupdate} src -ts ./translations/universalindent_ja.ts -silent)
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
system($${lrelease} ./translations/universalindent_ja.ts -qm ./translations/universalindent_ja.qm -silent)
system($${lrelease} ./translations/universalindent_ru.ts -qm ./translations/universalindent_ru.qm -silent)
system($${lrelease} ./translations/universalindent_uk.ts -qm ./translations/universalindent_uk.qm -silent)
system($${lrelease} ./translations/universalindent_zh_TW.ts -qm ./translations/universalindent_zh_TW.qm -silent)

# Copy Qts own translation files to the local translation directory
message ( Copy Qts own translation files to the local translation directory )
qtTranslationInstallDir = $$[QT_INSTALL_TRANSLATIONS]
win32:qtTranslationInstallDir = $$replace(qtTranslationInstallDir, /, \\)
unix:system(cp $${qtTranslationInstallDir}/qt_de.qm ./translations/ $$pipe2nul)
unix:system(cp $${qtTranslationInstallDir}/qt_fr.qm ./translations/ $$pipe2nul)
unix:system(cp $${qtTranslationInstallDir}/qt_ja.qm ./translations/ $$pipe2nul)
unix:system(cp $${qtTranslationInstallDir}/qt_ru.qm ./translations/ $$pipe2nul)
unix:system(cp $${qtTranslationInstallDir}/qt_uk.qm ./translations/ $$pipe2nul)
unix:system(cp $${qtTranslationInstallDir}/qt_zh_TW.qm ./translations/ $$pipe2nul)
win32:system(copy $${qtTranslationInstallDir}\\qt_de.qm .\\translations\\ /Y $$pipe2nul)
win32:system(copy $${qtTranslationInstallDir}\\qt_fr.qm .\\translations\\ /Y $$pipe2nul)
win32:system(copy $${qtTranslationInstallDir}\\qt_ja.qm .\\translations\\ /Y $$pipe2nul)
win32:system(copy $${qtTranslationInstallDir}\\qt_ru.qm .\\translations\\ /Y $$pipe2nul)
win32:system(copy $${qtTranslationInstallDir}\\qt_uk.qm .\\translations\\ /Y $$pipe2nul)
win32:system(copy $${qtTranslationInstallDir}\\qt_zh_TW.qm .\\translations\\ /Y $$pipe2nul)

# Defining files that shall be installed when calling "make install"
####################################################################
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
           src/UiGuiSettings.h \
           src/UiGuiSettingsDialog.h \
           src/UiGuiSystemInfo.h \
           src/UiGuiVersion.h \
           src/UpdateCheckDialog.h \
           src/debugging/TSLogger.h


FORMS += src/MainWindow.ui \
         src/ToolBarWidget.ui \
         src/UiGuiSettingsDialog.ui \
         src/AboutDialog.ui \
         src/UpdateCheckDialog.ui \
         src/debugging/TSLoggerDialog.ui

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
           src/UiGuiSettings.cpp \
           src/UiGuiSettingsDialog.cpp \
           src/UiGuiSystemInfo.cpp \
           src/UiGuiVersion.cpp \
           src/UpdateCheckDialog.cpp \
           src/debugging/TSLogger.cpp

RESOURCES += resources/Icons.qrc
RC_FILE    = resources/programicon.rc



#message(Creating symbolic links within target dir for debugging)
#macx:system(ln -s $$PWD/config ./debug/config)
#macx:system(ln -s $$PWD/indenters ./debug/indenters)
#macx:system(ln -s $$PWD/translations ./debug/translations)
#macx:system(ln -s $$PWD/config ./release/config)
#macx:system(ln -s $$PWD/indenters ./release/indenters)
#macx:system(ln -s $$PWD/translations ./release/translations)
