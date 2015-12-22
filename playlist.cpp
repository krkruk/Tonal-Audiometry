#include "playlist.h"
#include "constants.h"
#include <cmath>

//PlaylistIter implementation **********************************************

/*!
 * \brief PlaylistIter::PlaylistIter Iterator abstract class for Playlist
 * \param playlist
 * This abstract class allows implementing iteration over the elements
 * that are placed in Playlist class. The user can access begin and end
 * iterators.
 * The key of iteration are hasNext() and skipCurrentSoundSet() functions.
 * See the reference of these functions for more details.
 */
PlaylistIter::PlaylistIter(const Playlist *playlist)
    : soundIterEnd(playlist->soundOrder.constEnd()),
      volumeIterEnd(playlist->volumeOrder->volumeInDecimal.constEnd()),
      soundIterBegin(playlist->soundOrder.constBegin()),
      volumeIterBegin(playlist->volumeOrder->volumeInDecimal.constBegin()),
      playlist(playlist),
      soundIterator(playlist->soundOrder.constBegin()),
      volumeIterator(playlist->volumeOrder->volumeInDecimal.constBegin())
{}

PlaylistIter::PlaylistIter(const PlaylistIter &other)
    : soundIterEnd(other.playlist->soundOrder.constEnd()),
      volumeIterEnd(other.playlist->volumeOrder->volumeInDecimal.constEnd()),
      soundIterBegin(other.playlist->soundOrder.constBegin()),
      volumeIterBegin(other.playlist->volumeOrder->volumeInDecimal.constBegin()),
      playlist(other.playlist),
      soundIterator(other.playlist->soundOrder.constBegin()),
      volumeIterator(other.playlist->volumeOrder->volumeInDecimal.constBegin())
{}

/*!
 * \brief PlaylistIter::nextLeft Return sound source and volume for the left speaker
 * \return Return QIODevice* sound device and qreal volume in system range [0..1]
 * This function allows playing externally the sound that is described
 * in QPair output. The volume passed in QPair is calibrated beforehand according
 * to VolumeAlgorithm corresponding values.
 * The return value is a current iteration that is made by hasNext() method
 */
QPair<QIODevice *, qreal> PlaylistIter::nextLeft()
{
    QPair<QIODevice *, qreal>
            leftSoundSample((*soundIterator)->getSound(SoundSample::Direction::Left),
                            (*volumeIterator));
    return leftSoundSample;
}

/*!
 * \brief PlaylistIter::nextRight Return sound source and volume for the right speaker
 * \return Return QIODevice* sound device and qreal volume in system range [0..1]
 * This function allows playing externally the sound that is described
 * in QPair output. The volume passed in QPair is calibrated beforehand according
 * to VolumeAlgorithm corresponding values.
 * The return value is a current iteration that is made by hasNext() method
 */
QPair<QIODevice *, qreal> PlaylistIter::nextRight()
{
    QPair<QIODevice *, qreal>
            rightSoundSample((*soundIterator)->getSound(SoundSample::Direction::Right),
                             (*volumeIterator));
    return rightSoundSample;
}

void PlaylistIter::stop()
{
    soundIterator = soundEnd();
    volumeIterator = volumeEnd();
}

/*!
 * \brief PlaylistIter::reset Resets all iterators to its beginings
 */
void PlaylistIter::reset()
{
    resetSoundIterator();
    resetVolumeIterator();
}

/*!
 * \brief PlaylistIter::getCurrentFrequency Returns current frequency that is played
 * \return Frequency in integer
 */
int PlaylistIter::getCurrentFrequency() const
{
    return static_cast<int>((*soundIterator)->getFrequency());
}

/*!
 * \brief PlaylistIter::getCurrentVolumeDb Gets current sound level in dB scale
 * \return qreal decibels
 */
qreal PlaylistIter::getCurrentVolumeDb() const
{
    return VolumeAlgorithm::soundPressureToDecibel(*volumeIterator);
}

/*!
 * \brief PlaylistIter::getCurrentVolumePercent Gets current sound level in percent scale
 * \return qreal decibels
 */
qreal PlaylistIter::getCurrentVolumePercent() const
{
    return *volumeIterator;
}

void PlaylistIter::resetSoundIterator()
{
    soundIterator = playlist->soundOrder.constBegin();
}

void PlaylistIter::resetVolumeIterator()
{
    volumeIterator = playlist->volumeOrder->volumeInDecimal.constBegin();
}

qreal PlaylistIter::calibrate(int frequency, qreal volumePercentToCalibrate) const
{
    return playlist->volumeOrder->calibrate(frequency,
                                            volumePercentToCalibrate);
}



/*!
 * \brief PlaylistIter::calibrateVolume Calibrates the volume according to VolumeAlgorithm value
 * \param input Current value in percent [0..1] that is to be played
 * \return [0..1] calibrated value
 */


//PlaylistIterVolumeSequence ***********************************************

PlaylistIterVolumeSequence::PlaylistIterVolumeSequence(const Playlist *playlist)
    : PlaylistIter(playlist)
{}

PlaylistIterVolumeSequence::PlaylistIterVolumeSequence(const PlaylistIterVolumeSequence &other)
    : PlaylistIter(other)
{}

