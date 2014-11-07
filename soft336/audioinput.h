#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QtCore>
#include <QtMultimedia>

#define AUDIO_BUFFER_SIZE 1024
#define SAMPLE_RATE 44100
#define SAMPLE_SIZE 16

#define MAX_AMPLITUDE 32768

class AudioInput : public QObject
{
    Q_OBJECT
public:
    explicit AudioInput(QAudioDeviceInfo devinfo, QObject *parent = 0);
    void setVolume(float volume);

signals:
    void dataReady(QByteArray data);

public slots:

private slots:
    void readyRead();

private:
    QAudioInput *audio;
    QIODevice *device;
};

#endif // AUDIOINPUT_H
