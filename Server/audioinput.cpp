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

    //connect(device, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void AudioInput::readyRead()
{
    QByteArray data;

    //Check the number of samples in input buffer
    qint64 len = audio->bytesReady(); 

    //Read sound samples from input device to buffer
    if (len > 0) {
        data.resize(len);
        device->read(data.data(), len);
    }

    emit dataReady(data);
}