PlaylistIterVolumeSequence::~PlaylistIterVolumeSequence()
{}
/*!
 * \brief PlaylistIterVolumeSequence::hasNext Iterate over sounds and volumes
 * \return Boolean
 * This function is the main engine that iterates over the sound and volume values.
 * The sequence is: start a new sound, then iterate over all volume values.
 * If volume reaches end, a new sound is passed and so on. The iteration stops
 * when the set sounds reaches end.
 */
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

    auto isNextSoundIterator = [&](){ return soundIterator != soundEnd(); };
    if(++volumeIterator == volumeEnd())
    {
        resetVolumeIterator();
        ++soundIterator;
        return isNextSoundIterator();
    }
    else
        return isNextSoundIterator();
}

/*!
 * \brief PlaylistIterVolumeSequence::skipCurrentSoundSet Skips current playing set of sound
 * Skip current set of volumes at particular sound
 */
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

/*!
 * \brief PlaylistIterVolumeSequence::reset Resets the iterator
 * Resets the iterator into the initial state.
 */
void PlaylistIterVolumeSequence::reset()
{
    PlaylistIter::reset();
    startIterationPointFlag = true;
}

PlaylistIterVolumeSequence PlaylistIterVolumeSequence::operator=(const PlaylistIterVolumeSequence &p)
{
    return p;
}
//PlaylistIterVolumeSequenceHearingLevel implementation ***********************
PlaylistIterVolumeSequenceHearingLevel::PlaylistIterVolumeSequenceHearingLevel(const Playlist *playlist)
    : PlaylistIterVolumeSequence(playlist)
{}

PlaylistIterVolumeSequenceHearingLevel::PlaylistIterVolumeSequenceHearingLevel(const PlaylistIterVolumeSequenceHearingLevel &other)
    : PlaylistIterVolumeSequence(other)
{}

PlaylistIterVolumeSequenceHearingLevel::~PlaylistIterVolumeSequenceHearingLevel()
{}

QPair<QIODevice *, qreal> PlaylistIterVolumeSequenceHearingLevel::nextLeft()
{
    auto currFreq = getCurrentFrequency();
    QPair<QIODevice *, qreal>
            leftSoundSample((*soundIterator)->getSound(SoundSample::Direction::Left),
                            calibrate(currFreq, *volumeIterator));
    return leftSoundSample;
}

QPair<QIODevice *, qreal> PlaylistIterVolumeSequenceHearingLevel::nextRight()
{
    auto currFreq = getCurrentFrequency();
    QPair<QIODevice *, qreal>
            rightSoundSample((*soundIterator)->getSound(SoundSample::Direction::Right),
                             calibrate(currFreq, *volumeIterator));
    return rightSoundSample;
}

PlaylistIterVolumeSequenceHearingLevel PlaylistIterVolumeSequenceHearingLevel::operator=(const PlaylistIterVolumeSequenceHearingLevel &p)
{
    return p;
}

//Playlist implementation **************************************************
/*!
 * \brief Playlist::Playlist Creates playlist
 * \param parent
 * Playlist combines both sound samples and volume samples that
 * are meant to be played. If data is passed into the class
 * the iterator can be return and the sound samples played.
 */
Playlist::Playlist(QObject *parent)
    : QObject(parent), volumeOrder(std::shared_ptr<VolumeAlgorithm>{new VolumePercentLevel})
{}

Playlist::Playlist(const Playlist &playlist, QObject *parent)
    : QObject(parent), soundOrder(playlist.soundOrder),
      volumeOrder(playlist.volumeOrder)
{}

Playlist::Playlist(Playlist &&playlist, QObject *parent)
    : QObject(parent), soundOrder(std::move(playlist.soundOrder)),
      volumeOrder(std::move(playlist.volumeOrder))
{}

Playlist::~Playlist()
{
}

/*!
 * \brief Playlist::addSound Add Sound sample
 * \param sound Sound sample
 */
void Playlist::addSound(const std::shared_ptr<Sound> &sound)
{
    soundOrder.append(sound);
}

/*!
 * \brief Playlist::addSound Adds Sound samples in QList
 * \param sound Sound samples
 */
void Playlist::addSound(const QList<std::shared_ptr<Sound> > &sounds)
{
    soundOrder.append(sounds);
}

/*!
 * \brief Playlist::setVolumeAlgoritm Sets volume algorithm that volumes are passed with
 * \param volumeAlgorithm Any child class of VolumeAlgorithm
 * In order to pass and play a sequence of sounds with given volume an appropriate
 * volume algorithm must be chosen. If none satisfies user requirements, it
 * is possible to create his own Volume Algorithm
 */
void Playlist::setVolumeAlgoritm(const std::shared_ptr<VolumeAlgorithm> &volumeAlgorithm)
{
    volumeOrder = volumeAlgorithm;
}

/*!
 * \brief Playlist::iterator Returns iterator to the sound and volume data
 * \return Iterator object created with new operator
 * The returned value allows iterating over the elements in order
 * to play the playlist in a given sequence.
 * The PlaylistIterVolumeSequence object must be destroyed manually
 * after it expires.
 */
std::shared_ptr<PlaylistIter> Playlist::iterator()
{
    if(volumeOrder->getVolumeScaleName() == Consts::VOLUME_DECIBEL_HEARING_LEVEL)
        return std::shared_ptr<PlaylistIter>{new PlaylistIterVolumeSequenceHearingLevel(this)};
    else
        return std::shared_ptr<PlaylistIter>{new PlaylistIterVolumeSequence(this)};
}
