#include "client.h"

Client::Client(QString host, QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    socket->connectToHost(host, 8002);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void Client::readyRead()
{
    QByteArray data;

    while (socket->bytesAvailable() > 0)
        data.append(socket->readAll());

    output.writeData(data);
}
