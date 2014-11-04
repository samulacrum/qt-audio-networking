#ifndef SERVER_H
#define SERVER_H

#include <QtCore>
#include <QtNetwork>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

signals:

public slots:
    void writeDatagram(QByteArray data);

private slots:
    void acceptTCPConnection();

private:
    QUdpSocket *socketUDP;
    QTcpSocket *socketTCP;
    QTcpServer * serverTCP;
};

#endif // SERVER_H
