/***************************************************************************
 *   Copyright (C) 2006-2008 by Thomas Schweitzer                          *
 *   thomas-schweitzer(at)arcor.de                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 2.0 as   *
 *   published by the Free Software Foundation.                            *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program in the file LICENSE.GPL; if not, write to the *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef INDENTHANDLER_H
#define INDENTHANDLER_H

#include <QWidget>
#include <QToolBox>
#include <QVBoxLayout>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QToolButton>
#include <QFile>
#include <QProcess>
#include <QSettings>
#include <QStringList>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QByteArray>
#include <QDir>
#include <QMessageBox>
#include <QMainWindow>
#include <QTextStream>
#include <QTextCodec>
#include <QtScript>
#include <QDesktopServices>

#include "uiguierrormessage.h"
#include "templateBatchScript.h"
#include "uiguiIniFileParser.h"


class IndentHandler : public QWidget
{
    Q_OBJECT

public:
    IndentHandler(QString indenterDirPathStr, QString settingsDirPathStr, QString tempDirPathStr, int indenterID, QMainWindow *mainWindow = 0, QWidget *parent = 0);
    ~IndentHandler();

    QString generateCommandlineCall(QString inputFileExtension);
    QString callIndenter(QString sourceCode, QString inputFileExtension);
    void loadConfigFile(QString filePathName);
    QStringList getAvailableIndenters();
    QString getPossibleIndenterFileExtensions();
    QString getParameterString();
    QString getIndenterCfgFile();
    QString getManual();
    void retranslateUi();
    QString getCurrentIndenterName();

signals:
    void indenterSettingsChanged();
    
private slots:
    void setIndenter(int indenterID);
    void showIndenterManual();

private:
    QString callExecutableIndenter(QString sourceCode, QString inputFileExtension);
    QString callJavaScriptIndenter(QString sourceCode, QString inputFileExtension);
    void writeConfigFile(QString filePathName, QString parameterString);
    void readIndentIniFile(QString iniFilePath);
    bool createIndenterCallString();

    //! Holds a reference to all created pages of the toolbox and the pages boxlayout
    struct ToolBoxPage {
        QWidget *page;
        QVBoxLayout *vboxLayout;
    };
    QVector<ToolBoxPage> toolBoxPages;

    //! Holds a reference to all checkboxes needed for boolean parameter setting and the parameters name
    struct ParamBoolean {
        QString paramName;
        QString trueString;
        QString falseString;
        QCheckBox *checkBox;
    };
    QVector<ParamBoolean> paramBooleans;

    //! Holds a reference to all line edits needed for parameter setting and the parameters name
    struct ParamString {
        QString paramName;
        QString paramCallName;
        QCheckBox *valueEnabledChkBox;
        QLineEdit *lineEdit;
        QLabel *label;
    };
    QVector<ParamString> paramStrings;

    //! Hold a reference to all spin boxes needed for parameter setting and the parameters name
    struct ParamNumeric {
        QString paramName;
        QString paramCallName;
        QCheckBox *valueEnabledChkBox;
        QSpinBox *spinBox;
        QLabel *label;
    };
    QVector<ParamNumeric> paramNumerics;

    //! Hold a reference to all combo boxes needed for parameter setting and the parameters name
    struct ParamMultiple {
        QString paramName;
        QString paramCallName;
        QCheckBox *valueEnabledChkBox;
        QComboBox *comboBox;
        QStringList choicesStrings;
		QStringList choicesStringsReadable;
    };
    QVector<ParamMultiple> paramMultiples;

    QComboBox *indenterSelectionComboBox;
    QToolButton *indenterParameterHelpButton;
    QVBoxLayout *vboxLayout;
    QToolBox *toolBox;
    UiguiIniFileParser *indenterSettings;
    QStringList indenterParameters;
    QString cfgFileData;
    //! The indenters name in a descriptive form
    QString indenterName;
    //! The indenters file name (w/o extension), that is being called
    QString indenterFileName;
    QString indenterDirctoryStr;
    QString tempDirctoryStr;
    QString settingsDirctoryStr;
    QStringList indenterIniFileList;
	QString parameterOrder;
    QString configFilename;
    QString cfgFileParameterEnding;
    QString inputFileParameter;
    QString inputFileName;
    QString outputFileParameter;
    QString outputFileName;
    QString fileTypes;
    QString useCfgFileParameter;
    QString indenterShowHelpParameter;
    QMainWindow *mainWindow;
    UiGuiErrorMessage *errorMessageDialog;
    QString indenterExecutableCallString;
    QString indenterExecutableSuffix;
};

#endif // INDENTHANDLER_H
