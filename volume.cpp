#include <QDebug>
#include "volume.h"
#include "constants.h"
#include <utility>
#include <cmath>

//VolumeAlgorithm implementation **********************************************
qreal VolumeAlgorithm::calibrate(qreal value) const
{
    return value + volumeCalibrationDecibel;
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
    : volumeInDecimalCalibrated(other.volumeInDecimalCalibrated),
      volumeCalibrationDecibel(other.volumeCalibrationDecibel),
      volumeScaleName(other.volumeScaleName)
{}

VolumeAlgorithm::VolumeAlgorithm(VolumeAlgorithm &&move)
    : volumeInDecimalCalibrated(std::move(move.volumeInDecimalCalibrated)),
      volumeCalibrationDecibel(std::move(move.volumeCalibrationDecibel)),
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
    volumeInDecimalCalibrated.append(convert(volume));
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
        volumeInDecimalCalibrated.append(convert(elem));
}

qreal VolumeAlgorithm::getVolumeCalibrationDecibel() const
{
    return volumeCalibrationDecibel;
}
/*!
 * \brief VolumeAlgorithm::setVolumeCalibrationDecibel Calibrate the output audio
 * \param value +/- decibels that are required to calibrate the device
 *
 * The audio output can be calibrated using this function. Enter a proper value
 * in decibels so any volume record passed into the class will be adjusted
 * according to the entered value.
 */
void VolumeAlgorithm::setVolumeCalibrationDecibel(const qreal &value)
{
    volumeCalibrationDecibel = VolumeAlgorithm::decibelToSoundPressure(value);
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
    return VolumeAlgorithm::decibelToSoundPressure(input);
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

VolumeDecibelSoundPressureLevel::VolumeDecibelSoundPressureLevel(const VolumeDecibelSoundPressureLevel &other)
    : VolumeAlgorithm(other)
{}

VolumeDecibelSoundPressureLevel::VolumeDecibelSoundPressureLevel(VolumeDecibelSoundPressureLevel &&move)
    : VolumeAlgorithm(std::move(move))
{}

////VolumeDecibelHearingLevel implementation ************************************
///*!
// * \brief VolumeDecibelSoundPressureLevel::convert Allows passing dB SPL
// * \param input Must be in db SPL scale
// * \return System required scale [0..1]
// */
//qreal VolumeDecibelHearingLevel::convert(qreal input)
//{
//    return VolumeAlgorithm::decibelToSoundPressure(input);
//}

///*!
// * \brief VolumeDecibelHearingLevel::VolumeDecibelHearingLevel Volume algorithm for dB HL scale
// * This class can be utilized in passing dB HL volume levels into the playlist.
// * The reference sound level is 0dB SPL at 1kHz frequency.
// * The reference pressure is 2*10^-5 Pa.
// *
// * Example
//  VolumeDecibelHearingLevel volHL;
//  volHL.addVolume(VolumeDecibelHearingLevel::convertToHearingLevel());
// */
//VolumeDecibelHearingLevel::VolumeDecibelHearingLevel()
//    : VolumeAlgorithm(Consts::VOLUME_DECIBEL_HEARING_LEVEL)
//{}

//VolumeDecibelHearingLevel::VolumeDecibelHearingLevel(const VolumeDecibelHearingLevel &other)
//    : VolumeAlgorithm(other)
//{}

//VolumeDecibelHearingLevel::VolumeDecibelHearingLevel(VolumeDecibelHearingLevel &&move)
//    : VolumeAlgorithm(std::move(move))
//{}

///*!
// * \brief VolumeDecibelHearingLevel::convertToHearingLevel
// * \param spl Volume intensity in db SPL scale
// * \param hearingLevelCoefficient Correction coefficient that must be added or substracted from the dBSPL value
// * \return Proper dBHL sound intensity value
// *
// * The hearingLevelCoefficient can be obtained from equal-loudness contour.
// */
//qreal VolumeDecibelHearingLevel::convertToHearingLevel(qreal spl, qreal hearingLevelCoefficient)
//{
//    return spl + hearingLevelCoefficient;
//}
/*!
 * \brief VolumeDecibelHearingLevel::convert Allows passing dB HL
 * \param input Must be in dB HL scale
 * \return System required scale [0..1]
 * This function converts dBHL value into the percent one that can be utilized
 * in QAudioOutput. The absolute threshold of hearing must be passed beforehand
 */
//qreal VolumeDecibelHearingLevel::convert(qreal input)
//{
//    return VolumeAlgorithm::decibelToSoundPressure(input + hearingLevelSPL);
//}

/*!
 * \brief VolumeDecibelHearingLevel::VolumeDecibelHearingLevel Volume algorithm for dB HL scale
 * This class can be utilized in passing dB HL volume levels into the playlist.
 * The reference sound level is 0dB SPL at 1kHz frequency.
 * The reference pressure is 2*10^-5 Pa.
 *
 * Example
 * VolumeDecibelHearingLevel volHL;
 * volHL.setHearingLevel(5);
 * volHL.addVolume(0); //results in 5dBSPL and 0dBHL
 */
//VolumeDecibelHearingLevel::VolumeDecibelHearingLevel()
//    : VolumeAlgorithm(Consts::VOLUME_DECIBEL_HEARING_LEVEL)
//{}

//VolumeDecibelHearingLevel::VolumeDecibelHearingLevel(const VolumeDecibelHearingLevel &other)
//    : VolumeAlgorithm(other)
//{}

//VolumeDecibelHearingLevel::VolumeDecibelHearingLevel(VolumeDecibelHearingLevel &&move)
//    : VolumeAlgorithm(std::move(move))
//{}

///*!
// * \brief VolumeDecibelHearingLevel::setHearingLevel Adds Hearing Level threshold
// * \param spl The threshold in dbSPL
// * The spl can be obtained from absolute threshold of hearing chart
// * e.g. @1000Hz ATH is 3 dBSPL and 0 dBHL
// */
//void VolumeDecibelHearingLevel::setHearingLevel(qreal spl)
//{
//    hearingLevelSPL = spl;
//}
