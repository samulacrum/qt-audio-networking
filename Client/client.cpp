#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    buffered = new QByteArray();

    socket = new QUdpSocket(this);
    socket->bind(8002);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readDatagrams()));
}

void Client::readDatagrams()
{
    while (socket->hasPendingDatagrams()) {
        QByteArray buffer2(socket->pendingDatagramSize(), 0);
        socket->readDatagram(buffer2.data(), buffer2.size());

        output.writeData(buffer2);/*

        qDebug() << buffer2.size();
        if(buffered->size() <= 4096) { //size of buffer
            buffered->append(buffer2);
            qDebug() << " BS:" << buffered->size();
        }
        else {
            test();
        }*/
    }
}

void Client::test()
{
    if (!buffered->isEmpty())
    {
        qDebug() << "BS IS: " << buffered->size();
        output.writeData(*buffered);
        buffered->clear();
    }
}
