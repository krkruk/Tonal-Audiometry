#ifndef APPENGINE_H
#define APPENGINE_H

#include <QObject>
#include <QAudioOutput>
#include "filesound.h"
#include "soundplayer.h"
#include "audiogramchart.h"

class AudiogramChartWidget;


class AppEngine : public QObject
{
    Q_OBJECT
    friend class AudiogramChartWidget;
    static constexpr int MAX_VOLUME_DB = 120;

    void createPlaylist();
    void resetVariables();
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
    void onAboutToPlayNextElement();

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

    bool canPopElement {false};
    bool isEverButtonPressed {false};

};

#endif // APPENGINE_H
