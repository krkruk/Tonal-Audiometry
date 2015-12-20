#include "audiogram.h"
#include <utility>

//AudiogramData implementation ************************************************
AudiogramData::AudiogramData()
{}

AudiogramData::AudiogramData(int freq, qreal volDb, qreal volPercent)
    : frequency(freq), volumeDb(volDb), volumePercent(volPercent)
{}

AudiogramData::AudiogramData(const AudiogramData &other)
    : frequency(other.frequency), volumeDb(other.volumeDb),
      volumePercent(other.volumePercent)
{}

AudiogramData::AudiogramData(AudiogramData &&move)
    : frequency(std::move(move.frequency)), volumeDb(std::move(move.volumeDb)),
      volumePercent(std::move(move.volumePercent))
{}

int AudiogramData::getFrequency() const
{
    return frequency;
}

void AudiogramData::setFrequency(int value)
{
    frequency = value;
}

qreal AudiogramData::getVolumeDb() const
{
    return volumeDb;
}

void AudiogramData::setVolumeDb(qreal value)
{
    volumeDb = value;
}

qreal AudiogramData::getVolumePercent() const
{
    return volumePercent;
}

void AudiogramData::setVolumePercent(qreal value)
{
    volumePercent = value;
}

AudiogramData &AudiogramData::operator=(const AudiogramData &a)
{
    frequency = a.frequency;
    volumeDb = a.volumeDb;
    volumePercent = a.volumePercent;
    return *this;
}

//AudiogramPlotData implementation ********************************************
AudiogramPlotData::AudiogramPlotData()
    : AudiogramObservable()
{}

AudiogramPlotData::~AudiogramPlotData()
{}

/*!
 * \brief AudiogramPlotData::update Append new data to the audiogram
 * \param audiogram AudiogramData
 */
void AudiogramPlotData::update(const AudiogramData &audiogram)
{
    audiogramData.append(audiogram);
}

/*!
 * \brief AudiogramPlotData::getSortedData Return sorted data in ascending order
 * \return QList<AudiogramData> in ascending order
 * This function returns ordered data so it can be used in drawing an
 * audiogram chart
 */
QList<AudiogramData> AudiogramPlotData::getSortedData()
{
    auto audiogramDataSort(audiogramData);
    qSort(audiogramDataSort);
//    qSort(audiogramDataSort.begin(), audiogramDataSort.end(),
//          [&](const auto &a, const auto &b){return a.getFrequency() > b.getFrequency();});
    return audiogramDataSort;
}

bool AudiogramPlotData::popLast()
{
    if(!audiogramData.isEmpty())
    {
        audiogramData.pop_back();
        return true;
    }
    return false;
}
