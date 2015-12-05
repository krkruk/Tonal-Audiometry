#include <QDebug>
#include "soundplayer.h"

#define DIRECTION_ERROR "Incorrect SoundSample::Direction"
#define PLAYLIST_ERROR "Playlist error"

SoundPlayer::SoundPlayer(const QAudioFormat &format, const QAudioDeviceInfo &info, QObject *parent)
    : QObject(parent), audioDevice(new QAudioOutput(info, format, this)),
      audioDeviceInfo(info)
{
    timeGapTimer = new QTimer(this);
    timeGapTimer->setInterval(noSoundTimeSpanMs);
    timeGapTimer->setSingleShot(true);
    connect(timeGapTimer, SIGNAL(timeout()), this, SLOT(playNextSample()));

//    connect(audioDevice, &QAudioOutput::stateChanged, this, &SoundPlayer::onStateChanged);
    connect(audioDevice, SIGNAL(stateChanged(QAudio::State)), this, SLOT(onStateChanged(QAudio::State)));
}

SoundPlayer::~SoundPlayer()
{
    delete audioDevice;
    delete timeGapTimer;
}

int SoundPlayer::getNoSoundTimeSpanMs() const
{
    return noSoundTimeSpanMs;
}

void SoundPlayer::setNoSoundTimeSpanMs(int miliseconds)
{
    noSoundTimeSpanMs = miliseconds;
}

void SoundPlayer::setPlaylist(Playlist *playlist)
{
    this->playlist = playlist;
}

void SoundPlayer::runPlaylist(const SoundSample::Direction &channel)
{
    playlistIter = std::shared_ptr<PlaylistIterVolumeSequence>(playlist->iterator());   //might be a memory leak

    if(playlistIter->hasNext())
    {
        auto sample = playlistIter->nextLeft();
        audioDevice->setVolume(sample.second * 0.01);
        if((sample.first)->open(QIODevice::ReadOnly))
            audioDevice->start(sample.first);
    }
    else
        emit errorString(PLAYLIST_ERROR);

    Q_UNUSED(channel)
}

void SoundPlayer::resetPlaylist()
{
    playlistIter.reset();
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
        auto sample = playlistIter->nextLeft();
        audioDevice->setVolume(sample.second * 0.01);
        if((sample.first)->open(QIODevice::ReadOnly))
            audioDevice->start(sample.first);
    }
    else emit playlistEnded();
}

