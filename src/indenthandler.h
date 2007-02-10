/***************************************************************************
 *   Copyright (C) 2006-2007 by Thomas Schweitzer                          *
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


class IndentHandler : public QWidget
{
    Q_OBJECT

public:
	IndentHandler(QString dataDirPathStr, int indenterID, QMainWindow *mainWindow = 0, QWidget *parent = 0);

    //! Format source code with the currently selected indenter
    QString callIndenter(QString sourceCode, QString inputFileExtension);

	void loadConfigFile(QString filePathName);

    QStringList getAvailableIndenters();

	QString getPossibleIndenterFileExtensions();

    QString getParameterString();

    QString getIndenterCfgFile();

private:
    void writeConfigFile(QString parameterString);
    void readIndentIniFile(QString iniFilePath);

    //! Holds a reference to all created pages of the toolbox and the pages boxlayout
    struct ToolBoxPage
	{
        QWidget *page;
		QVBoxLayout *vboxLayout;
	};
	QVector<ToolBoxPage> toolBoxPages;

    //! Holds a reference to all checkboxes needed for boolean parameter setting and the parameters name
    struct ParamBoolean
	{
        QString paramName;
		QString trueString;
		QString falseString;
        QCheckBox *checkBox;
	};
	QVector<ParamBoolean> paramBooleans;

    //! Holds a reference to all lineedits needed for parameter setting and the parameters name
    struct ParamString
	{
        QString paramName;
		QString paramCallName;
        QCheckBox *valueEnabledChkBox;
        QLineEdit *lineEdit;
        QLabel *label;
	};
	QVector<ParamString> paramStrings;

    //! Hold a reference to all spinboxes needed for parameter setting and the parameters name
    struct ParamNumeric
	{
        QString paramName;
		QString paramCallName;
        QCheckBox *valueEnabledChkBox;
        QSpinBox *spinBox;
        QLabel *label;
	};
	QVector<ParamNumeric> paramNumerics;

    //! Hold a reference to all comboboxes needed for parameter setting and the parameters name
    struct ParamMultiple
	{
        QString paramName;
		QString paramCallName;
        QCheckBox *valueEnabledChkBox;
        QComboBox *comboBox;
        QStringList choicesStrings;
	};
	QVector<ParamMultiple> paramMultiples;

	QVBoxLayout *vboxLayout;
    QToolBox *toolBox;
    QSettings *indenterSettings;
    QStringList indenterParameters;
	QByteArray cfgFileData;
    //! The indenters name in a descriptive form
    QString indenterName;
    //! The indenters file name (w/o extension), that is being called
    QString indenterFileName;
    QString dataDirctoryStr;
    QStringList indenterIniFileList;
    QString configFilename;
    QString cfgFileParameterEnding;
    QString inputFileParameter;
    QString inputFileName;
    QString outputFileParameter;
	QString outputFileName;
    QString fileTypes;
    QString useCfgFileParameter;
    bool noIndenterExecExistDialogAlreadyShown;
    QMainWindow *mainWindow;

public slots:
    void setIndenter(int indenterID);

signals:
    void settingsCodeChanged();

};

#endif // INDENTHANDLER_H
