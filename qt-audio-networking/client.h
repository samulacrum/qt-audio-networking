#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore>
#include <QtNetwork>

#include "audiooutput.h"

#define UDP_PORT 8002
/**
 * @brief The Client class, responsible for reading and processing datagrams, and playing audio.
 */
class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();
    void startListen();
    void endListen();
    void setVolume(float volume);

signals:
    void clientBroadcastReceived(QString clientAddress, QString controlString);
public slots:

private slots:
    void readDatagrams();

private:
    AudioOutput *output;
    QUdpSocket *socket;
    bool listen;
};

#endif // CLIENT_H
