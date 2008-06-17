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


#include "UiguiIndentServer.h"

UiguiIndentServer::UiguiIndentServer(void) : QObject() {
    tcpServer = NULL;
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
}


void UiguiIndentServer::stopServer() {
    if ( tcpServer != NULL ) {
        tcpServer->close();
        delete tcpServer;
        tcpServer = NULL;
    }
}


void UiguiIndentServer::handleNewConnection() {
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect( clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()) );

    connect( clientConnection, SIGNAL(readyRead()), this, SLOT(handleReceivedData()) );
}


void UiguiIndentServer::handleReceivedData() {

    QTcpSocket *clientConnection = qobject_cast<QTcpSocket*>( sender() );
    quint16 blockSize = 0;
    QString receivedData = "";

    if ( clientConnection != NULL ) {
        QDataStream in(clientConnection);
        //in.setVersion(QDataStream::Qt_4_0);

        if ( blockSize == 0 ) {
            if ( clientConnection->bytesAvailable() < (int)sizeof(quint16) )
                return;

            in >> blockSize;
        }

        if ( clientConnection->bytesAvailable() < blockSize )
            return;

        unsigned int l = clientConnection->bytesAvailable();
        char *myData = new char[l];
        in.readRawData( myData, l);
        delete myData;
    }
}
