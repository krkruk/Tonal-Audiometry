#include <QDebug>
#include "soundplayer.h"

#define DIRECTION_ERROR "Incorrect SoundSample::Direction"
#define PLAYLIST_ERROR "Playlist error"
#define SOUND_SAMPLE_OPEN_ERROR "Could not open a sound sample file"

/*!
 * \brief SoundPlayer::SoundPlayer Constructor
 * \param format Requires audio format that is compatible with the device and *.wav files
 * \param info Info about the audio output device that is to be playing the soudn
 * \param parent QObject
 *
 * SoundPlayer constructor creates an object that deals with a playlist and
 * allows playing the sound in a required order
 */
SoundPlayer::SoundPlayer(const QAudioFormat &format, const QAudioDeviceInfo &info, QObject *parent)
    : QObject(parent), audioDevice(new QAudioOutput(info, format, this)),
      audioDeviceInfo(info)
{
    timeGapTimer = new QTimer(this);
    timeGapTimer->setInterval(noSoundTimeSpanMs);
    timeGapTimer->setSingleShot(true);

    connect(timeGapTimer, SIGNAL(timeout()), this, SLOT(playNextSample()));
    connect(audioDevice, SIGNAL(stateChanged(QAudio::State)), this, SLOT(onStateChanged(QAudio::State)));
}

/*!
 * \brief SoundPlayer::~SoundPlayer Destroy SoundPlayer
 */
SoundPlayer::~SoundPlayer()
{
    delete audioDevice;
    delete timeGapTimer;
}

/*!
 * \brief SoundPlayer::getNoSoundTimeSpanMs Gets a period of time the
 * sound is not played between each sound sample.
 * \return Time in miliseconds.
 */
int SoundPlayer::getNoSoundTimeSpanMs() const
{
    return noSoundTimeSpanMs;
}
/*!
 * \brief SoundPlayer::setNoSoundTimeSpanMs Sets time span of silence between each sound sample
 * \param miliseconds
 */
void SoundPlayer::setNoSoundTimeSpanMs(int miliseconds)
{
    noSoundTimeSpanMs = miliseconds;
    timeGapTimer->setInterval(noSoundTimeSpanMs);
}

/*!
 * \brief SoundPlayer::setPlaylist Set playlist of samples that is to be played
 * \param playlist Pointer of Playlist type
 */
void SoundPlayer::setPlaylist(Playlist *playlist)
{
    this->playlist = playlist;
}

/*!
 * \brief SoundPlayer::playPlaylist Start playing the sound samples in given sound channel
 * \param channel Select the sound channel of the sample
 *
 * playPlaylist allows playing the set of sound samples. The playlist always starts
 * at the beginning. The order of samples is set by PlaylistIter abstract class.
 * If playPlaylist is called and no samples are found the function emits PLAYLIST_ERROR.
 */
void SoundPlayer::playPlaylist(const SoundSample::Direction &channel)
{
    resetPlaylist();
    playlistIter = std::shared_ptr<PlaylistIter>(playlist->iterator());
    currentChannel = channel;

    if(playlistIter->hasNext())
    {
        auto sample = getSample();
        setAudioDevice(sample.first, sample.second);
    }
    else emit errorString(PLAYLIST_ERROR);
}

void SoundPlayer::resetPlaylist()
{
    playlistIter.reset();
}

/*!
 * \brief SoundPlayer::setCorrectionAdjustVolume Adjust sound level to the proper level
 * \param percent +/- value of the initial sound amplitude
 *
 * In order to adjust and calibrate the sound output the correction
 * coefficient must be set. The percent value adds or substracts the
 * amplitude so the sound level can be adjusted. Initially the value
 * is 0.
 *
 * positive integer - increase sound level
 * negative integer - decrease sound level
 */
void SoundPlayer::setCorrectionAdjustVolume(short percent)
{
    volumeAdjust = percent;
}

void SoundPlayer::onStateChanged(QAudio::State state)
{
    switch(state)
    {
    case QAudio::ActiveState:
        qDebug() <<"Active State" << audioDevice->volume()*100;
        break;
    case QAudio::SuspendedState:
        qDebug() <<"Suspended State";
        break;
    case QAudio::StoppedState:
//        qDebug() <<"Stopped State";

        timeGapTimer->start();
        break;
    case QAudio::IdleState:
//        qDebug() <<"Idle State";
        audioDevice->stop();
        break;
    default: break;
    }
}

void SoundPlayer::playNextSample()
{
    if(playlistIter->hasNext())
    {
        auto sample = getSample();
        setAudioDevice(sample.first, sample.second);
    }
    else emit playlistEnded();
}

/*!
 * \brief SoundPlayer::getSample Gets a sound sample with an appropriate sound channel
 * \return QPair<QIODevice *, int>
 *
 * The function returns sound sample data according to the channel selected in
 * playPlaylist() function.
 */
QPair<QIODevice *, int> SoundPlayer::getSample() const
{
    switch(currentChannel)
    {
    case SoundSample::Direction::Left:
        return playlistIter->nextLeft();
        break;
    case SoundSample::Direction::Right:
        return playlistIter->nextRight();
        break;
    default:
        return QPair<QIODevice *, int>(nullptr, 0);
        break;
    }
}

/*!
 * \brief SoundPlayer::setAudioDevice Sets QAudioDevice and its volume
 * \param device QIODevice pointer
 * \param volume integer, percent scaled
 *
 * The function sets AudioDevice, which contains sound sample, and
 * checks whether the device is open. If the device cannot be opened
 * SOUND_SAMPLE_OPEN_ERROR is emitted.
 */
void SoundPlayer::setAudioDevice(QIODevice *device, int volume)
{
    audioDevice->setVolume((volume + volumeAdjust) * 0.01);
    if(!device->isOpen())
    {
        if(device->open(QIODevice::ReadOnly))
            audioDevice->start(device);
        else
            emit errorString(SOUND_SAMPLE_OPEN_ERROR);
    }
    else
        audioDevice->start(device);
}

