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

#include "UiGuiIndentServer.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QtDebug>

//! \defgroup grp_Server All concerning the server component.

/*!
    \class UiGuiIndentServer
    \ingroup grp_Server
    \brief UiGuiIndentServer is in such an early state, that even the communication
    protocol isn't completely planned. So this class lacks documentation until
    I really know where all this will lead to.

    The plan however is to have a server that receives commands for selecting an
    indenter and perhaps load some by the user predefined indenter config file. Then
    the client can send a text to it and will receive it formatted.
    The idea behind that is to make UiGUIs use as plugin or whatever more flexible.
    So the plugin is developed for Eclipse for example and it takes the client role,
    making it possible to use UiGUI from within Eclipse. Choosing a network protocol
    makes everything platform and programming language independent, so it doesn't
    matter for which application the plugin/client is developed.
*/

UiGuiIndentServer::UiGuiIndentServer(void) : QObject() {
    _tcpServer = NULL;
    _currentClientConnection = NULL;
    _readyForHandleRequest = false;
}


UiGuiIndentServer::~UiGuiIndentServer(void) {
}


void UiGuiIndentServer::startServer() {
    if ( _tcpServer == NULL ) {
        _tcpServer = new QTcpServer(this);
    }

    if ( !_tcpServer->isListening() ) {
        if ( !_tcpServer->listen(QHostAddress::Any, quint16(84484)) ) {
            QMessageBox::critical( NULL, tr("UiGUI Server"), tr("Unable to start the server: %1.").arg(_tcpServer->errorString()) );
            return;
        }
    }

    connect( _tcpServer, SIGNAL(newConnection()), this, SLOT(handleNewConnection()) );
    _readyForHandleRequest = true;
    _blockSize = 0;
}


void UiGuiIndentServer::stopServer() {
    if ( _tcpServer != NULL ) {
        _tcpServer->close();
        delete _tcpServer;
        _tcpServer = NULL;
    }
    _currentClientConnection = NULL;
    _readyForHandleRequest = false;
}


void UiGuiIndentServer::handleNewConnection() {
    QTcpSocket *clientConnection = _tcpServer->nextPendingConnection();
    connect( clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()) );

    connect( clientConnection, SIGNAL(readyRead()), this, SLOT(handleReceivedData()) );
}


void UiGuiIndentServer::handleReceivedData() {
    if ( !_readyForHandleRequest ) {
        return;
    }

    _currentClientConnection = qobject_cast<QTcpSocket*>( sender() );
    QString receivedData = "";

    if ( _currentClientConnection != NULL ) {
        QDataStream in(_currentClientConnection);
        in.setVersion(QDataStream::Qt_4_0);

        if ( _blockSize == 0 ) {
            if ( _currentClientConnection->bytesAvailable() < (int)sizeof(quint32) )
                return;

            in >> _blockSize;
        }

        if ( _currentClientConnection->bytesAvailable() < _blockSize )
            return;

        QString receivedMessage;
        in >> receivedMessage;

        _blockSize = 0;

        qDebug() << "receivedMessage: " << receivedMessage;

        if ( receivedMessage == "ts" ) {
            sendMessage("Toll");
        }
        else {
            sendMessage("irgendwas");
        }
    }
}


void UiGuiIndentServer::sendMessage( const QString &message ) {
    _readyForHandleRequest = false;

    _dataToSend = "";
    QDataStream out(&_dataToSend, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0;
    out << message;
    out.device()->seek(0);
    out << (quint32)(_dataToSend.size() - sizeof(quint32));

    connect(_currentClientConnection, SIGNAL(bytesWritten(qint64)), this, SLOT(checkIfReadyForHandleRequest()));
    _currentClientConnection->write(_dataToSend);
}


void UiGuiIndentServer::checkIfReadyForHandleRequest() {
    if ( _currentClientConnection->bytesToWrite() == 0 ) {
        QString dataToSendStr = _dataToSend.right( _dataToSend.size() - sizeof(quint32) );
        qDebug() << "checkIfReadyForHandleRequest _dataToSend was: " << dataToSendStr;
        disconnect(_currentClientConnection, SIGNAL(bytesWritten(qint64)), this, SLOT(checkIfReadyForHandleRequest()));
        _readyForHandleRequest = true;
    }
}
