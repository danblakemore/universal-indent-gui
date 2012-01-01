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
#ifndef ABOUTDIALOGGRAPHICSVIEW_H
#define ABOUTDIALOGGRAPHICSVIEW_H

#include <QGraphicsView>

class AboutDialog;

class QTimeLine;
class QSplashScreen;


class AboutDialogGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    AboutDialogGraphicsView(AboutDialog *aboutDialog, QWidget *parentWindow = NULL);
    ~AboutDialogGraphicsView(void);

public slots:
    void show();
    void hide();

private slots:
    void updateStep(int step);
    void showAboutDialog();
    void hideReally();

private:
    AboutDialog *_aboutDialog;
    QGraphicsProxyWidget *_graphicsProxyWidget;
    QWidget *_parentWindow;
    QTimeLine *_timeLine;
    QSplashScreen *_aboutDialogAsSplashScreen;
    int _windowTitleBarWidth;
    int _windowPosOffset;
};

#endif // ABOUTDIALOGGRAPHICSVIEW_H
