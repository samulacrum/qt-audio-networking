#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    listen = false;
    output = new AudioOutput(this);
    socket = new QUdpSocket(this);
    socket->bind(UDP_PORT);
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

        uncompressed = qUncompress(buffer);

        QDataStream in(&uncompressed, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_5_3);
        in >> controlString >> audioBlock;

        //if we have audio data write it to the output
        if(!controlString.compare("audio")) {
            if(listen) {
                output->writeData(audioBlock);
            }
        }
        else if (!controlString.compare("broadcast")) {
            qDebug() << "Received broadcast from: " << senderAddress.toString();
            emit clientBroadcastReceived(senderAddress.toString());
        }
    }
}

void Client::endListen()
{
    listen = false;
}

void Client::startListen()
{
    listen = true;
}

void Client::setVolume(float volume)
{
    output->setVolume(volume);
}

