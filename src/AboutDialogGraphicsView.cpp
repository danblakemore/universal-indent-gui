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

#include "AboutDialogGraphicsView.h"

#include "AboutDialog.h"

#include <QtGui>
#include <QDesktopWidget>
#include <QDate>
#include <QTimeLine>
#include <QSplashScreen>

/*!
    \class AboutDialogGraphicsView
    \brief A container for the real \a AboutDialog. Makes the 3D animation possible.

    The 3D animation shall suggest the user, that he is looking at his desktop, while
    this animation is done. Since this is not directly possible, \a AboutDialogGraphicsView
    when shown starts in frameless fullscreen mode with a screenshot of the desktop as background.
*/

/*!
    \brief The constructor initializes everything needed for the 3D animation.
 */
AboutDialogGraphicsView::AboutDialogGraphicsView(AboutDialog *aboutDialog, QWidget *parentWindow) : QGraphicsView(parentWindow)
	, _aboutDialog(NULL)
	, _graphicsProxyWidget(NULL)
	, _parentWindow(NULL)
	, _timeLine(NULL)
	, _aboutDialogAsSplashScreen(NULL)
{
    _parentWindow = parentWindow;
    setWindowFlags(Qt::SplashScreen);

#ifdef Q_OS_LINUX
    QRect availableGeometry = QApplication::desktop()->availableGeometry();
    QRect newGeometry = QRect( availableGeometry.x(), availableGeometry.y(), availableGeometry.width(), availableGeometry.height() );
#else
    QRect newGeometry = QRect( -1,-1, QApplication::desktop()->rect().width()+2, QApplication::desktop()->rect().height()+2 );
#endif
    setGeometry( newGeometry );

    _aboutDialog = aboutDialog;

    _windowTitleBarWidth = 0;
    _windowPosOffset = 0;

    QGraphicsScene *scene = new QGraphicsScene(this);
    setSceneRect( newGeometry );
    _aboutDialogAsSplashScreen = new QSplashScreen(this);
    _graphicsProxyWidget = scene->addWidget(_aboutDialogAsSplashScreen);
    _graphicsProxyWidget->setWindowFlags( Qt::ToolTip );

    setScene( scene );
    setRenderHint(QPainter::Antialiasing);

    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    connect(_aboutDialog, SIGNAL(finished(int)), this, SLOT(hide()));

    //setWindowOpacity(0.9);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setStyleSheet("AboutDialogGraphicsView { border: 0px; }");

    _timeLine = new QTimeLine(1000, this);
    _timeLine->setFrameRange(270, 0);
    //_timeLine->setUpdateInterval(10);
    //_timeLine->setCurveShape(QTimeLine::EaseInCurve);
    connect(_timeLine, SIGNAL(frameChanged(int)), this, SLOT(updateStep(int)));
}


AboutDialogGraphicsView::~AboutDialogGraphicsView(void) {
}


/*!
    \brief Grabs a screenshot of the full desktop and shows that as background. Above that background the
    AboutDialog 3D animation is shown. Also grabs the content of the AboutDialog itself.
 */
