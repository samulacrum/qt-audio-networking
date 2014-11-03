#include "audioinput.h"

AudioInput::AudioInput(QAudioDeviceInfo devinfo, QObject *parent)
    : QObject(parent)
    ,   buffer(4096, 0)
{
    qDebug("Create Audio");
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
    qDebug("Create Audio2");
    audio = new QAudioInput(devinfo, format, this);
    device = audio->start();
    connect(device, SIGNAL(readyRead()), this, SLOT(readAudio()));
    /*

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(readAudio()));
    timer->start(10);*/
}

void AudioInput::readAudio()
{
    if (!audio)
        return;

    //Check the number of samples in input buffer
    qint64 len = audio->bytesReady();
    qDebug() << len;
    if (len > 4096) {
        len = 4096;
    }

    //Read sound samples from input device to buffer
    qint64 l = device->read(buffer.data(), len);
    if (l > 0) {
        //Assign sound samples to short array
        short* resultingData = (short*)buffer.data();


        short *outdata=resultingData;
        outdata[ 0 ] = resultingData [ 0 ];

        //lowpass filter
        int iIndex;
        for ( iIndex=1; iIndex < len; iIndex++ )
        {
            outdata[ iIndex ] = 0.333 * resultingData[iIndex ] + ( 1.0 - 0.333 ) * outdata[ iIndex-1 ];
        }
        //m_output->write((char*)outdata, len);
        emit dataReady((char*)outdata);
    }

    //emit dataReady(data);
}
