#include "audioinput.h"

/**
 * @brief AudioInput::AudioInput default constructor for the AudioInput.
 * @param devinfo the device info to use when initialiasing.
 * @param parent the parent object.
 */
AudioInput::AudioInput(QAudioDeviceInfo devinfo, QObject *parent)
    : QObject(parent),
      sendAudio(false)
{
    format.setChannelCount(1);
    format.setSampleRate(SAMPLE_RATE);
    format.setSampleSize(SAMPLE_SIZE);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    startDevice(devinfo);
}

/**
 * @brief AudioInput::changeDevice changes the input audio device.
 * @param devinfo the new audio device.
 */
void AudioInput::changeDevice(QAudioDeviceInfo devinfo)
{
    audio->stop();
    delete audio;

    startDevice(devinfo);
}

/**
 * @brief AudioInput::startDevice initialises the audio and starts recording.
 * @param devinfo device to read from.
 */
void AudioInput::startDevice(QAudioDeviceInfo devinfo)
{
    qDebug() << "device started";
    audio = new QAudioInput(devinfo, format, this);
    audio->setBufferSize(AUDIO_BUFFER_SIZE);
    device = audio->start();
    connect(device, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

/**
 * @brief AudioInput::readyRead slot called when device is ready to read, emits dataReady() signal.
 */
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
            if(sendAudio)
                emit dataReady(buffer);
        }
    }
}

/**
 * @brief AudioInput::setVolume sets input volume of device.
 * @param volume the volume to set.
 */
void AudioInput::setVolume(float volume)
{
    audio->setVolume(volume);
}

/**
 * @brief AudioInput::startAudio sets sendAudio to true; starts sending audio.
 */
void AudioInput::startAudio()
{
    sendAudio = true;
}

/**
 * @brief AudioInput::stopAudio sets sendAudio to false; stops sending audio.
 */
void AudioInput::stopAudio()
{
    sendAudio = false;
}
