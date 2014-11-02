#ifndef CLIENT_H
#define CLIENT_H

#include <QtCore>
#include <QtNetwork>
#include "audiooutput.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);

signals:

public slots:

private slots:
    void readyRead();

private:
    QUdpSocket *socket;
    AudioOutput output;
};

#endif // CLIENT_H
