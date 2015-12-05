#include "sound.h"
#include <utility>

/*!
 * \brief Sound::Sound Contructor
 * \param freq Frequency allowed in the program
 */
Sound::Sound(const SoundSample::Frequency &freq)
    : frequency(freq)
{}

Sound::Sound(SoundSample::Frequency &&freq)
    : frequency(std::move(freq))
{}

/*!
 * \brief Sound::getFrequency return the given frequency of the sample
 * \return SoundSample::Frequency
 */
SoundSample::Frequency Sound::getFrequency() const
{
    return frequency;
}

