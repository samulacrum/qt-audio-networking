#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore>
#include <QtNetwork>
#include <QTimer>
#include <QBuffer>
#include "audiooutput.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    void startListen();
    void endListen();

signals:
    void clientBroadcastReceived(QString clientAddress);
public slots:

private slots:
    void readDatagrams();

private:
    QUdpSocket *socket;
    AudioOutput *output;
    bool listen;
};

#endif // CLIENT_H
