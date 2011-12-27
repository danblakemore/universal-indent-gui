#!/bin/sh
qmake -spec macx-xcode UniversalIndentGUI.pro

# Some notes on how to deploy on Mac using MacPorts Qt installation
# macdeployqt ./release/UniversalIndentGUI.app
# cp -rf /opt/local/lib/Resources/qt_menu.nib ./release/UniversalIndentGUI.app/Contents/Resources/