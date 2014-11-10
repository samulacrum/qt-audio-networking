#ifndef SERVER_H
#define SERVER_H

#include <QtCore>
#include <QtNetwork>
#include <QtMultimedia>
#include <QTimer>
#include <QThread>

#include "audioinput.h"
#include "model.h"

#define UDP_PORT 8002

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(ClientList *clients, QObject *parent = 0);
    ~Server();
    void setVolume(float volume);

public slots:
    void writeDatagram(QByteArray data);
    void startAudioSend();
    void endAudioSend();
    void changeDevice(QAudioDeviceInfo devinfo);

private slots:
    void sendUpdate();
    void updateBroadcast(QString data);

private:
    QUdpSocket *socket;
    QTimer *broadcastTimer;
    AudioInput *input;
    QAudioDeviceInfo devinfo;
    QString broadcastStatus;
    QString listeningStatus;

public:
    ClientList *clientList;
    QHostAddress serverIP;
};

#endif // SERVER_H
