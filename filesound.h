#ifndef FILESOUND_H
#define FILESOUND_H
#include "sound.h"
#include <memory>

class FileSound : public Sound
{
    QString leftSoundUrl;
    QString rightSoundUrl;
    QFile currentSoundFileProcessed;

    QString getSoundChannelFileUrl(const SoundSample::Direction &dirn);

public:
    explicit FileSound(const SoundSample::Frequency &freq,
              const QString &leftChannel, const QString &rightChannel);
    FileSound(const FileSound &f);
    FileSound(FileSound &&f);
    ~FileSound();

    QString getLeftSoundUrl() const;
    QString getRightSoudnUrl() const;

    QIODevice* getSound(const SoundSample::Direction &dirn);
    void close();
};

#endif // FILESOUND_H
