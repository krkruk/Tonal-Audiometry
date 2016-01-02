#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <QObject>
#include <QAudio>
#include <QAudioOutput>
#include <QTimer>
#include "playlist.h"
#include "audiogram.h"

class Playlist;
class FileSound;

class SoundPlayer : public QObject
{
    Q_OBJECT
public:
    explicit SoundPlayer(const QAudioFormat &format,
                         const QAudioDeviceInfo &info,
                         QObject *parent = 0);

    ~SoundPlayer();



    int getNoSoundTimeSpanMs() const;
    void setNoSoundTimeSpanMs(int miliseconds);

    void setPlaylist(Playlist *playlist);
    void playPlaylist(const SoundSample::Direction &channel);
    void resetPlaylist();
    void skipCurrentSoundSet();

    void setCorrectionAdjustVolume(qreal percent);

signals:
    void errorString(const QString &e);
    void playlistEnded();
    void currentPlaylistElement(const AudiogramData &currentElement);
    void aboutToPlayNextElement();
    void stopPlaying();

public slots:
    void stopPlaylist();

private slots:
    void onStateChanged(QAudio::State state);
    void playNextSample();

private:
    QAudioOutput *audioDevice {nullptr};
    QAudioDeviceInfo audioDeviceInfo;
    QTimer *timeGapTimer {nullptr};

    Playlist *playlist {nullptr};
    std::shared_ptr<PlaylistIter> playlistIter;

    int noSoundTimeSpanMs {2000};
    qreal volumeAdjust {0.0};

    SoundSample::Direction currentChannel {SoundSample::Direction::None};
    QPair<QIODevice *, qreal> getSample() const;
    void setAudioDevice(QIODevice *device, qreal volume);
};

class SingleFilePlayer : public QObject
{
    Q_OBJECT

public:
    explicit SingleFilePlayer(const QAudioFormat &format,
                              const QAudioDeviceInfo &info,
                              QObject *parent = 0);
    ~SingleFilePlayer();

    FileSound *getFileSound() const;
    void setFileSound(FileSound *value);

    void setVolume(qreal volume);
    qreal getVolume() const;

signals:
    void errorString(const QString &error);

public slots:
    void stop();
    void start();

private slots:
    void onStateChanged(QAudio::State state);

private:
    QAudioOutput *audioDevice {nullptr};
    QAudioDeviceInfo audioDeviceInfo;
    FileSound *file {nullptr};

    void setAudioDevice(QIODevice *device);
};

#endif // SOUNDPLAYER_H
