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

#include <QtDebug>

#include "UiguiIndentServer.h"

//! \defgroup grp_Server All concerning the server component.

/*!
    \class UiguiIndentServer
    \ingroup grp_Server
    \brief UiguiIndentServer is in such an early state, that even the communication
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

UiguiIndentServer::UiguiIndentServer(void) : QObject() {
    tcpServer = NULL;
    currentClientConnection = NULL;
    readyForHandleRequest = false;
}


UiguiIndentServer::~UiguiIndentServer(void) {
}


void UiguiIndentServer::startServer() {
    if ( tcpServer == NULL ) {
        tcpServer = new QTcpServer(this);
    }

    if ( !tcpServer->isListening() ) {
        if ( !tcpServer->listen(QHostAddress::Any, 84484) ) {
            QMessageBox::critical( NULL, tr("UiGUI Server"), tr("Unable to start the server: %1.").arg(tcpServer->errorString()) );
            return;
        }
    }

    connect( tcpServer, SIGNAL(newConnection()), this, SLOT(handleNewConnection()) );
    readyForHandleRequest = true;
    blockSize = 0;
}


void UiguiIndentServer::stopServer() {
    if ( tcpServer != NULL ) {
        tcpServer->close();
        delete tcpServer;
        tcpServer = NULL;
    }
    currentClientConnection = NULL;
    readyForHandleRequest = false;
}


void UiguiIndentServer::handleNewConnection() {
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect( clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()) );

    connect( clientConnection, SIGNAL(readyRead()), this, SLOT(handleReceivedData()) );
}


void UiguiIndentServer::handleReceivedData() {
    if ( !readyForHandleRequest ) {
        return;
    }

    currentClientConnection = qobject_cast<QTcpSocket*>( sender() );
    QString receivedData = "";

    if ( currentClientConnection != NULL ) {
        QDataStream in(currentClientConnection);
        in.setVersion(QDataStream::Qt_4_0);

        if ( blockSize == 0 ) {
            if ( currentClientConnection->bytesAvailable() < (int)sizeof(quint32) )
                return;

            in >> blockSize;
        }

        if ( currentClientConnection->bytesAvailable() < blockSize )
            return;

        QString receivedMessage;
        in >> receivedMessage;

        blockSize = 0;

        qDebug() << "receivedMessage: " << receivedMessage;

        if ( receivedMessage == "ts" ) {
            sendMessage("Toll");
        }
        else {
            sendMessage("irgendwas");
        }
    }
}


void UiguiIndentServer::sendMessage( const QString &message ) {
    readyForHandleRequest = false;

    dataToSend = "";
    QDataStream out(&dataToSend, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0;
    out << message;
    out.device()->seek(0);
    out << (quint32)(dataToSend.size() - sizeof(quint32));

    connect(currentClientConnection, SIGNAL(bytesWritten(qint64)), this, SLOT(checkIfReadyForHandleRequest()));
    currentClientConnection->write(dataToSend);
}


void UiguiIndentServer::checkIfReadyForHandleRequest() {
    if ( currentClientConnection->bytesToWrite() == 0 ) {
        QString dataToSendStr = dataToSend.right( dataToSend.size() - sizeof(quint32) );
        qDebug() << "checkIfReadyForHandleRequest dataToSend was: " << dataToSendStr;
        disconnect(currentClientConnection, SIGNAL(bytesWritten(qint64)), this, SLOT(checkIfReadyForHandleRequest()));
        readyForHandleRequest = true;
    }
}
