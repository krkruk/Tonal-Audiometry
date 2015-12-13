#include <QDebug>
#include "volume.h"
#include "constants.h"
#include <utility>
#include <cmath>

//VolumeCalibration implementation ********************************************
/*!
 * \brief VolumeCalibration::VolumeCalibration Sound level calibration class
 * The class allows adjusting and calibrating audio output as user needs.
 * It is possible to set global gain of the sound as well as setting
 * the gain for a particular frequency
 */
VolumeCalibration::VolumeCalibration(){}

VolumeCalibration::VolumeCalibration(qreal volumeGainDB)
    : volumeGainDB(volumeGainDB)
{}

VolumeCalibration::VolumeCalibration(const VolumeCalibration &other)
    : volumeGainDB(other.volumeGainDB),
      frequencyCalibrationVolumeDB(other.frequencyCalibrationVolumeDB)
{}

VolumeCalibration::VolumeCalibration(VolumeCalibration &&move)
    : volumeGainDB(std::move(move.volumeGainDB)),
      frequencyCalibrationVolumeDB(std::move(move.frequencyCalibrationVolumeDB))
{}

VolumeCalibration &VolumeCalibration::operator=(const VolumeCalibration &other)
{
    volumeGainDB = other.volumeGainDB;
    frequencyCalibrationVolumeDB = other.frequencyCalibrationVolumeDB;
    return *this;
}

/*!
 * \brief VolumeCalibration::getVolumeGainDB Gets global sound gain coefficient
 * \return qreal in decibels
 */
qreal VolumeCalibration::getVolumeGainDB() const
{
    return volumeGainDB;
}

/*!
 * \brief VolumeCalibration::setVolumeGainDB Sets global gain coefficient
 * \param value Decibels to be added or substracted for each sound sample
 */
void VolumeCalibration::setVolumeGainDB(qreal value)
{
    volumeGainDB = value;
}

/*!
 * \brief VolumeCalibration::getFrequencyCalibrationVolumeDB Gets gain coefficient for a particular frequency
 * \param frequency Frequency in integer, Herz unit
 * \return Total gain for a particular frequency
 * The return value consists of calibration coefficient for the particular frequency
 * + global volume gain
 */
qreal VolumeCalibration::getFrequencyCalibrationVolumeDB(int frequency) const
{
    return frequencyCalibrationVolumeDB.value(frequency, 0) + volumeGainDB;
}

/*!
 * \brief VolumeCalibration::getFrequencyCalibrationVolumeDB Gets gain coefficient for a particular frequency
 * \param frequency Frequency in SoundSample::Frequency, Herz unit
 * \return Total gain for a particular frequency
 * The return value consists of calibration coefficient for the particular frequency
 * + global volume gain
 */
qreal VolumeCalibration::getFrequencyCalibrationVolumeDB(SoundSample::Frequency frequency) const
{
    return frequencyCalibrationVolumeDB.value(
                static_cast<int>(frequency), 0) + volumeGainDB;
}

/*!
 * \brief VolumeCalibration::setFrequencyCalibrationVolumeDB Sets gain coefficient for a particular frequency
 * \param frequency Frequency in integer, Herz unit
 * \param splCalibration In decibel unit, value to be added/substracted for each particular frequency
 */
void VolumeCalibration::setFrequencyCalibrationVolumeDB(
        int frequency, qreal splCalibration)
{
    frequencyCalibrationVolumeDB.insert(frequency, splCalibration);
}

/*!
 * \brief VolumeCalibration::setFrequencyCalibrationVolumeDB Sets gain coefficient for a particular frequency
 * \param frequency Frequency in SoundSample::Frequency, Herz unit
 * \param splCalibration In decibel unit, value to be added/substracted for each particular frequency
 */
void VolumeCalibration::setFrequencyCalibrationVolumeDB(
        SoundSample::Frequency frequency, qreal splCalibration)
{
    frequencyCalibrationVolumeDB.insert(
                static_cast<int>(frequency), splCalibration);
}

//VolumeAlgorithm implementation **********************************************


qreal VolumeAlgorithm::calibrate(int freq, qreal volumePercent) const
{
//    qDebug() <<volumePercent;
//    qDebug() << VolumeAlgorithm::decibelToSoundPressure(
//                    volumeCalibration.getFrequencyCalibrationVolumeDB(freq));
//    return VolumeAlgorithm::decibelToSoundPressure(
//                volumeCalibration.getFrequencyCalibrationVolumeDB(freq))
//            + volumePercent;
    return VolumeAlgorithm::decibelToSoundPressure(
                volumeCalibration.getFrequencyCalibrationVolumeDB(freq)
                + VolumeAlgorithm::soundPressureToDecibel(volumePercent));
}

