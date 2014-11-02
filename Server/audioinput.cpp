#include "audioinput.h"

AudioInput::AudioInput(QAudioDeviceInfo devinfo, QObject *parent) : QObject(parent)
{
    QAudioFormat format;
    format.setChannelCount(1);
    format.setSampleRate(8000);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    audio = new QAudioInput(devinfo, format, this);
    audio->setBufferSize(1024);

    device = audio->start();

    connect(device, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void AudioInput::readyRead()
{
    /*
    QByteArray buffer(8192, 0);
    QDataStream stream(&buffer, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_3);

    qint64 len = audio->bytesReady();

    //Read sound samples from input device to buffer
    if (len > 0)
    {
        //data.resize(len);
        stream << device->read(len);
    }

    emit dataReady(buffer);*/

    QByteArray data;

    //Check the number of samples in input buffer
    qint64 len = audio->bytesReady(); 

    //Read sound samples from input device to buffer
    if (len > 0)
    {
        data.resize(len);
        device->read(data.data(), len);
    }

    qDebug() << data.size();

    emit dataReady(data);
}
