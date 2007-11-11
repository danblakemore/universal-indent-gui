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

#include "aboutdialog.h"

/*!
    \class AboutDialog
    \brief Displays a dialog window with information about UniversalIndentGUI
*/

/*!
    \brief The constructor calls the setup function for the ui created by uic and adds
    the GPL text to the text edit.
 */
AboutDialog::AboutDialog(QWidget *parent, QString version, QString revision, QString buildDate) :
    QDialog(parent)
{
	qApp->setStyleSheet("QTextBrowser{background-color:transparent}");
    setupUi(this);
    creditsTextBrowser->setStyleSheet("QTextBrowser#creditsTextBrowser{background-color:white}");

    //licenseLabel->setText( "The System Locale is : " + QLocale::system().name() );
    connect( authorTextBrowser, SIGNAL(anchorClicked (const QUrl)), this, SLOT(linkClicked(const QUrl)) );
    textBrowserSavedContent = authorTextBrowser->toHtml();

    this->version = version;
    this->revision = revision;
    this->buildDate = buildDate;
    QString versionString = versionTextBrowser->toHtml();
    versionString = versionString.arg(version).arg(revision).arg(buildDate);
    versionTextBrowser->setHtml(versionString);

    creditsTextBrowser->setHtml("<html><head></head><body>"
        "<pre> </br></pre>"
        "<h3 align='center'>Thanks go out to</h3>"
        "<p align='center'><b>Nelson Tai</b> for chinese translation, good ideas and always fast answers.</p></br>"
        "<p align='center'><b>Riverbank</b> for their Scinitlla Qt wrapper QScintilla.</p></br>"
        "<p align='center'>The <b>Artistic Styler</b> project.</p></br>"
        "<p align='center'>The <b>BCPP</b> project.</p></br>"
        "<p align='center'>The <b>GNU Indent</b> project.</p></br>"
        "<p align='center'>The <b>GreatCode</b> project.</p></br>"
        "<p align='center'>The <b>HTML Tidy</b> project.</p></br>"
        "<p align='center'>The <b>Uncrustify</b> project, specially <b>Ben Gardner</b>.</p></br>"
        "<p align='center'><b>Nirvash</b> for the japanese translation.</p></br>"
        "<p align='center'>The <b>Tango Project</b> for their icons.</p></br>"
        "<p align='center'><b>famfamfam</b> for the flag icons.</p></br>"
        "<p align='center'><b>Trolltech</b> for their Qt.</p></br>"
        "<h3 align='center'>My girlfriend for putting my head right and not sit all the time in front of my computer ;-)</h3>"
        "</body></html>");

    scrollDirection = 1;
    scrollSpeed = 100;
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()), this, SLOT(scroll()) );
    connect( this, SIGNAL(accepted()), timer, SLOT(stop()) );
}


/*!
    \brief This slot is being called, when the link in the about dialog is being clicked.
 */
void AboutDialog::linkClicked(const QUrl &link) {
    authorTextBrowser->setHtml(textBrowserSavedContent);
    QDesktopServices::openUrl(link);
}


/*!
    \brief Catches language change events and retranslates all needed widgets.
 */
void AboutDialog::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi(this);

        QString versionString = versionTextBrowser->toHtml();
        versionString = versionString.arg(version).arg(revision).arg(buildDate);
        versionTextBrowser->setHtml(versionString);
    } 
    else {
        QWidget::changeEvent(event);
    }
}


/*!
    \brief Reimplements the dialog execution function to init the credits scroller.
 */
int AboutDialog::exec() {
    //creditsTextBrowser->verticalScrollBar()->setValue(0);
    timer->start(scrollSpeed);
    return QDialog::exec();
}


/*!
    \brief This slot is called each timer timeout to scroll the credits textbrowser.
    Also changes the scroll direction and speed when reaching the start or end.
 */
void AboutDialog::scroll() {
    QScrollBar *scrollBar = creditsTextBrowser->verticalScrollBar();
    scrollBar->setValue( scrollBar->value()+scrollDirection );

    if ( scrollBar->value() == scrollBar->maximum() ) {
        // Toggle scroll direction and change scroll speed;
        scrollDirection = -1;
        scrollSpeed = 5;
        timer->stop();
        timer->start(scrollSpeed);
    }
    else if ( scrollBar->value() == scrollBar->minimum() ) {
        // Toggle scroll direction and change scroll speed;
        scrollDirection = 1;
        scrollSpeed = 100;
        timer->stop();
        timer->start(scrollSpeed);
    }
}
