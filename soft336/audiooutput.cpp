#include "audiooutput.h"

AudioOutput::AudioOutput(QObject *parent) : QObject(parent)
{
    QAudioFormat format;
    format.setChannelCount(1);
    format.setSampleRate(SAMPLE_RATE);
    format.setSampleSize(SAMPLE_SIZE);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    audio = new QAudioOutput(format, this);
    audio->setBufferSize(AUDIO_BUFFER_SIZE);
    audio->setVolume(1.0);
    device = audio->start();
}

void AudioOutput::writeData(QByteArray data)
{
    qDebug() << "wrote: " << device->write(data.data(), data.size()) << " down from: " << data.size();
}

void AudioOutput::setVolume(float volume)
{
    qDebug() << "output volume set to:" << volume;
    audio->setVolume(volume);
}

