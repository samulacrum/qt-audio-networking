#include "audiooutput.h"

AudioOutput::AudioOutput(QObject *parent) : QObject(parent)
{
    QAudioFormat format;
    format.setChannelCount(1);
    format.setSampleRate(44100);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    audio = new QAudioOutput(format, this);
    device = audio->start();
}

void AudioOutput::writeData(QByteArray data)
{
    qDebug() << "wrote: " << device->write(data.data(), data.size()) << " down from: " << data.size();
}
