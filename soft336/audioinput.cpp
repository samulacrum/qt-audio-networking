#include "audioinput.h"

AudioInput::AudioInput(QAudioDeviceInfo devinfo, QObject *parent) : QObject(parent)
{
    format.setChannelCount(1);
    format.setSampleRate(SAMPLE_RATE);
    format.setSampleSize(SAMPLE_SIZE);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    startDevice(devinfo);
}

void AudioInput::changeDevice(QAudioDeviceInfo devinfo)
{
    audio->stop();
    delete audio;

    //device->close();
    //device->disconnect(this);
    startDevice(devinfo);
}

void AudioInput::startDevice(QAudioDeviceInfo devinfo)
{
    audio = new QAudioInput(devinfo, format, this);
    audio->setBufferSize(AUDIO_BUFFER_SIZE);
    device = audio->start();
    connect(device, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void AudioInput::readyRead()
{
    QByteArray buffer;

    //Check the number of samples in input buffer
    qint64 len = audio->bytesReady();
    qint64 l;

    if(len > AUDIO_BUFFER_SIZE)
        len = AUDIO_BUFFER_SIZE;

    //Read sound samples from input device to buffer
    if (len > 0) {
        buffer.resize(len);
        l = device->read(buffer.data(), len);
        //ensures we only send data if there is actual data to send
        if (l > 0) {
            emit dataReady(buffer);
        }
    }
}

void AudioInput::setVolume(float volume)
{
    audio->setVolume(volume);
}

