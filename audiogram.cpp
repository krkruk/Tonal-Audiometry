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

//QDebug AudiogramData::operator<<(QDebug dbg, const AudiogramData &a)
//{
//    QString msg("Freq: ");
//    msg += QString::number(a.frequency) + '\n';
//    msg += "VolDB: " + QString::number(a.volumeDb) + '\n';
//    msg += "Vol%: " + QString::number(a.volumePercent) + '\n';
//    msg += "UserInteraction: " + QString::number(a.userReactionConfirmed) + '\n';

//    return QDebug(msg);
//}

//AudiogramObserver implementation ********************************************

AudiogramObserver::AudiogramObserver()
{}

AudiogramObserver::AudiogramObserver(const AudiogramObserver &other)
    : audiogramData(other.audiogramData)
{}

AudiogramObserver::AudiogramObserver(AudiogramObserver &&move)
    : audiogramData(std::move(move.audiogramData))
{}

void AudiogramObserver::appendNewData(const AudiogramData &audiogram)
{
    audiogramData.append(audiogram);
}

QList<AudiogramData> AudiogramObserver::getData() const
{
    return audiogramData;
}

void AudiogramObserver::clear()
{
    audiogramData.clear();
}
