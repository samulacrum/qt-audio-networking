#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    socket = new QUdpSocket(this);
}

void Server::writeData(QByteArray data)
{
    if (socket) {
        socket->writeDatagram(data, QHostAddress::Broadcast, 8002);
    }
}
