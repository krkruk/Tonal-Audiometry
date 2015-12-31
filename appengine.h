#ifndef APPENGINE_H
#define APPENGINE_H

#include <QObject>
#include <QAudioOutput>
#include "filesound.h"
#include "soundplayer.h"
#include "audiogramchart.h"

class AudiogramChartWidget;

class AudiometryAlgorithm
{
    static constexpr int MAX_AVAILABLE_VOLUME_DB = 100;
    static constexpr int MIN_AVAILABLE_VOLUME_DB = 0;
    static constexpr int LAST_AVAILABLE_VOLUME_DB = 80;

    AudiogramPlotData audiogramPlotData;
    AudiogramData currentAudiogramData;
    AudiogramData previousAudiogramData;
    QList<AudiogramData> audiogramDataTemp;

    bool canSkipTrack {false};
    bool canSkipTrackMaxVol {false};

    SoundPlayer *player {nullptr};
    bool hearButtonPressed{false};
    bool canSkipMaxDueToButtonPressed {false};
public:
    AudiometryAlgorithm(SoundPlayer *player)
        : player(player){}

    void onCurrentPlaylistElement(const AudiogramData &data);

    void onHearingButtonClicked();
    void onAboutToPlayNextElement();

    AudiogramPlotData getAudiogramPlotData() const;
    void resetAll();

private:
    void updateAudiogramPlotData();
};

class AppEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString topBarMsg READ topBarMsg WRITE setTopBarMsg NOTIFY topBarMsgChanged)

    friend class AudiogramChartWidget;
    using SSDir = SoundSample::Direction;

    static constexpr int MAX_AVAILABLE_VOLUME_DB = 100;
    static constexpr int MIN_AVAILABLE_VOLUME_DB = 0;
    static constexpr int LAST_AVAILABLE_VOLUME_DB = 80;

    void createPlaylist();
    void resetVariables();
public:
    explicit AppEngine(QObject *parent = 0);
    ~AppEngine();

    void setRootQmlObject(QObject *rootQmlObj);
    void connectAll();

    QString topBarMsg() const { return m_topBarMsg; }

signals:
    void playlistEnded();    
    void topBarMsgChanged(QString topBarMsg);

public slots:
    void onPlaylistEnded();
    void playPlaylist(int direction);
    void onCurrentPlaylistElement(const AudiogramData &data);

    void onHearingButtonClicked();
    void onAboutToPlayNextElement();

    void setTopBarMsg(QString topBarMsg);
    void saveFileRequest(const QUrl &url);
    void calibrationRequest(int decibel);
    void calibrationPlayRequest();

private:
    QObject *rootObj {nullptr};
    QAudioFormat audioFormat;
    SoundPlayer *player;
    Playlist playlist;
    std::shared_ptr<VolumePercentLevel> volumesPercent;
    std::shared_ptr<VolumeDecibelSoundPressureLevel> volumesSPL;
    std::shared_ptr<VolumeDecibelHearingLevel> volumesHL;

    AudiometryAlgorithm *algorithm;

    SSDir currentDirection {SSDir::None};

    AudiogramPlotData audiogramPlotDataLeft;
    AudiogramPlotData audiogramPlotDataRight;

    QString m_topBarMsg;
};

#endif // APPENGINE_H
