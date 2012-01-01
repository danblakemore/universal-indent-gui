/***************************************************************************
 *   Copyright (C) 2006-2012 by Thomas Schweitzer                          *
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

class UiGuiErrorMessage;
class UiGuiIniFileParser;

class QMenu;
class QVBoxLayout;
class QLabel;
class QSpinBox;
class QComboBox;
class QCheckBox;
class QLineEdit;
class QToolButton;
class QToolBox;


class IndentHandler : public QWidget
{
    Q_OBJECT

public:
    IndentHandler(int indenterID, QWidget *mainWindow = NULL, QWidget *parent = NULL);
    ~IndentHandler();

    QString generateShellScript(const QString &configFilename);
    QString callIndenter(QString sourceCode, QString inputFileExtension);
    bool loadConfigFile(QString filePathName);
    void resetToDefaultValues();
    QStringList getAvailableIndenters();
    QString getPossibleIndenterFileExtensions();
    QString getParameterString();
    QString getIndenterCfgFile();
    QString getManual();
    void retranslateUi();
    QString getCurrentIndenterName();
    QMenu* getIndenterMenu();
    QList<QAction*> getIndenterMenuActions();
    void contextMenuEvent( QContextMenuEvent *event );
    void setParameterChangedCallback( void(*paramChangedCallback)(void) );
    void setWindowClosedCallback( void(*winClosedCallback)(void) );
    int getIndenterId();

signals:
    void indenterSettingsChanged();
    void selectedIndenterIndexChanged(int index);

protected:
    bool event( QEvent *event );
    void closeEvent(QCloseEvent *event);
    void wheelEvent( QWheelEvent *event );

private slots:
    void setIndenter(int indenterID);
    void showIndenterManual();
    void openConfigFileDialog();
    void saveasIndentCfgFileDialog();
    void createIndenterCallShellScript();
    void resetIndenterParameter();
    void handleChangedIndenterSettings();
    void updateDrawing();

private:
    QString callExecutableIndenter(QString sourceCode, QString inputFileExtension);
    QString callJavaScriptIndenter(QString sourceCode);
    void saveConfigFile(QString filePathName, QString parameterString);
    void readIndentIniFile(QString iniFilePath);
    bool createIndenterCallString();
    void initIndenterMenu();

    //! Holds a reference to all created pages of the parameter categories toolbox and the pages boxlayout
    struct IndenterParameterCategoryPage {
        QWidget *widget;
        QVBoxLayout *vboxLayout;
    };
    QVector<IndenterParameterCategoryPage> _indenterParameterCategoryPages;

    //! Holds a reference to all checkboxes needed for boolean parameter setting and the parameters name
    struct ParamBoolean {
        QString paramName;
        QString trueString;
        QString falseString;
        QCheckBox *checkBox;
    };
    QVector<ParamBoolean> _paramBooleans;

    //! Holds a reference to all line edits needed for parameter setting and the parameters name
    struct ParamString {
        QString paramName;
        QString paramCallName;
        QCheckBox *valueEnabledChkBox;
        QLineEdit *lineEdit;
        QLabel *label;
    };
    QVector<ParamString> _paramStrings;

    //! Hold a reference to all spin boxes needed for parameter setting and the parameters name
    struct ParamNumeric {
        QString paramName;
        QString paramCallName;
        QCheckBox *valueEnabledChkBox;
        QSpinBox *spinBox;
        QLabel *label;
    };
    QVector<ParamNumeric> _paramNumerics;

    //! Hold a reference to all combo boxes needed for parameter setting and the parameters name
    struct ParamMultiple {
        QString paramName;
        QString paramCallName;
        QCheckBox *valueEnabledChkBox;
        QComboBox *comboBox;
        QStringList choicesStrings;
        QStringList choicesStringsReadable;
    };
    QVector<ParamMultiple> _paramMultiples;

    QComboBox *_indenterSelectionCombobox;
    QToolButton *_indenterParameterHelpButton;
    //! Vertical layout box, into which the toolbox will be added
    QVBoxLayout *_toolBoxContainerLayout;
    QToolBox *_indenterParameterCategoriesToolBox;
    UiGuiIniFileParser *_indenterSettings;
    QStringList _indenterParameters;
    //! The indenters name in a descriptive form
    QString _indenterName;
    //! The indenters file name (w/o extension), that is being called
    QString _indenterFileName;
    QString _indenterDirctoryStr;
    QString _tempDirctoryStr;
    QString _settingsDirctoryStr;
    QStringList _indenterIniFileList;
    QString _parameterOrder;
    QString _globalConfigFilename;
    QString _cfgFileParameterEnding;
    QString _inputFileParameter;
    QString _inputFileName;
    QString _outputFileParameter;
    QString _outputFileName;
    QString _fileTypes;
    QString _useCfgFileParameter;
    QString _indenterShowHelpParameter;
    QWidget *_mainWindow;
    UiGuiErrorMessage *_errorMessageDialog;
    QString _indenterExecutableCallString;
    QString _indenterExecutableSuffix;

    QMenu *_menuIndenter;
    QAction *_actionLoadIndenterConfigFile;
    QAction *_actionSaveIndenterConfigFile;
    QAction *_actionCreateShellScript;
    QAction *_actionResetIndenterParameters;
    //! Needed for the NPP plugin.
    void(*_parameterChangedCallback)(void);
    //! Needed for the NPP plugin.
    void(*_windowClosedCallback)(void);

	//TODO: This function should go into a string helper/tool class/file.
    QString encodeToHTML(const QString &text);
};

#endif // INDENTHANDLER_H
