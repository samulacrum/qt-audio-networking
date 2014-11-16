#include "server.h"

/**
 * @brief Server::Server default constructor for the server.
 * @param clients pointer to the clients object we will read from when sending to clients.
 * @param parent the parent object.
 */
Server::Server(ClientList *clients, QObject *parent)
    : QObject(parent),
      socket(new QUdpSocket(this))
{
    broadcastStatus = ":broadcasting_no";
    listeningStatus = ":listening_no";

    clientList = clients;

    //initiate the broadcast timer
    broadcastTimer = new QTimer(this);
    connect (broadcastTimer, SIGNAL(timeout()), this, SLOT(sendUpdate()));
    broadcastTimer->start(200);

    //get our local IP
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)) {
            if(address.toString().section(".", -1, -1) != "1") {
                qDebug() << address.toString();
                serverIP = QHostAddress(address);
            }
        }
    }

    //initialise with default device
    input = new AudioInput(QAudioDeviceInfo::defaultInputDevice(), this);
    connect(input, SIGNAL(dataReady(QByteArray)), this, SLOT(writeDatagram(QByteArray)));
}

Server::~Server()
{
    delete socket;
    delete broadcastTimer;
    delete input;
    delete clientList;
}

/**
 * @brief Server::writeDatagram write audio datagram(s) to all clients (excluding this one).
 * @param data audio data to send.
 */
void Server::writeDatagram(QByteArray data)
{
    if (socket) {
        //package data into a data stream
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_3);
        out << QString("audio");
        out << data;

        //compress data before sending
        QByteArray compressed = qCompress(block);

        //send to all connected clients
        for (int i = 0; i < clientList->rowCount(); i++) {
            QHostAddress sendTo = clientList->getAddressAt(clientList->index(i, 0));
            if(sendTo != serverIP) {
                socket->writeDatagram(compressed, sendTo, UDP_PORT);
            }
        }
    }
}

/**
 * @brief Server::sendUpdate sends an update broadcast, informing clients of our status.
 */
void Server::sendUpdate()
{
    if (socket) {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_3);
        out << QString("update" + broadcastStatus + listeningStatus);
        out << QByteArray();

        //compress data before sending
        QByteArray compressed = qCompress(block);

        qDebug() << "update sent: " << socket->writeDatagram(compressed, QHostAddress::Broadcast, 8002);
    }
}

/**
 * @brief Server::updateBroadcast updates the outgoing broadcast message.
 * @param data string containing broadcast status update.
 */
void Server::updateBroadcast(QString data)
{
    //make changes to the control string here
    if(data.contains("broadcasting_yes")) {
        broadcastStatus = ":" + data;
    }
    if(data.contains("broadcasting_no")) {
        broadcastStatus = ":" + data;
    }
    if(data.contains("listening_yes")) {
        listeningStatus = ":" + data;
    }
    if(data.contains("listening_no")) {
        listeningStatus = ":" + data;
    }
}

/**
 * @brief Server::startAudioSend initiates the audio device.
 */
void Server::startAudioSend()
{
    input->startAudio();
}

/**
 * @brief Server::endAudioSend stops the audio device.
 */
void Server::endAudioSend()
{
    input->stopAudio();
}

/**
 * @brief Server::setVolume sets the volume of the input device.
 * @param volume the audio to set.
 */
void Server::setVolume(float volume)
{
    input->setVolume(volume);
}

/**
 * @brief Server::changeDevice changes the audio device.
 * @param devinfo the audio device to change too.
 */
void Server::changeDevice(QAudioDeviceInfo devinfo)
{
    input->changeDevice(devinfo);
}
