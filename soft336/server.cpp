#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    socketUDP = new QUdpSocket(this);
    clientList = new ClientList(this);

    broadcastTimer = new QTimer(this);
    connect (broadcastTimer, SIGNAL(timeout()), this, SLOT(sendBroadcast()));
    broadcastTimer->start(1000);

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
        //lowpass test
        /*
         * //Assign sound samples to short array
        short* resultingData = (short*)m_buffer.data();


        short *outdata=resultingData;
        outdata[ 0 ] = resultingData [ 0 ];

         int iIndex;
         if(ui->chkRemoveNoise->checkState() == Qt::Checked)
         {
                //Remove noise using Low Pass filter algortm[Simple algorithm used to remove noise]
                for ( iIndex=1; iIndex < len; iIndex++ )
                {
                    outdata[ iIndex ] = 0.333 * resultingData[iIndex ] + ( 1.0 - 0.333 ) * outdata[ iIndex-1 ];
                }
          }

         for ( iIndex=0; iIndex < len; iIndex++ )
         {
             //Cange volume to each integer data in a sample
             outdata[ iIndex ] = ApplyVolumeToSample( outdata[ iIndex ]);
         }

         //write modified sond sample to outputdevice for playback audio
          m_output->write((char*)outdata, len);
          */
        //end lowpass test


        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_3);
        out << QString("audio");
        out << data;

        //compress data before sending
        QByteArray compressed = qCompress(block);

        //send to all connected clients
        for (int i = 0; i < clientList->rowCount(); i++) {
            QHostAddress sendTo = clientList->getAddressAt(clientList->index(i));
            if(sendTo != serverIP) {
                qDebug() << "audio sent: " << socketUDP->writeDatagram(compressed, sendTo, 8002) << " to: " << sendTo.toString();
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
