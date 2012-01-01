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

#include "AboutDialog.h"
#include "ui_AboutDialog.h"

#include "UiGuiVersion.h"

#include <QUrl>
#include <QDesktopServices>
#include <QScrollBar>
#include <QTimer>
#include <QLocale>

/*!
    \class AboutDialog
    \brief Displays a dialog window with information about UniversalIndentGUI
*/

/*!
    \brief The constructor calls the setup function for the ui created by uic and adds
    the GPL text to the text edit.
 */
AboutDialog::AboutDialog(QWidget *parent, Qt::WindowFlags flags) : QDialog(parent, flags)
	, _dialogForm(NULL)
	, _timer(NULL)
 {
    _dialogForm = new Ui::AboutDialog();
    _dialogForm->setupUi(this);

    _dialogForm->authorTextBrowser->setOpenExternalLinks( true );
    _dialogForm->creditsTextBrowser->setOpenExternalLinks( true );

    QString versionString = _dialogForm->versionTextBrowser->toHtml();
    versionString = versionString.arg(PROGRAM_VERSION_STRING).arg( UiGuiVersion::getBuildRevision() ).arg( UiGuiVersion::getBuildDate() );
    _dialogForm->versionTextBrowser->setHtml(versionString);

    _dialogForm->creditsTextBrowser->setHtml("<html><head></head><body>"
        "<pre> </br></pre>"
        "<h3 align='center'>Thanks go out to</h3>"
        "<p align='center'><a href=\"http://www.csie.nctu.edu.tw/~chtai/\"><b>Nelson Tai</b></a> for Chinese translation, good ideas and always fast answers.</p></br>"
        "<p align='center'><a href=\"http://www.hartwork.org/\"><b>Sebastian Pipping</b></a> for helping me bring UiGUI into the Debian repository and other good ideas.</p></br>"
        "<p align='center'><a href=\"http://korytskyy.lviv.ua/\"><b>Oleksandr</b></a> for Ukrainian and Russian translation.</p></br>"
        "<p align='center'><b>Erwan &quot;leg&quot;</b> for French translation and the icon logo.</p></br>"
        "<p align='center'>The <a href=\"http://www.scintilla.org/\"><b>Scintilla</b></a> project for their great text editing component.</p></br>"
        "<p align='center'><a href=\"http://www.riverbankcomputing.co.uk/software/qscintilla/\"><b>Riverbank</b></a> for their Scintilla Qt wrapper QScintilla.</p></br>"
        "<p align='center'>The <a href=\"http://astyle.sourceforge.net/\"><b>Artistic Style</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://invisible-island.net/bcpp/\"><b>BCPP</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://www.siber.com/sct/tools/cbl-beau.html\"><b>Cobol Beautifier</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://csstidy.sourceforge.net/\"><b>CSSTidy</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"ftp://ftp.ifremer.fr/ifremer/ditigo/fortran90/\"><b>Fortran 90 PPR</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://www.gnu.org/software/indent/\"><b>GNU Indent</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://sourceforge.net/projects/gcgreatcode/\"><b>GreatCode</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://linux.com.hk/penguin/man/1/hindent.html\"><b>hindent</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://www.digital-mines.com/htb/\"><b>HTB</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://tidy.sourceforge.net/\"><b>HTML Tidy</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://code.gosu.pl/\"><b>JsDecoder</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://jsppp.sourceforge.net/\"><b>JSPPP</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://perltidy.sourceforge.net/\"><b>Perltidy</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://beautifyphp.sourceforge.net/\"><b>PHP_Beautifier</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://www.waterproof.fr/products/phpCodeBeautifier/\"><b>phpCB</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://sourceforge.net/projects/phpstylist/\"><b>PHP Stylist</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://coverage.livinglogic.de/Tools/scripts/pindent.py.html\"><b>pindent</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://psti.equinoxbase.com/\"><b>Pl/Sql tidy</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://www.arachnoid.com/ruby/rubyBeautifier.html\"><b>Ruby Beautifier</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://raa.ruby-lang.org/project/ruby_formatter/\"><b>Ruby Formatter</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://www.bolthole.com/AWK.html\"><b>Shell Indent</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://uncrustify.sourceforge.net/\"><b>Uncrustify</b></a> project, specially <b>Ben Gardner</b>.</p></br>"
        "<p align='center'>The <a href=\"http://www.daansystems.com/vbsbeaut/\"><b>VBSBeautifier</b></a> project.</p></br>"
        "<p align='center'>The <a href=\"http://xmlindent.sourceforge.net/\"><b>XML Indent</b></a> project.</p></br>"
        "<p align='center'><b>Nirvash</b> for the initial Japanese translation.</p></br>"
        "<p align='center'>The <a href=\"http://tango.freedesktop.org/Tango_Desktop_Project/\"><b>Tango Project</b></a> for their icons.</p></br>"
        "<p align='center'><a href=\"http://www.famfamfam.com/\"><b>famfamfam</b></a> for the flag icons.</p></br>"
        "<p align='center'><a href=\"http://trolltech.com/\"><b>Trolltech</b></a> for their really great GUI framework <img src=\":/aboutDialog/qt_logo.png\"/>.</p></br>"
        "<h3 align='center'>My girlfriend (meanwhile also wife) for putting my head right and not sit all the time in front of my computer ;-)</h3>"
        "</body></html>");

    _scrollDirection = 1;
    _scrollSpeed = 100;
    _timer = new QTimer(this);
    connect( _timer, SIGNAL(timeout()), this, SLOT(scroll()) );
    connect( this, SIGNAL(accepted()), _timer, SLOT(stop()) );
}


/*!
    \brief Catches language change events and retranslates all needed widgets.
 */
void AboutDialog::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        _dialogForm->retranslateUi(this);

        QString versionString = _dialogForm->versionTextBrowser->toHtml();
        versionString = versionString.arg(PROGRAM_VERSION_STRING).arg( UiGuiVersion::getBuildRevision() ).arg( UiGuiVersion::getBuildDate() );
        _dialogForm->versionTextBrowser->setHtml(versionString);
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
    _timer->start(_scrollSpeed);
    return QDialog::exec();
}


/*!
    \brief This slot is called each _timer timeout to scroll the credits textbrowser.
    Also changes the scroll direction and speed when reaching the start or end.
 */
void AboutDialog::scroll() {
    QScrollBar *scrollBar = _dialogForm->creditsTextBrowser->verticalScrollBar();
    scrollBar->setValue( scrollBar->value()+_scrollDirection );

    if ( scrollBar->value() == scrollBar->maximum() ) {
        // Toggle scroll direction and change scroll speed;
        _scrollDirection = -1;
        _scrollSpeed = 5;
        _timer->stop();
        _timer->start(_scrollSpeed);
    }
    else if ( scrollBar->value() == scrollBar->minimum() ) {
        // Toggle scroll direction and change scroll speed;
        _scrollDirection = 1;
        _scrollSpeed = 100;
        _timer->stop();
        _timer->start(_scrollSpeed);
    }

    _dialogForm->creditsTextBrowser->update();
}


/*!
    \brief Shows the about dialog and also starts the credits scroller.
 */
void AboutDialog::show() {
    _timer->start(_scrollSpeed);
    QDialog::show();
}
