#include "audioinput.h"

AudioInput::AudioInput(QAudioDeviceInfo devinfo, QObject *parent) : QObject(parent)
{
    QAudioFormat format;
    format.setChannelCount(1);
    format.setSampleRate(8000);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    audio = new QAudioInput(devinfo, format, this);
    audio->setBufferSize(320);
    device = audio->start();

    connect(device, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void AudioInput::readyRead()
{
    QByteArray buffer;

    //Check the number of samples in input buffer
    qint64 len = audio->bytesReady();
    qint64 l;

    if(len > 320)
        len = 320;

    //Read sound samples from input device to buffer
    if (len > 0) {
        buffer.resize(len);
        l = device->read(buffer.data(), len);
        if (l > 0) {
            emit dataReady(buffer);
        }
    }
}
