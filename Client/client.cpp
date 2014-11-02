#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    socket = new QUdpSocket(this);
    socket->bind(8002);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void Client::readyRead()
{
    while (socket->hasPendingDatagrams()) {
        QByteArray buffer(socket->pendingDatagramSize(), 0);
        socket->readDatagram(buffer.data(), buffer.size());

        output.writeData(buffer);
    }
}
