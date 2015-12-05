#include "filesound.h"
#include <utility>

/*!
 * \brief FileSound::getSoundChannelFileUrl Get a proper file url according to the desired direction
 * \param dirn Channel direction: left or right
 * \return QString url of the sound file
 */
QString FileSound::getSoundChannelFileUrl(const SoundSample::Direction &dirn)
{
    switch(dirn)
    {
    case SoundSample::Direction::Left:
        return leftSoundUrl;
        break;
    case SoundSample::Direction::Right:
        return rightSoundUrl;
        break;
    default:
        return QString();
        break;
    }
}

/*!
 * \brief FileSound::FileSound Create a class for a single sound sample
 * \param freq Frequency
 * \param leftChannel QString url to the left channel sound file
 * \param rightChannel QString url to the right channel sound file
 */
FileSound::FileSound(const SoundSample::Frequency &freq,
                     const QString &leftChannel, const QString &rightChannel)
    : Sound(freq), leftSoundUrl(leftChannel), rightSoundUrl(rightChannel)
{}

FileSound::FileSound(const FileSound &f)
    : Sound(f.getFrequency())
{
    leftSoundUrl = f.leftSoundUrl;
    rightSoundUrl = f.rightSoundUrl;
    this->close();
    currentSoundFileProcessed.setFileName(
                f.currentSoundFileProcessed.fileName());
}

FileSound::FileSound(FileSound && f)
    : Sound(std::move(f.getFrequency()))
{
    leftSoundUrl = std::move(f.leftSoundUrl);
    rightSoundUrl = std::move(f.rightSoundUrl);
    this->close();
    currentSoundFileProcessed.setFileName(
                f.currentSoundFileProcessed.fileName());
}

FileSound::~FileSound()
{
    if(currentSoundFileProcessed.isOpen())
        currentSoundFileProcessed.close();
}

/*!
 * \brief FileSound::getLeftSoundUrl Return a url to the left channel sound file
 * \return file name and/or directory
 */
QString FileSound::getLeftSoundUrl() const
{
    return leftSoundUrl;
}

/*!
 * \brief FileSound::getRightSoudnUrl Return a url to the right channel sound file
 * \return file name and/or directory
 */
QString FileSound::getRightSoudnUrl() const
{
    return rightSoundUrl;
}

/*!
 * \brief FileSound::getSound Get a pointer to a proper QIODevice containing sound data
 * \param dirn Choose sound channel
 * \return Function returns QIODevice* that can be utilized to play sound
 *
 * The function opens a file containing a proper sound file. The file
 * is opened automatically in ReadOnly mode. The sound file must be
 * closed by the user calling FileSound::close() function;
 */
QIODevice* FileSound::getSound(const SoundSample::Direction &dirn)
{

    auto fileUrl = getSoundChannelFileUrl(dirn);
    this->close();
    currentSoundFileProcessed.setFileName(fileUrl);
//    currentSoundFileProcessed.open(QIODevice::ReadOnly);
    return &currentSoundFileProcessed;
}

/*!
 * \brief FileSound::close Close QIODevice containing sound data if the QIODevice is open
 * Close QIODevice if it is not needed at the moment.
 */
void FileSound::close()
{
    if(currentSoundFileProcessed.isOpen())
        currentSoundFileProcessed.close();
}