qreal VolumeAlgorithm::calibrate(qreal volumeDb) const
{
    return volumeDb + volumeCalibration.getVolumeGainDB();
}

void VolumeAlgorithm::setFrequencyCalibrationVolumeDB(int frequency, qreal splCalibration)
{
    volumeCalibration.setFrequencyCalibrationVolumeDB(frequency, splCalibration);
}

void VolumeAlgorithm::setFrequencyCalibrationVolumeDB(
        SoundSample::Frequency frequency, qreal splCalibration)
{
    volumeCalibration.setFrequencyCalibrationVolumeDB(frequency, splCalibration);
}
/*!
 * \brief VolumeAlgorithm::VolumeAlgorithm Name the VolumeAlgorithm child
 * \param volumeScaleName name of the child class
 * In order to create a class that adjusts user input into system
 * predefined one (range [0..1]) it is necessery to inherit this class.
 * All conversions must be implemented in pure virtual function convert().
 */
VolumeAlgorithm::VolumeAlgorithm(const QString &volumeScaleName)
    : volumeScaleName(volumeScaleName)
{}

VolumeAlgorithm::VolumeAlgorithm(const VolumeAlgorithm &other)
    : volumeInDecimal(other.volumeInDecimal),
      volumeScaleName(other.volumeScaleName)
{}

VolumeAlgorithm::VolumeAlgorithm(VolumeAlgorithm &&move)
    : volumeInDecimal(std::move(move.volumeInDecimal)),
      volumeScaleName(std::move(move.volumeScaleName))
{}

/*!
 * \brief VolumeAlgorithm::addVolume Adds volume to the series
 * \param volume must be in a proper format
 * This function adds a new volume value into a playlist. The
 * value is automatically converted into in-system format.
 * See convert() function for more details.
 */
void VolumeAlgorithm::addVolume(double volume)
{
    volumeInDecimal.append(convert(volume));
}
/*!
 * \brief VolumeAlgorithm::addVolume Adds volumes to the series
 * \param volumes must be in a proper format
 * This function adds a new QList of volume values into a playlist. The
 * value is automatically converted into in-system format.
 * See convert() function for more details.
 */
void VolumeAlgorithm::addVolume(const QList<double> &volumes)
{
    for(auto elem : volumes)
        volumeInDecimal.append(convert(elem));
}

VolumeCalibration VolumeAlgorithm::getVolumeCalibration() const
{
    return volumeCalibration;
}

void VolumeAlgorithm::setVolumeCalibration(const VolumeCalibration &value)
{
    volumeCalibration = value;
}

qreal VolumeAlgorithm::getVolumeGainDB() const
{
    return volumeCalibration.getVolumeGainDB();
}

void VolumeAlgorithm::setVolumeGainDB(qreal value)
{
    volumeCalibration.setVolumeGainDB(value);
}
/*!
 * \brief VolumeAlgorithm::getVolumeScaleName Get the name of current volume algorithm
 * \return Name of the current volume algorithm in QString format
 */
QString VolumeAlgorithm::getVolumeScaleName() const
{
    return volumeScaleName;
}

void VolumeAlgorithm::setVolumeScaleName(const QString &value)
{
    volumeScaleName = value;
}

/*!
 * \brief VolumeAlgorithm::decibelToSoundPressure Converts decibels into sound pressure in Pascals
 * \param decibel Decibel value
 * \return Sound pressure in Pascal unit.
 *
 * This function allows converting dB SPL or dB HL (or any) into Pascal unit and
 * process the data as needed.
 * The reference sound level is 0dB SPL at 1kHz frequency. The reference pressure is
 * 2*10^-5 Pa.
 */
qreal VolumeAlgorithm::decibelToSoundPressure(qreal decibel)
{
    return Consts::PRESSURE_0dB * pow(10, decibel * 0.05);
}

/*!
 * \brief VolumeAlgorithm::soundPressureToDecibel Converts sound pressure pascals into decibels
 * \param soundPressure Pascal unit value. Cannot be less or equal to 0.
 * \return Sound Pressure Level in Decibels
 *
 * This function allows converting sound pressure in Pascal unit
 * into decibel scale.
 * The reference sound level is 0dB SPL at 1kHz frequency. The reference pressure is
 * 2*10^-5 Pa.
 */
qreal VolumeAlgorithm::soundPressureToDecibel(qreal soundPressure)
{
    return 20 * log10(Consts::PRESSURE_0dB_INVERSED * soundPressure);
}


