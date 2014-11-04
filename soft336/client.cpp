#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    socket = new QUdpSocket(this);
    socket->bind(8002);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readDatagrams()));
}

void Client::readDatagrams()
{
    while (socket->hasPendingDatagrams()) {
        QByteArray buffer2(socket->pendingDatagramSize(), 0);
        socket->readDatagram(buffer2.data(), buffer2.size());

        output.writeData(buffer2);
    }
}
