#include "client.h"

/**
 * @brief Client::Client Default constructor for client.
 * @param parent the Parent Object
 */
Client::Client(QObject *parent)
    : QObject(parent),
      output(new AudioOutput(this)),
      socket(new QUdpSocket(this))
{
    listen = false;
    socket->bind(UDP_PORT);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readDatagrams()));
}

Client::~Client()
{
    delete socket;
    delete output;
}

/**
 * @brief Client::readDatagrams reads datagrams from the socket.
 */
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
        else if (controlString.contains("update")) {
            //qDebug() << "Received broadcast from: " << senderAddress.toString();
            emit clientBroadcastReceived(senderAddress.toString(), controlString);
        }
    }
}

/**
 * @brief Client::endListen sets listen bool to false; stops writing audio to the device.
 */
void Client::endListen()
{
    listen = false;
}

/**
 * @brief Client::startListen sets listen bool to true; starts writing audio to the device.
 */
void Client::startListen()
{
    listen = true;
}

/**
 * @brief Client::setVolume sets the volume of the device.
 * @param volume the volume to set.
 */
void Client::setVolume(float volume)
{
    output->setVolume(volume);
}

