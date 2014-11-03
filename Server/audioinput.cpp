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

    device = audio->start();

    connect(device, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void AudioInput::readyRead()
{
    QByteArray buffer;

    //Check the number of samples in input buffer
    qint64 len = audio->bytesReady(); 

    if(len > 1024)
        len = 1024;

    //Read sound samples from input device to buffer
    if (len > 0) {
        buffer.resize(len);
        qDebug() << "read: " << device->read(buffer.data(), len);
        emit dataReady(buffer);
    }
    /*

    //Check the number of samples in input buffer
    qint64 len = audio->bytesReady();

    //Limit sample size
    if(len > 1024)
        len = 1024;
    //Read sound samples from input device to buffer
    qint64 l = device->read(buffer.data(), len);
    if(l > 0)
    {
        //Assign sound samples to short array
        short* resultingData = (short*)buffer.data();


        short *outdata=resultingData;
        outdata[ 0 ] = resultingData [ 0 ];

        int iIndex;

        //Low Pass filter
        for ( iIndex=1; iIndex < len; iIndex++ )
        {
        outdata[ iIndex ] = 0.333 * resultingData[iIndex ] + ( 1.0 - 0.333 ) * outdata[ iIndex-1 ];
        }

        emit dataReady((char*)outdata);

    }*/
}
