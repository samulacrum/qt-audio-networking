#include "client.h"

Client::Client(QString host, QObject *parent) : QObject(parent)
{
    socket = new QUdpSocket(this);
    socket->bind(8002);
    /*
    socket->connectToHost(host, 8002);*/
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void Client::readyRead()
{
    /*
    QByteArray data;

    while (socket->bytesAvailable() > 0)
        data.append(socket->readAll());*/

    while (socket->hasPendingDatagrams()) {
        QByteArray buffer(socket->pendingDatagramSize(), 0);
        socket->readDatagram(buffer.data(), buffer.size());

        output.writeData(buffer);
    }

}
