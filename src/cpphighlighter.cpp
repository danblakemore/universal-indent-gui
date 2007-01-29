/****************************************************************************
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtGui>

#include "cpphighlighter.h"

/*!
    \class CppHighlighter
    \brief Highlighter used by QTextEdit fields to syntax highlight cpp source code
*/

/*!
    The constructor initializes some regular expressions and keywords to identify cpp tokens
 */
CppHighlighter::CppHighlighter(QTextEdit *parent)
: QSyntaxHighlighter(parent)
{
    this->parent = parent;
	HighlightingRule rule;

    highlightningIsOn = true;

	keywordFormat.setForeground(Qt::darkBlue);
	keywordFormat.setFontWeight(QFont::Bold);
	QStringList keywordPatterns;
	keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
		<< "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
		<< "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
		<< "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
		<< "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
		<< "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
		<< "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
		<< "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
		<< "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
		<< "\\bvoid\\b" << "\\bvolatile\\b" << "\\belse\\b"
        << "\\bif\\b" << "\\bwhile\\b";
	foreach (QString pattern, keywordPatterns) {
		rule.pattern = QRegExp(pattern);
		rule.format = keywordFormat;
		highlightingRules.append(rule);
	}

	classFormat.setFontWeight(QFont::Bold);
	classFormat.setForeground(Qt::darkMagenta);
	rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
	rule.format = classFormat;
	highlightingRules.append(rule);

	singleLineCommentFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegExp("//[^\n]*");
	rule.format = singleLineCommentFormat;
	highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(Qt::darkGreen);

	quotationFormat.setForeground(Qt::darkRed);
	rule.pattern = QRegExp("\".*\"");
	rule.format = quotationFormat;
	highlightingRules.append(rule);

	//functionFormat.setFontItalic(true);
	functionFormat.setForeground(Qt::blue);
	rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
	rule.format = functionFormat;
	highlightingRules.append(rule);

	commentStartExpression = QRegExp("/\\*");
	commentEndExpression = QRegExp("\\*/");
}

/*!
    This function is automatically called by the connected QTextEdit object if needed. \a text 
    is a pointer to the QString which is parsed for keywords and other syntax elements to highlight.
    This function needs to be optimized because it scans the whole text for each keyword/RegExp
*/
void CppHighlighter::highlightBlock(const QString &text)
{
    if ( highlightningIsOn ) {
        foreach (HighlightingRule rule, highlightingRules) {
            QRegExp expression(rule.pattern);
            int index = text.indexOf(expression);
            while (index >= 0) {
                int length = expression.matchedLength();
                setFormat(index, length, rule.format);
                index = text.indexOf(expression, index + length);
            }
        }
        setCurrentBlockState(0);

        int startIndex = 0;
        if (previousBlockState() != 1)
            startIndex = text.indexOf(commentStartExpression);

        while (startIndex >= 0) {
            int endIndex = text.indexOf(commentEndExpression, startIndex);
            int commentLength;
            if (endIndex == -1) {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            } else {
                commentLength = endIndex - startIndex
                    + commentEndExpression.matchedLength();
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = text.indexOf(commentStartExpression,
                startIndex + commentLength);
        }
    }
}

/*!
    Turns the syntax parser on.
*/
void CppHighlighter::turnHighlightOn() {
    highlightningIsOn = true;
}

/*!
    Turns the syntax parser off.
*/
void CppHighlighter::turnHighlightOff() {
    highlightningIsOn = false;
/*  // Some tests on changing the style of the textedit for export with syntax highlight.
    this->parent->setHtml("<html>Thomas ist public toll!<br> Und ich auch.</html>");
    QString text = this->parent->toPlainText();
    QTextCursor cursor = this->parent->textCursor();
    QTextCharFormat fmt;
    fmt.setFontWeight(QFont::Light);
    fmt.setForeground(Qt::red);
    cursor.setCharFormat(fmt);
    this->parent->setCurrentCharFormat(fmt);

    foreach (HighlightingRule rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = text.indexOf(expression);
        while (index >= 0) {
            int length = expression.matchedLength();
            bool b = cursor.hasSelection();
            cursor.setPosition(index);
            cursor.setPosition(index+length, QTextCursor::KeepAnchor);
            b = cursor.hasSelection();
            int s = cursor.selectionStart();
            s = cursor.selectionEnd();
            cursor.mergeCharFormat(fmt);
            this->parent->setTextCursor(cursor);
            this->parent->mergeCurrentCharFormat(rule.format);
            index = text.indexOf(expression, index + length);
        }
    }
*/
}
