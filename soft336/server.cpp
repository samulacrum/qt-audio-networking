#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    //iniate udp server and client list
    socketUDP = new QUdpSocket(this);
    clientList = new ClientList(this);

    //initiate the broadcast timer
    broadcastTimer = new QTimer(this);
    connect (broadcastTimer, SIGNAL(timeout()), this, SLOT(sendBroadcast()));
    broadcastTimer->start(200);

    //get our IP address
    QNetworkConfigurationManager mgr;
    QNetworkConfiguration nconfig = mgr.defaultConfiguration();
    QNetworkSession session ( nconfig );
    QNetworkInterface ninter = session.interface();
    QList<QNetworkAddressEntry> laddr = ninter.addressEntries();
    qDebug() << "Server IP: " << laddr.at(1).ip() << endl;
    serverIP = laddr.at(1).ip();

    /*
    socketTCP = 0;
    serverTCP = new QTcpServer(this);
    serverTCP->listen(QHostAddress::Any, 8003);

    connect(serverTCP, SIGNAL(newConnection()), this, SLOT(acceptTCPConnection()));*/
}

void Server::writeDatagram(QByteArray data)
{
    if (socketUDP) {
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
                qDebug() << "audio sent: " << socketUDP->writeDatagram(compressed, sendTo, UDP_PORT) << " to: " << sendTo.toString();
            }
        }
    }
}
/*
void Server::acceptTCPConnection() {
    socketTCP = serverTCP->nextPendingConnection();
    connect(socketTCP, SIGNAL(disconnected()), socketTCP, SLOT(deleteLater()));
}
*/
void Server::sendBroadcast() {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << QString("broadcast");
    out << QByteArray();

    //compress data before sending
    QByteArray compressed = qCompress(block);

    qDebug() << "broadcast sent: " << socketUDP->writeDatagram(compressed, QHostAddress::Broadcast, 8002);
}

void Server::appendClient(QString client)
{
    clientList->appendClient(client);
}

void Server::startAudioSend()
{
    //initiate audio device
    input = new AudioInput(devinfo, this);
    connect(input, SIGNAL(dataReady(QByteArray)), this, SLOT(writeDatagram(QByteArray)));
}

void Server::endAudioSend()
{
    delete input;
}

void Server::setVolume(float volume)
{
    input->setVolume(volume);
}

void Server::changeDevice(QAudioDeviceInfo devinfo)
{
    this->devinfo = devinfo;
}
