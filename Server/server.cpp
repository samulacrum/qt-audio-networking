#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    socket = new QUdpSocket(this);
}

void Server::writeData(QByteArray data)
{
    if (socket) {
        qDebug() << "SENT: " << socket->writeDatagram(data, QHostAddress::Broadcast, 8002);
    }
}
