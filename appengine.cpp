#include "appengine.h"
#include <QDebug>

void AppEngine::createPlaylist()
{
    playlist.addSound(std::shared_ptr<Sound>
                        {new FileSound(SoundSample::Frequency::Hz1000,
                         QString(":/soundSamples/soundSamples/Hz1000Left.wav"),
                         QString(":/soundSamples/soundSamples/Hz1000Right.wav"))});
    playlist.addSound(std::shared_ptr<Sound>
                        {new FileSound(SoundSample::Frequency::Hz8000,
                         QString(":/soundSamples/soundSamples/Hz8000Left.wav"),
                         QString(":/soundSamples/soundSamples/Hz8000Right.wav"))});
    playlist.addSound(std::shared_ptr<Sound>
                        {new FileSound(SoundSample::Frequency::Hz500,
                         QString(":/soundSamples/soundSamples/Hz500Left.wav"),
                         QString(":/soundSamples/soundSamples/Hz500Right.wav"))});
//    playlist.addSound(std::shared_ptr<Sound>
//                        {new FileSound(SoundSample::Frequency::Hz250,
//                         QString(":/soundSamples/soundSamples/Hz250Left.wav"),
//                         QString(":/soundSamples/soundSamples/Hz250Right.wav"))});

    volumesPercent->addVolume(0.00002);
    volumesPercent->addVolume(0.01);
    volumesPercent->addVolume(0.02);
    volumesPercent->addVolume(0.04);
    volumesPercent->addVolume(0.08);
    volumesPercent->addVolume(0.16);
    volumesPercent->addVolume(0.32);
    volumesPercent->addVolume(0.64);
    volumesPercent->addVolume(0.90);
    volumesPercent->addVolume(1.0);

    volumesSPL->addVolume(3);
    volumesSPL->addVolume(63);
//    volumesSPL->addVolume(30);
//    volumesSPL->addVolume(40);
//    volumesSPL->addVolume(50);
//    volumesSPL->addVolume(60);
//    volumesSPL->addVolume(65);

    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz125, 45.0);
    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz250, 15.0);
    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz500, 9.0);
    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz1000, 3.0);
    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz2000, -3.0);
    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz4000, -4.0);
    volumesHL->setDecibelHearingLevelCalibrationGain(SoundSample::Frequency::Hz8000, 13.0);
    volumesHL->addVolume(50);
    volumesHL->addVolume(40);
    volumesHL->addVolume(30);
    volumesHL->addVolume(20);
//    volumesHL->addVolume(10);
//    volumesHL->addVolume(0);
//    volumesHL->addVolume(60);
//    volumesHL->addVolume(65);
//    volumesHL->addVolume(70);
//    volumesHL->addVolume(110);

    playlist.setVolumeAlgoritm(volumesHL);
}

void AppEngine::connectAll()
{
    connect(rootObj, SIGNAL(playSequence()), this, SLOT(playPlaylist()));
    connect(rootObj, SIGNAL(heardButtonClicked()), this, SLOT(onHearingButtonClicked()));
    connect(player, SIGNAL(currentPlaylistElement(AudiogramData)), this, SLOT(onCurrentPlaylistElement(AudiogramData)));
    connect(player, SIGNAL(playlistEnded()), this, SLOT(onPlaylistEnded()));
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

void AppEngine::playPlaylist()
{
    player->playPlaylist(SoundSample::Direction::Left);
}

void AppEngine::onCurrentPlaylistElement(const AudiogramData &data)
{
    if(!hearingButtonClicked)
    {
        audiogramPlotData.update(currentAudiogramData);
        player->skipCurrentSoundSet();
    }
    currentAudiogramData = data;
    unsetHearingButton();
}

void AppEngine::onPlaylistEnded()
{
    emit playlistEnded();
    player->resetPlaylist();
    //print an audiogram

    auto audiogram = audiogramPlotData.getSortedData();
    for(auto elem : audiogram)
        qDebug() << elem;

}

void AppEngine::onHearingButtonClicked()
{
    setHearingButtonClicked();
}

