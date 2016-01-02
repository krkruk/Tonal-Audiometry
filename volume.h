#ifndef VOLUMEALGORITHM_H
#define VOLUMEALGORITHM_H

#include <QList>
#include <QMap>
#include "soundsample.h"

class PlaylistIter;

class VolumeCalibration
{
    qreal volumeGainDB {0.0};
    QMap<int, qreal> frequencyCalibrationVolumeDB;

public:
    VolumeCalibration();
    VolumeCalibration(qreal volumeGainDB);
    VolumeCalibration(const VolumeCalibration &other);
    VolumeCalibration(VolumeCalibration &&move);
    VolumeCalibration &operator=(const VolumeCalibration &other);


    qreal getVolumeGainDB() const;
    void setVolumeGainDB(qreal value);

    qreal getFrequencyCalibrationVolumeDB(int frequency) const;
    qreal getFrequencyCalibrationVolumeDB(SoundSample::Frequency frequency) const;
    void setFrequencyCalibrationVolumeDB(int frequency, qreal splCalibration);
    void setFrequencyCalibrationVolumeDB(SoundSample::Frequency frequency, qreal splCalibration);
};

class VolumeAlgorithm
{
    friend class PlaylistIter;

private:
    QList<qreal> volumeInDecimal;
    VolumeCalibration volumeCalibration;
    QString volumeScaleName;

protected:
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
    qreal calibrate(int freq, qreal volumePercent) const;
    qreal calibrate(qreal volumeDb) const;
    void setFrequencyCalibrationVolumeDB(int frequency, qreal splCalibration);
    void setFrequencyCalibrationVolumeDB(SoundSample::Frequency frequency, qreal splCalibration);

public:
    VolumeAlgorithm(const QString &volumeScaleName);
    VolumeAlgorithm(const VolumeAlgorithm &other);
    VolumeAlgorithm(VolumeAlgorithm &&move);
    virtual ~VolumeAlgorithm(){}


    virtual void addVolume(qreal volume);
    virtual void addVolume(const QList<qreal> &volumes);

    VolumeCalibration getVolumeCalibration() const;
    void setVolumeCalibration(const VolumeCalibration &value);

    qreal getVolumeGainDB() const;
    void setVolumeGainDB(qreal value);

    QString getVolumeScaleName() const;
    void setVolumeScaleName(const QString &value);

    static qreal decibelToSoundPressureLevel(qreal decibel);
    static qreal soundPressureLevelToDecibel(qreal soundPressure);
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

class VolumeDecibelHearingLevel : public VolumeAlgorithm
{
protected:
    qreal convert(qreal input);

public:
    VolumeDecibelHearingLevel();
    VolumeDecibelHearingLevel(const VolumeDecibelHearingLevel&other);
    VolumeDecibelHearingLevel(VolumeDecibelHearingLevel &&move);
    ~VolumeDecibelHearingLevel() {}

    void setDecibelHearingLevelCalibrationGain(int frequency, float splCalibration);
    void setDecibelHearingLevelCalibrationGain(SoundSample::Frequency frequency,
                                               float splCalibration);
};

#endif // VOLUMEALGORITHM_H
