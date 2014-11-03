#include "audioinput.h"

AudioInput::AudioInput(QAudioDeviceInfo devinfo, QObject *parent) : QObject(parent)
{
    QAudioFormat format;
    format.setChannelCount(1);
    format.setSampleRate(8000);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    //error checking
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(format)) {
        qWarning() << "Default format not supported - trying to use nearest";
        format = info.nearestFormat(format);
    }

    audio = new QAudioInput(devinfo, format, this);
    device = audio->start();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(readAudio()));
    timer->start(10);
}

void AudioInput::readAudio()
{
    QByteArray data;
    //Check the number of samples in input buffer
    qint64 len = audio->bytesReady(); 

    if (len > 1024) {
        len = 1024;
    }
    //Read sound samples from input device to buffer
    if (len > 0) {
        data.resize(len);
        device->read(data.data(), len);
    }

    emit dataReady(data);
}
