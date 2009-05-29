TEMPLATE = lib
CONFIG += plugin
QT += script

TARGET = UniversalIndentGUI_NPP

DEFINES += \
           UNIVERSALINDENTGUI_NPP_EXPORTS
DEFINES -= \
           _UNICODE \
           UNICODE

DEPENDPATH += resources \
              src \
              src/UniversalIndentGUI_NPP \
              src/UniversalIndentGUI_NPP/NPPCommon \
              debug \
              release

INCLUDEPATH += src \
               src/UniversalIndentGUI_NPP \
               src/UniversalIndentGUI_NPP/NPPCommon

LIBS += -lshlwapi \

CONFIG(release, debug|release) {

pipe2nul = ">NUL"
message(Updating language files)

lupdate = lupdate
lrelease = lrelease
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
win32:system(copy $${qtTranslationInstallDir}\qt_de.qm .\translations\ /Y $$pipe2nul)
win32:system(copy $${qtTranslationInstallDir}\qt_fr.qm .\translations\ /Y $$pipe2nul)
win32:system(copy $${qtTranslationInstallDir}\qt_ja_jp.qm .\translations\ /Y $$pipe2nul)
win32:system(copy $${qtTranslationInstallDir}\qt_ru.qm .\translations\ /Y $$pipe2nul)
win32:system(copy $${qtTranslationInstallDir}\qt_uk.qm .\translations\ /Y $$pipe2nul)
win32:system(copy $${qtTranslationInstallDir}\qt_zh_TW.qm .\translations\ /Y $$pipe2nul)

}


CONFIG(debug, debug|release) {
    DESTDIR = ./debug_npp
} else {
    DESTDIR = ./release_npp
}

MOC_DIR = $${DESTDIR}/moc
UI_DIR = $${DESTDIR}/uic
OBJECTS_DIR = $${DESTDIR}/obj
RCC_DIR = $${DESTDIR}/qrc

#message ( destdir is $${DESTDIR}. uic is $${UI_DIR}. moc is $${MOC_DIR})

FORMS +=   src/UiGuiLoggerDialog.ui

# Input
HEADERS += \
#           src/AboutDialog.h \
#           src/AboutDialogGraphicsView.h \
           src/IndentHandler.h \
           src/SettingsPaths.h \
           src/TemplateBatchScript.h \
           src/UiGuiErrorMessage.h \
           src/UiGuiIniFileParser.h \
           src/UiGuiLogger.h \
           src/UiGuiSettings.h \
           src/UiGuiSystemInfo.h \
           src/UiGuiVersion.h \
           src/UniversalIndentGUI_NPP/UniversalIndentGUI_NPP.h \
           src/UniversalIndentGUI_NPP/NPPCommon/PluginInterface.h \
           src/UniversalIndentGUI_NPP/NPPCommon/Scintilla.h

SOURCES += \
#           src/AboutDialog.cpp \
#           src/AboutDialogGraphicsView.cpp \
           src/IndentHandler.cpp \
           src/SettingsPaths.cpp \
           src/TemplateBatchScript.cpp \
           src/UiGuiErrorMessage.cpp \
           src/UiGuiIniFileParser.cpp \
           src/UiGuiLogger.cpp \
           src/UiGuiSettings.cpp \
           src/UiGuiSystemInfo.cpp \
           src/UniversalIndentGUI_NPP/UniversalIndentGUI_NPP.cpp


RESOURCES += resources/Icons.qrc
RC_FILE    = resources/programicon.rc
