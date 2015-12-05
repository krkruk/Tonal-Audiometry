#ifndef SOUND_H
#define SOUND_H
#include <QFile>
#include "soundsample.h"

/*!
 * \brief The Sound class is responsible for giving the interface for the sound sample
 */
class Sound
{
    SoundSample::Frequency frequency;
public:
    explicit Sound(const SoundSample::Frequency &freq);
    Sound(SoundSample::Frequency &&freq);
    virtual ~Sound() {}
    SoundSample::Frequency getFrequency() const;

    /*!
     * \brief getSound Get the sound data that can be played with QAudioOutput
     * \param dirn Channel the sound is played in
     * \return The function returns QIODevice object that can be played with QAudioOutput
     *
     * This is a virutal function that must be implemented by a child class.
     * This construction allows differing the source the sound is played
     * e.g. the sound files or generated sine wave.
     */
    virtual QIODevice* getSound(const SoundSample::Direction &dirn) = 0;

    /*!
     * \brief close Close QIODevice containing sound data if the QIODevice is open
     */
    virtual void close() = 0;
};

#endif // SOUND_H
