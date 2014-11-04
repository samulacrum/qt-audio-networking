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
        QHostAddress senderAddress; //holds our sender address
        QByteArray audioBlock; //holds received audio
        QByteArray uncompressed; //holds our uncompressed datagram
        QString controlString; //holds received control string

        //read a datagram
        QByteArray buffer(socket->pendingDatagramSize(), 0);
        socket->readDatagram(buffer.data(), buffer.size(), &senderAddress);

        qDebug() << senderAddress.toString();

        uncompressed = qUncompress(buffer);

        QDataStream in(&uncompressed, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_5_3);
        in >> controlString >> audioBlock;

        //if we have audio data write it to the output
        if(!controlString.compare("audio")) {
            output.writeData(audioBlock);
        }
        else if (!controlString.compare("broadcast")){
            qDebug() << "Received broadcast from: " << senderAddress.toString();
            //add IP address to list of clients?
        }

        //send
        output.writeData(audioBlock);
    }
}
