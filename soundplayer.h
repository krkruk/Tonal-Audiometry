#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <QObject>
#include <QAudio>
#include <QAudioOutput>
#include <QTimer>
#include "playlist.h"

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
    void runPlaylist(const SoundSample::Direction &channel);
    void resetPlaylist();

signals:
    void errorString(const QString &e);
    void playlistEnded();

private slots:
    void onStateChanged(QAudio::State state);
    void playNextSample();

private:
    QAudioOutput *audioDevice {nullptr};
    QAudioDeviceInfo audioDeviceInfo;
    QTimer *timeGapTimer {nullptr};

    Playlist *playlist {nullptr};
    std::shared_ptr<PlaylistIterVolumeSequence> playlistIter;
    QPair<QIODevice *, int> (PlaylistIterVolumeSequence::*play)(void);

    int noSoundTimeSpanMs {2000};

};

#endif // SOUNDPLAYER_H
