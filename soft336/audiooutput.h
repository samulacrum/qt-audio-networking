#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QtCore>
#include <QtMultimedia>

#define AUDIO_BUFFER_SIZE 1024
#define SAMPLE_RATE 44100
#define SAMPLE_SIZE 16

class AudioOutput : public QObject
{
    Q_OBJECT
public:
    explicit AudioOutput(QObject *parent = 0);
    void setVolume(float volume);

public slots:
    void writeData(QByteArray data);

private:
    QAudioOutput *audio;
    QIODevice *device;
};

#endif // AUDIOOUTPUT_H