//VolumePercentLevel implementation *******************************************
/*!
 * \brief VolumePercentLevel::convert Allows passing percent value
 * \param input Percent in range of [0..1]
 * \return Percent in range of [0..1]
 * No calibration here is done.
 */
qreal VolumePercentLevel::convert(qreal input)
{
    return input;
}

/*!
 * \brief VolumePercentLevel::VolumePercentLevel Volume algorithm in percent scale
 * This class can be utilized in passing audio levels in percent scale.
 * 0 is the minimum (no sound), 1.0 is maximum (maximum sound).
 */
VolumePercentLevel::VolumePercentLevel()
    : VolumeAlgorithm(Consts::VOLUME_PERCENT_LEVEL)
{}

VolumePercentLevel::VolumePercentLevel(const VolumePercentLevel &other)
    : VolumeAlgorithm(other)
{}

VolumePercentLevel::VolumePercentLevel(VolumePercentLevel &&move)
    : VolumeAlgorithm(std::move(move))
{}


//VolumeDecibelSoundPressureLevel implementation ******************************
/*!
 * \brief VolumeDecibelSoundPressureLevel::convert Allows passing dB SPL
 * \param input Must be in db SPL scale
 * \return System required scale [0..1]
 */
qreal VolumeDecibelSoundPressureLevel::convert(qreal input)
{
    return VolumeAlgorithm::decibelToSoundPressure(calibrate(input));
}
/*!
 * \brief VolumeDecibelSoundPressureLevel::VolumeDecibelSoundPressureLevel Volume algorithm for dB SPL scale
 * This class can be utilized in passing audio levels in db SPL scale.
 * The reference sound level is 0dB SPL at 1kHz frequency.
 * The reference pressure is 2*10^-5 Pa.
 */
VolumeDecibelSoundPressureLevel::VolumeDecibelSoundPressureLevel()
    : VolumeAlgorithm(Consts::VOLUME_DECIBEL_SPL_LEVEL)
{}

VolumeDecibelSoundPressureLevel::VolumeDecibelSoundPressureLevel(
        const VolumeDecibelSoundPressureLevel &other)
    : VolumeAlgorithm(other)
{}

VolumeDecibelSoundPressureLevel::VolumeDecibelSoundPressureLevel(
        VolumeDecibelSoundPressureLevel &&move)
    : VolumeAlgorithm(std::move(move))
{}

////VolumeDecibelHearingLevel implementation ************************************
/*!
 * \brief VolumeDecibelSoundPressureLevel::convert Allows passing dB SPL
 * \param input Must be in db SPL scale
 * \return System required scale [0..1]
 */
qreal VolumeDecibelHearingLevel::convert(qreal input)
{
    return VolumeAlgorithm::decibelToSoundPressure(input);
}

/*!
 * \brief VolumeDecibelHearingLevel::VolumeDecibelHearingLevel Volume algorithm for dB HL scale
 * This class can be utilized in passing dB HL volume levels into the playlist.
 * The reference sound level is 0dB SPL at 1kHz frequency.
 * The reference pressure is 2*10^-5 Pa.
 */
VolumeDecibelHearingLevel::VolumeDecibelHearingLevel()
    : VolumeAlgorithm(Consts::VOLUME_DECIBEL_HEARING_LEVEL)
{}

VolumeDecibelHearingLevel::VolumeDecibelHearingLevel(const VolumeDecibelHearingLevel &other)
    : VolumeAlgorithm(other)
{}

VolumeDecibelHearingLevel::VolumeDecibelHearingLevel(VolumeDecibelHearingLevel &&move)
    : VolumeAlgorithm(std::move(move))
{}

/*!
 * \brief VolumeDecibelHearingLevel::setDecibelHearingLevelCalibrationGain Sets gain coefficient for a particular frequency
 * \param frequency Frequency in integer, Herz unit
 * \param splCalibration Decibel, to be added/substracted from the each value passed at given frequency
 */
void VolumeDecibelHearingLevel::setDecibelHearingLevelCalibrationGain(
        int frequency, float splCalibration)
{
    this->setFrequencyCalibrationVolumeDB(frequency, splCalibration);
}

/*!
 * \brief VolumeDecibelHearingLevel::setDecibelHearingLevelCalibrationGain Sets gain coefficient for a particular frequency
 * \param frequency Frequency in SoundSample::Frequency, Herz unit
 * \param splCalibration Decibel, to be added/substracted from the each value passed at given frequency
 */
void VolumeDecibelHearingLevel::setDecibelHearingLevelCalibrationGain(
        SoundSample::Frequency frequency, float splCalibration)
{
    this->setFrequencyCalibrationVolumeDB(frequency, splCalibration);
}

