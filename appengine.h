#ifndef APPENGINE_H
#define APPENGINE_H

#include <QObject>
#include <QAudioOutput>
#include "filesound.h"
#include "soundplayer.h"

class AppEngine : public QObject
{
    Q_OBJECT

    void createPlaylist();
public:
    explicit AppEngine(QObject *parent = 0);
    ~AppEngine();

    void setRootQmlObject(QObject *rootQmlObj);
    void connectAll();

signals:
    void playlistEnded();

public slots:
    void playPlaylist();
    void onCurrentPlaylistElement(const AudiogramData &data);
    void onPlaylistEnded();
    void onHearingButtonClicked();

private:
    QObject *rootObj {nullptr};
    QAudioFormat audioFormat;
    SoundPlayer *player;
    Playlist playlist;
    std::shared_ptr<VolumePercentLevel> volumesPercent;
    std::shared_ptr<VolumeDecibelSoundPressureLevel> volumesSPL;
    std::shared_ptr<VolumeDecibelHearingLevel> volumesHL;

    AudiogramPlotData audiogramPlotData;
    AudiogramData currentAudiogramData;
    bool hearingButtonClicked {true};
    void unsetHearingButton() { hearingButtonClicked = false; }
    void setHearingButtonClicked() { hearingButtonClicked = true; }
};

#endif // APPENGINE_H
