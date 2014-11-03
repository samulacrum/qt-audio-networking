#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QtCore>
#include <QObject>
#include <QAudioInput>
#include "server.h"

class AudioInput : public QObject
{
    Q_OBJECT
public:
    explicit AudioInput(QAudioDeviceInfo devinfo, QObject *parent = 0);

signals:
    void dataReady(QByteArray data);

public slots:

private slots:
    void readAudio();

private:
    QAudioInput *audio;
    QIODevice *device;
    QAudioFormat format;
    QByteArray buffer;
};

#endif // AUDIOINPUT_H
