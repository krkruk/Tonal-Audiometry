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
    Q_PROPERTY(QString topBarMsg READ topBarMsg WRITE setTopBarMsg NOTIFY topBarMsgChanged)

    friend class AudiogramChartWidget;
    static constexpr int MAX_AVAILABLE_VOLUME_DB = 100;

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
    AudiogramData previousAudiogramData;
    QList<AudiogramData> audiogramDataTemp;

    bool canSkipTrack {false};

    QString m_topBarMsg;
};

#endif // APPENGINE_H
