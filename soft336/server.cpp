#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    broadcastStatus = ":broadcasting_no";
    listeningStatus = ":listening_no";

    //iniate udp server and client list
    socketUDP = new QUdpSocket(this);
    clientList = new ClientList(this);

    //initiate the broadcast timer
    broadcastTimer = new QTimer(this);
    connect (broadcastTimer, SIGNAL(timeout()), this, SLOT(sendUpdate()));
    broadcastTimer->start(200);

    //get our IP address
    QNetworkConfigurationManager mgr;
    QNetworkConfiguration nconfig = mgr.defaultConfiguration();
    QNetworkSession session ( nconfig );
    QNetworkInterface ninter = session.interface();
    QList<QNetworkAddressEntry> laddr = ninter.addressEntries();
    qDebug() << "Server IP: " << laddr.at(1).ip() << endl;
    serverIP = laddr.at(1).ip();

    //this->processBroadcast(serverIP.toString());
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
        for (int i = 0; i < clientList->getSize(); i++) {
            QHostAddress sendTo = clientList->getAddressAt(clientList->index(i, 0));
            if(sendTo != serverIP) {
                qDebug() << "audio sent: " << socketUDP->writeDatagram(compressed, sendTo, UDP_PORT) << " to: " << sendTo.toString();
            }
        }
    }
}

void Server::sendUpdate()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << QString("update" + broadcastStatus + listeningStatus);
    out << QByteArray();

    //compress data before sending
    QByteArray compressed = qCompress(block);

    qDebug() << "update sent: " << socketUDP->writeDatagram(compressed, QHostAddress::Broadcast, 8002);
}

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

void Server::processBroadcast(QString address, QString controlString)
{
    clientList->processClient(address, controlString);
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
