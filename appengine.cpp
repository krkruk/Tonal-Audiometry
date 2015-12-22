#include "appengine.h"
#include <QDebug>

void AppEngine::createPlaylist()
{
    playlist.addSound(std::shared_ptr<Sound>
                        {new FileSound(SoundSample::Frequency::Hz1000,
                         QString(":/soundSamples/soundSamples/Hz1000Left.wav"),
                         QString(":/soundSamples/soundSamples/Hz1000Right.wav"))});
    playlist.addSound(std::shared_ptr<Sound>
                        {new FileSound(SoundSample::Frequency::Hz250,
                         QString(":/soundSamples/soundSamples/Hz250Left.wav"),
                         QString(":/soundSamples/soundSamples/Hz250Right.wav"))});
    playlist.addSound(std::shared_ptr<Sound>
                        {new FileSound(SoundSample::Frequency::Hz500,
                         QString(":/soundSamples/soundSamples/Hz500Left.wav"),
                         QString(":/soundSamples/soundSamples/Hz500Right.wav"))});
//    playlist.addSound(std::shared_ptr<Sound>
//                        {new FileSound(SoundSample::Frequency::Hz125,
//                         QString(":/soundSamples/soundSamples/Hz125Left.wav"),
//                         QString(":/soundSamples/soundSamples/Hz125Right.wav"))});
//    playlist.addSound(std::shared_ptr<Sound>
//                        {new FileSound(SoundSample::Frequency::Hz8000,
//                         QString(":/soundSamples/soundSamples/Hz8000Left.wav"),
//                         QString(":/soundSamples/soundSamples/Hz8000Right.wav"))});
//    playlist.addSound(std::shared_ptr<Sound>
//                        {new FileSound(SoundSample::Frequency::Hz2000,
//                         QString(":/soundSamples/soundSamples/Hz2000Left.wav"),
//                         QString(":/soundSamples/soundSamples/Hz2000Right.wav"))});
//    playlist.addSound(std::shared_ptr<Sound>
//                        {new FileSound(SoundSample::Frequency::Hz4000,
//                         QString(":/soundSamples/soundSamples/Hz4000Left.wav"),
//                         QString(":/soundSamples/soundSamples/Hz4000Right.wav"))});



    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz125, 45.0);
    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz250, 15.0);
    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz500, 9.0);
    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz1000, 3.0);
    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz2000, -3.0);
    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz4000, -4.0);
    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz8000, 13.0);


//    volumesHL->addVolume(70);
//    volumesHL->addVolume(60);
    volumesHL->addVolume(50);
//    volumesHL->addVolume(40);
//    volumesHL->addVolume(30);
//    volumesHL->addVolume(20);
//    volumesHL->addVolume(10);
//    volumesHL->addVolume(0);
    volumesHL->addVolume(MAX_AVAILABLE_VOLUME_DB);
    volumesHL->addVolume(90);
//    volumesHL->addVolume(80);


    playlist.setVolumeAlgoritm(volumesHL);
}

void AppEngine::resetVariables()
{
    audiogramPlotData.clear();
}

void AppEngine::connectAll()
{
    connect(rootObj, SIGNAL(playSequence(int)), this, SLOT(playPlaylist(int)));
    connect(rootObj, SIGNAL(heardButtonClicked()), this, SLOT(onHearingButtonClicked()));
    connect(player, SIGNAL(currentPlaylistElement(AudiogramData)), this, SLOT(onCurrentPlaylistElement(AudiogramData)));
    connect(player, SIGNAL(playlistEnded()), this, SLOT(onPlaylistEnded()));
    connect(player, SIGNAL(aboutToPlayNextElement()), this, SLOT(onAboutToPlayNextElement()));
}


AppEngine::AppEngine(QObject *parent)
    : QObject(parent), rootObj(nullptr),
      volumesPercent{new VolumePercentLevel},
      volumesSPL{new VolumeDecibelSoundPressureLevel},
      volumesHL{new VolumeDecibelHearingLevel}
{
    audioFormat.setSampleRate(44100);
    audioFormat.setChannelCount(2);
    audioFormat.setSampleSize(16);
    audioFormat.setCodec("audio/pcm");
    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    audioFormat.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(audioFormat))
        audioFormat = info.nearestFormat(audioFormat);


    player = new SoundPlayer(audioFormat, QAudioDeviceInfo::defaultOutputDevice(), this);
    createPlaylist();
    player->setPlaylist(&playlist);
}

AppEngine::~AppEngine()
{
    delete player;
}

void AppEngine::setRootQmlObject(QObject *rootQmlObj)
{
    rootObj = rootQmlObj;
}

void AppEngine::onPlaylistEnded()
{
    player->resetPlaylist();

    emit playlistEnded();
}

void AppEngine::playPlaylist(int direction)
{
    resetVariables();
    switch(direction)
    {
    case static_cast<int>(SoundSample::Direction::Left):
        player->playPlaylist(SoundSample::Direction::Left); break;
    case static_cast<int>(SoundSample::Direction::Right):
        player->playPlaylist(SoundSample::Direction::Right); break;
    default: setTopBarMsg(tr("Channel does not exist")); break;
    }
}

void AppEngine::onCurrentPlaylistElement(const AudiogramData &data)
{
    canSkipTrack = false;
    currentAudiogramData = data;
    if(currentAudiogramData.getVolumeDb() == MAX_AVAILABLE_VOLUME_DB)
    {
        audiogramDataTemp.append(currentAudiogramData);
        canSkipTrack = true;
    }

    qDebug () << currentAudiogramData;
}

void AppEngine::onHearingButtonClicked()
{
    audiogramDataTemp.append(currentAudiogramData);
    canSkipTrack = false;
}

void AppEngine::onAboutToPlayNextElement()
{
    if(previousAudiogramData.getFrequency()
            != currentAudiogramData.getFrequency())
    {
        if(!audiogramDataTemp.isEmpty())
        {
            auto audiogramData = audiogramDataTemp.takeLast();
            audiogramPlotData.update(audiogramData);
            canSkipTrack = true;
        }
        else
            canSkipTrack = false;
    }

    if(canSkipTrack && !audiogramDataTemp.isEmpty())
    {
        auto audiogramData = audiogramDataTemp.takeLast();
        audiogramDataTemp.clear();
        audiogramDataTemp.append(audiogramData);
        player->skipCurrentSoundSet();
    }

    previousAudiogramData = currentAudiogramData;
}

void AppEngine::setTopBarMsg(QString topBarMsg)
{
    if (m_topBarMsg == topBarMsg)
        return;

    m_topBarMsg = topBarMsg;
    emit topBarMsgChanged(topBarMsg);
}
