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
        QHostAddress senderAddress;

        QByteArray buffer(socket->pendingDatagramSize(), 0);
        socket->readDatagram(buffer.data(), buffer.size(), &senderAddress);
        qDebug() << senderAddress.toString();

        //QDATASTREAM TEST
        QByteArray audioBlock;
        QString test;
        QDataStream in(&buffer, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_5_3);
        in >> test >> audioBlock;
        qDebug() << test;




        //

        output.writeData(audioBlock);
    }
}
