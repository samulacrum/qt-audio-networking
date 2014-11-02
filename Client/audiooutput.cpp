#include "audiooutput.h"

AudioOutput::AudioOutput(QObject *parent) : QObject(parent)
{
    QAudioFormat format;
    format.setChannelCount(1);
    format.setSampleRate(16000);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    audio = new QAudioOutput(format, this);
    audio->setBufferSize(20000);

    device = audio->start();
}

void AudioOutput::writeData(QByteArray data)
{
    device->write(data.data(), data.size());
}
