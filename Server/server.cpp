#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    socket = new QUdpSocket(this);
}

void Server::writeData(QByteArray data)
{
    if (socket) {
        QByteArray compressed = qCompress(data);
        qDebug() << "SENT: " << socket->writeDatagram(compressed, QHostAddress::Broadcast, 8002) << " down from:" << data.size();
    }
}
