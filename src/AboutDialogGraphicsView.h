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
#ifndef ABOUTDIALOGGRAPHICSVIEW_H
#define ABOUTDIALOGGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QDesktopWidget>
#include <QDate>
#include <QTimeLine>
#include <QPixmap>

//Can't use this test, because MOC doesn't understand that code line
//#if QT_VERSION >= 0x040400

#include "aboutdialog.h"

class AboutDialogGraphicsView : public QGraphicsView
{
     Q_OBJECT
public:
    AboutDialogGraphicsView(AboutDialog *aboutDialog, QWidget *parent = 0);
    ~AboutDialogGraphicsView(void);
	void setScreenshotPixmap(const QPixmap &screenShot);

public slots:
    void show();
    void hide();

private:
    AboutDialog *aboutDialog;
    QGraphicsProxyWidget *graphicsProxyWidget;
    QGraphicsScene *scene;
    QWidget *parent;
    QTimeLine *timeLine;
    QLabel *aboutDialogAsLabel;
    int windowBorderWidth;
    int windowTitleBarWidth;
    bool firstRunOfAnimation;
	QPixmap originalPixmap;

private slots:
    void updateStep(int step);
    void showAboutDialog();
    void hideReally();
};

#endif // ABOUTDIALOGGRAPHICSVIEW_H

//#endif // #if QT_VERSION >= 0x040400