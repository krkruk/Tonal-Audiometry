#ifndef AUDIOGRAMDATA_H
#define AUDIOGRAMDATA_H
#include <QList>
#include <QDebug>
#include "soundsample.h"

class AudiogramData
{
    int frequency {0};
    int volumeDb {0};
    int volumePercent {0};
    bool userReactionConfirmed {false};

public:
    AudiogramData();
    AudiogramData(int freq, int volDb, int volPercent, bool userReactionConfirmed = false);
    AudiogramData(const AudiogramData &other);
    AudiogramData(AudiogramData &&move);

    int getFrequency() const;
    void setFrequency(int value);

    int getVolumeDb() const;
    void setVolumeDb(int value);

    int getVolumePercent() const;
    void setVolumePercent(int value);

    bool getUserReactionConfirmed() const;
    void setUserReactionConfirmed(bool value);

    AudiogramData &operator=(const AudiogramData &a);
    friend QDebug operator<<(QDebug dbg, const AudiogramData &a)
    {
        QString msg("Freq: ");
        msg += QString::number(a.frequency) + ' ';
        msg += "VolDB: " + QString::number(a.volumeDb) + ' ';
        msg += "Vol%: " + QString::number(a.volumePercent) + ' ';
        msg += "UserInteraction: " + QString::number(a.userReactionConfirmed) + ' ';
        dbg << msg;
        return dbg;
    }
};

class AudiogramObserver
{
    QList<AudiogramData> audiogramData;

public:
    AudiogramObserver();
    AudiogramObserver(const AudiogramObserver &other);
    AudiogramObserver(AudiogramObserver &&move);

    virtual void appendNewData(const AudiogramData &audiogram);
    QList<AudiogramData> getData() const;
    void clear();
};

#endif // AUDIOGRAMDATA_H
