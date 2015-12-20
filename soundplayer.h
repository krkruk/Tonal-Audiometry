#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <QObject>
#include <QAudio>
#include <QAudioOutput>
#include <QTimer>
#include "playlist.h"
#include "audiogram.h"

class Playlist;

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

    SoundSample::Direction currentChannel {SoundSample::Direction::Left};
    QPair<QIODevice *, qreal> getSample() const;
    void setAudioDevice(QIODevice *device, qreal volume);
};

#endif // SOUNDPLAYER_H
