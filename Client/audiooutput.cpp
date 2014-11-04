#include "audiooutput.h"

AudioOutput::AudioOutput(QObject *parent) : QObject(parent)
{
    QAudioFormat format;
    format.setChannelCount(1);
    format.setSampleRate(8000);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    audio = new QAudioOutput(format, this);
    device = audio->start();
}

void AudioOutput::writeData(QByteArray data)
{
    QByteArray uncompressed = qUncompress(data);
    qDebug() << "wrote: " << device->write(uncompressed.data(), uncompressed.size()) << " down from: " << data.size();
}
