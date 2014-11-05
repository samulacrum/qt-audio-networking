#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    socketUDP = new QUdpSocket(this);
    clientList = new ClientList(this);

    broadcastTimer = new QTimer(this);
    connect (broadcastTimer, SIGNAL(timeout()), this, SLOT(sendBroadcast()));
    broadcastTimer->start(1000);

    /*
    socketTCP = 0;
    serverTCP = new QTcpServer(this);
    serverTCP->listen(QHostAddress::Any, 8003);

    connect(serverTCP, SIGNAL(newConnection()), this, SLOT(acceptTCPConnection()));*/
}

void Server::writeDatagram(QByteArray data)
{
    if (socketUDP) {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_3);
        out << QString("audio");
        out << data;

        //compress data before sending
        QByteArray compressed = qCompress(block);
        //loop to send to all connected clients here

        for (int i = 0; i < clientList->rowCount(); i++) {
            QHostAddress sendTo = clientList->getAddressAt(clientList->index(i));
            //qDebug() << "audio sent: " << socketUDP->writeDatagram(compressed, QHostAddress::Broadcast, 8002);
            qDebug() << "audio sent: " << socketUDP->writeDatagram(compressed, sendTo, 8002) << " to: " << sendTo.toString();
        }
    }
}
/*
void Server::acceptTCPConnection() {
    socketTCP = serverTCP->nextPendingConnection();
    connect(socketTCP, SIGNAL(disconnected()), socketTCP, SLOT(deleteLater()));
}
*/
void Server::sendBroadcast() {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << QString("broadcast");
    out << QByteArray();

    //compress data before sending
    QByteArray compressed = qCompress(block);

    qDebug() << "broadcast sent: " << socketUDP->writeDatagram(compressed, QHostAddress::Broadcast, 8002);
}

void Server::appendClient(QString client)
{
    clientList->appendClient(client);
}
