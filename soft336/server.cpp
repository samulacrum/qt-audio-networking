#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    socketUDP = new QUdpSocket(this);

    socketTCP = 0;
    serverTCP = new QTcpServer(this);
    serverTCP->listen(QHostAddress::Any, 8003);/*

    connect(serverTCP, SIGNAL(newConnection()), this, SLOT(acceptTCPConnection()));*/
}

void Server::writeDatagram(QByteArray data)
{
    if (socketUDP) {
        //QDATASTREAM TEST
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_3);
        out << QString("test");
        out << data; //not compressed when sent out
        //
        //compress data before sending
        QByteArray compressed = qCompress(data, 9);
        qDebug() << "SENT: " << socketUDP->writeDatagram(block, QHostAddress::Broadcast, 8002) << " down from:" << data.size();
    }
}

void Server::acceptTCPConnection() {
    socketTCP = serverTCP->nextPendingConnection();
    connect(socketTCP, SIGNAL(disconnected()), socketTCP, SLOT(deleteLater()));
}