void AboutDialogGraphicsView::show() {
    // Because on X11 system the window decoration is only available after a widget has been shown once,
    // we can detect _windowTitleBarWidth here for the first time.
    _windowTitleBarWidth = _parentWindow->geometry().y() - _parentWindow->y();
    // If the _windowTitleBarWidth could not be determined, try it a second way. Even the chances are low to get good results.
    if ( _windowTitleBarWidth == 0 )
        _windowTitleBarWidth = _parentWindow->frameGeometry().height() - _parentWindow->geometry().height();
#ifdef Q_OS_LINUX
    if ( _windowTitleBarWidth == 0 ) {
        //TODO: 27 pixel is a fix value for the Ubuntu 10.4 default window theme and so just a workaround for that specific case.
        _windowPosOffset = 27;
        _windowTitleBarWidth = 27;
    }
#endif
    QPixmap originalPixmap = QPixmap::grabWindow(QApplication::desktop()->winId(), QApplication::desktop()->availableGeometry().x(), QApplication::desktop()->availableGeometry().y(), geometry().width(), geometry().height() );
    QBrush brush(originalPixmap);
    QTransform transform;
    transform.translate(0, QApplication::desktop()->availableGeometry().y());
    brush.setTransform(transform);

    setBackgroundBrush(brush);

    _aboutDialogAsSplashScreen->setPixmap( QPixmap::grabWidget(_aboutDialog) );
    _graphicsProxyWidget->setGeometry( _aboutDialog->geometry() );
    _aboutDialog->hide();
    _graphicsProxyWidget->setPos( _parentWindow->geometry().x()+(_parentWindow->geometry().width()-_graphicsProxyWidget->geometry().width()) / 2, _parentWindow->y()+_windowTitleBarWidth-_windowPosOffset);

    QRectF r = _graphicsProxyWidget->boundingRect();
    _graphicsProxyWidget->setTransform(QTransform()
        .translate(r.width() / 2, -_windowTitleBarWidth)
        .rotate(270, Qt::XAxis)
        //.rotate(90, Qt::YAxis)
        //.rotate(5, Qt::ZAxis)
        //.scale(1 + 1.5 * step, 1 + 1.5 * step)
        .translate(-r.width() / 2, _windowTitleBarWidth));

    _graphicsProxyWidget->show();
    //_aboutDialogAsSplashScreen->show();
    QGraphicsView::show();

    connect(_timeLine, SIGNAL(finished()), this, SLOT(showAboutDialog()));
    _timeLine->setDirection(QTimeLine::Forward);
    _timeLine->start();
}


/*!
    \brief Does the next calculation/transformation step.
 */
void AboutDialogGraphicsView::updateStep(int step) {
    QRectF r = _graphicsProxyWidget->boundingRect();
    _graphicsProxyWidget->setTransform(QTransform()
        .translate(r.width() / 2, -_windowTitleBarWidth)
        .rotate(step, Qt::XAxis)
        //.rotate(step, Qt::YAxis)
        //.rotate(step * 5, Qt::ZAxis)
        //.scale(1 + 1.5 * step, 1 + 1.5 * step)
        .translate(-r.width() / 2, _windowTitleBarWidth));
    //update();
}


/*!
    \brief Stops the 3D animation, moves the AboutDialog to the correct place and really shows it.
 */
void AboutDialogGraphicsView::showAboutDialog() {
    //hide();
    disconnect(_timeLine, SIGNAL(finished()), this, SLOT(showAboutDialog()));
    _aboutDialog->move( int(_parentWindow->geometry().x()+(_parentWindow->geometry().width()-_graphicsProxyWidget->geometry().width()) / 2), _parentWindow->y()+_windowTitleBarWidth-_windowPosOffset );
    _aboutDialog->exec();
}


/*!
    \brief Does not directly hide the AboutDialog but instead starts the "fade out" 3D animation.
 */
void AboutDialogGraphicsView::hide() {
    _graphicsProxyWidget->setPos( _parentWindow->geometry().x()+(_parentWindow->geometry().width()-_graphicsProxyWidget->geometry().width()) / 2, _parentWindow->y()+_windowTitleBarWidth-_windowPosOffset);

    QRectF r = _graphicsProxyWidget->boundingRect();
    _graphicsProxyWidget->setTransform(QTransform()
        .translate(r.width() / 2, -_windowTitleBarWidth)
        .rotate(0, Qt::XAxis)
        //.rotate(90, Qt::YAxis)
        //.rotate(5, Qt::ZAxis)
        //.scale(1 + 1.5 * step, 1 + 1.5 * step)
        .translate(-r.width() / 2, _windowTitleBarWidth));

    _graphicsProxyWidget->show();
    //_aboutDialogAsSplashScreen->show();
    QGraphicsView::show();

    connect(_timeLine, SIGNAL(finished()), this, SLOT(hideReally()));
    _timeLine->setDirection(QTimeLine::Backward);
    _timeLine->start();
}


/*!
    \brief This slot really hides this AboutDialog container.
 */
void AboutDialogGraphicsView::hideReally() {
    disconnect(_timeLine, SIGNAL(finished()), this, SLOT(hideReally()));
    QGraphicsView::hide();
    _parentWindow->activateWindow();
}
