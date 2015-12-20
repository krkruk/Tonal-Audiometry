#ifndef AUDIOGRAMDATA_H
#define AUDIOGRAMDATA_H
#include <QList>
#include <QDebug>
#include "soundsample.h"

class AudiogramData
{
    int frequency {0};
    qreal volumeDb {0.0};
    qreal volumePercent {0.0};

public:
    AudiogramData();
    AudiogramData(int freq, qreal volDb, qreal volPercent);
    AudiogramData(const AudiogramData &other);
    AudiogramData(AudiogramData &&move);

    int getFrequency() const;
    void setFrequency(int value);

    qreal getVolumeDb() const;
    void setVolumeDb(qreal value);

    qreal getVolumePercent() const;
    void setVolumePercent(qreal value);

    AudiogramData &operator=(const AudiogramData &a);
    friend QDebug operator<<(QDebug dbg, const AudiogramData &a)
    {
        QString msg("Freq: ");
        msg += QString::number(a.frequency) + ' ';
        msg += "VolDB: " + QString::number(a.volumeDb) + ' ';
        msg += "Vol%: " + QString::number(a.volumePercent);
        dbg << msg;
        return dbg;
    }

    bool operator<(const AudiogramData &a) const
    {
        return frequency < a.frequency;
    }
    bool operator>(const AudiogramData &a) const
    {
        return frequency > a.frequency;
    }
};

class AudiogramObservable
{
public:
    AudiogramObservable(){}
    virtual ~AudiogramObservable(){}
    virtual void update(const AudiogramData &audiogram) = 0;
};

class AudiogramPlotData : public AudiogramObservable
{
    QList<AudiogramData> audiogramData;
public:
    AudiogramPlotData();
    ~AudiogramPlotData();

    void update(const AudiogramData &audiogram);
    QList<AudiogramData> getSortedData();
    bool popLast();
};

#endif // AUDIOGRAMDATA_H
