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

#ifndef UIGUIERRORMESSAGE_H
#define UIGUIERRORMESSAGE_H

#include <QErrorMessage>

class QCheckBox;


class UiGuiErrorMessage : public QErrorMessage
{
    Q_OBJECT

public:
    UiGuiErrorMessage(QWidget *parent = 0);
    ~UiGuiErrorMessage(void);
    void showMessage( const QString &message );
    void showMessage( const QString &title, const QString &message );

private:
    QCheckBox *_showAgainCheckBox;
    QStringList _errorMessageList;
};

#endif // UIGUIERRORMESSAGE_H
