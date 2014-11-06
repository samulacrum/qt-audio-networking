#ifndef SERVER_H
#define SERVER_H

#include <QtCore>
#include <QtNetwork>
#include <QTimer>

#include "model.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

signals:

public slots:
    void writeDatagram(QByteArray data);
    void appendClient(QString client);

private slots:
    //void acceptTCPConnection();
    void sendBroadcast();

private:
    QUdpSocket *socketUDP;
    //QTcpSocket *socketTCP;
    //QTcpServer *serverTCP;
    QTimer *broadcastTimer;

public:
    ClientList *clientList;
    QHostAddress serverIP;
};

#endif // SERVER_H
