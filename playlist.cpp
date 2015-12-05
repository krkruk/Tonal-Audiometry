#include "playlist.h"
#include <cmath>

//PlaylistIter implementation **********************************************

PlaylistIter::PlaylistIter(const Playlist *playlist)
    : soundIterEnd(playlist->soundOrder.constEnd()),
      volumeIterEnd(playlist->volumePercentOrder.constEnd()),
      soundIterBegin(playlist->soundOrder.constBegin()),
      volumeIterBegin(playlist->volumePercentOrder.constBegin()),
      playlist(playlist),
      soundIterator(playlist->soundOrder.constBegin()),
      volumeIterator(playlist->volumePercentOrder.constBegin())
{}

PlaylistIter::PlaylistIter(const PlaylistIter &other)
    : soundIterEnd(other.playlist->soundOrder.constEnd()),
      volumeIterEnd(other.playlist->volumePercentOrder.constEnd()),
      soundIterBegin(other.playlist->soundOrder.constBegin()),
      volumeIterBegin(other.playlist->volumePercentOrder.constBegin()),
      playlist(other.playlist),
      soundIterator(other.playlist->soundOrder.constBegin()),
      volumeIterator(other.playlist->volumePercentOrder.constBegin())
{}

void PlaylistIter::reset()
{
    resetSoundIterator();
    resetVolumeIterator();
}

int PlaylistIter::getCurrentFrequency() const
{
    return static_cast<int>((*soundIterator)->getFrequency());
}

int PlaylistIter::getCurrentVolumeDb() const
{
    return Playlist::percentToDecibel(*volumeIterator);
}

int PlaylistIter::getCurrentVolumePercent() const
{
    return *volumeIterator;
}

void PlaylistIter::resetSoundIterator()
{
    soundIterator = playlist->soundOrder.constBegin();
}

void PlaylistIter::resetVolumeIterator()
{
    volumeIterator = playlist->volumePercentOrder.constBegin();
}


//PlaylistIterVolumeSequence ***********************************************

PlaylistIterVolumeSequence::PlaylistIterVolumeSequence(const Playlist *playlist)
    : PlaylistIter(playlist)
{}

PlaylistIterVolumeSequence::PlaylistIterVolumeSequence(const PlaylistIterVolumeSequence &other)
    : PlaylistIter(other)
{}

PlaylistIterVolumeSequence::~PlaylistIterVolumeSequence()
{}

bool PlaylistIterVolumeSequence::hasNext()
{
    if(volumeIterator == volumeEnd() && soundIterator == soundEnd())
        return false;

    if(volumeIterator == volumeBegin() && soundIterator != soundEnd()
            && startIterationPointFlag)
    {
        startIterationPointFlag = false;
        return true;
    }

    if(++volumeIterator != volumeEnd())
    {
        if(soundIterator != soundEnd())
            return true;
        else
            return false;
    }
    else
    {
        resetVolumeIterator();
        if(++soundIterator != soundEnd())
            return true;
        else
            return false;
    }
}

QPair<QIODevice *, int> PlaylistIterVolumeSequence::nextLeft()
{
    QPair<QIODevice *, int>
            leftSoundSample((*soundIterator)->getSound(SoundSample::Direction::Left),
                            *volumeIterator);
    return leftSoundSample;
}

QPair<QIODevice *, int> PlaylistIterVolumeSequence::nextRight()
{
    QPair<QIODevice *, int>
            rightSoundSample((*soundIterator)->getSound(SoundSample::Direction::Right),
                             *volumeIterator);
    return rightSoundSample;
}

void PlaylistIterVolumeSequence::skipCurrentSoundSet()
{
    if(++soundIterator != soundEnd())
    {
        startIterationPointFlag = true;
        resetVolumeIterator();
    }
    else
        volumeIterator = volumeEnd();
}

void PlaylistIterVolumeSequence::reset()
{
    PlaylistIter::reset();
    startIterationPointFlag = true;
}

PlaylistIterVolumeSequence PlaylistIterVolumeSequence::operator=(const PlaylistIterVolumeSequence &p)
{
    return p;
}


//Playlist implementation **************************************************

int Playlist::decibelToPercent(int decibel)
{
    return pow(10, decibel * 0.05 + 2);
}

int Playlist::percentToDecibel(int percent)
{
    return 20 * log10(percent * 0.01);
}

Playlist::Playlist(QObject *parent)
    : QObject(parent)
{}

Playlist::Playlist(const Playlist &playlist, QObject *parent)
    : QObject(parent), soundOrder(playlist.soundOrder),
      volumePercentOrder(playlist.volumePercentOrder)
{}

Playlist::Playlist(Playlist &&playlist, QObject *parent)
    : QObject(parent), soundOrder(std::move(playlist.soundOrder)),
      volumePercentOrder(std::move(playlist.volumePercentOrder))
{}

Playlist::~Playlist()
{
}

void Playlist::addSound(const std::shared_ptr<Sound> &sound)
{
    soundOrder.append(sound);
}

void Playlist::addVolumeInPercent(int volume)
{
    volumePercentOrder.append(volume);
}

void Playlist::addVolumeInDb(int volume)
{
    auto percent = decibelToPercent(volume);
    volumePercentOrder.append(percent);
}

QList<int> Playlist::getVolumeListPercent() const
{
    return volumePercentOrder;
}

QList<int> Playlist::getVolumeListDb()
{
    auto dbVolumes(volumePercentOrder);

    for(auto &dbVolume : dbVolumes)
        dbVolume = percentToDecibel(dbVolume);

    return dbVolumes;
}

PlaylistIterVolumeSequence *Playlist::iterator() const
{
    return new PlaylistIterVolumeSequence(this);
}

