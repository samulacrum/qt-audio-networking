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

signals:

public slots:

private slots:
    void readDatagrams();
    void test();

private:
    QUdpSocket *socket;
    AudioOutput output;
    QTimer *timer;
    QByteArray *buffered;
};

#endif // CLIENT_H
