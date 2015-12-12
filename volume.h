#ifndef VOLUMEALGORITHM_H
#define VOLUMEALGORITHM_H

#include <QList>

class PlaylistIter;

class VolumeAlgorithm
{
    QList<qreal> volumeInDecimalCalibrated;
    qreal volumeCalibrationDecibel {0.0};
    QString volumeScaleName;
    friend class PlaylistIter;

protected:
    qreal calibrate(qreal value) const;

    /*!
     * \brief VolumeAlgorithm::convert Converts input value into the range of [0..1]
     * \param input Value in range of [0..1]. If reimplemented, any number
     * \return range of [0..1]
     * This function acts as a Template Method pattern for addVolume methods.
     * It allows to convert easily one value into desired one in range of [0..1]
     * that is required by the QtAudioOutput
     *
     * By default, this function casts double type into qreal one.
     */
    virtual qreal convert(qreal input) = 0;

public:
    VolumeAlgorithm(const QString &volumeScaleName);
    VolumeAlgorithm(const VolumeAlgorithm &other);
    VolumeAlgorithm(VolumeAlgorithm &&move);
    virtual ~VolumeAlgorithm(){}


    virtual void addVolume(qreal volume);
    virtual void addVolume(const QList<qreal> &volumes);

    QString getVolumeScaleName() const;
    void setVolumeScaleName(const QString &value);

    static qreal decibelToSoundPressure(qreal decibel);
    static qreal soundPressureToDecibel(qreal soundPressure);

    qreal getVolumeCalibrationDecibel() const;
    void setVolumeCalibrationDecibel(const qreal &value);
};

class VolumePercentLevel : public VolumeAlgorithm
{
protected:
    qreal convert(qreal input);

public:
    VolumePercentLevel();
    VolumePercentLevel(const VolumePercentLevel &other);
    VolumePercentLevel(VolumePercentLevel &&move);
    ~VolumePercentLevel(){}
};

class VolumeDecibelSoundPressureLevel : public VolumeAlgorithm
{
protected:
    qreal convert(qreal input);

public:
    VolumeDecibelSoundPressureLevel();
    VolumeDecibelSoundPressureLevel(const VolumeDecibelSoundPressureLevel &other);
    VolumeDecibelSoundPressureLevel(VolumeDecibelSoundPressureLevel &&move);
    ~VolumeDecibelSoundPressureLevel(){}
};

//class VolumeDecibelHearingLevel : public VolumeAlgorithm
//{
//protected:
//    qreal convert(qreal input);

//public:
//    VolumeDecibelHearingLevel();
//    VolumeDecibelHearingLevel(const VolumeDecibelHearingLevel&other);
//    VolumeDecibelHearingLevel(VolumeDecibelHearingLevel &&move);
//    ~VolumeDecibelHearingLevel() {}

//    static qreal convertToHearingLevel(qreal spl, qreal hearingLevelCoefficient);
//};

#endif // VOLUMEALGORITHM_H
