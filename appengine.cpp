#include "appengine.h"
#include <QDebug>

void AppEngine::createPlaylist()
{
    playlist.addSound(std::shared_ptr<Sound>
                        {new FileSound(SoundSample::Frequency::Hz1000,
                         QString(":/soundSamples/soundSamples/Hz1000Left.wav"),
                         QString(":/soundSamples/soundSamples/Hz1000Right.wav"))});
    playlist.addSound(std::shared_ptr<Sound>
                        {new FileSound(SoundSample::Frequency::Hz2000,
                         QString(":/soundSamples/soundSamples/Hz2000Left.wav"),
                         QString(":/soundSamples/soundSamples/Hz2000Right.wav"))});
    playlist.addSound(std::shared_ptr<Sound>
                        {new FileSound(SoundSample::Frequency::Hz500,
                         QString(":/soundSamples/soundSamples/Hz500Left.wav"),
                         QString(":/soundSamples/soundSamples/Hz500Right.wav"))});
    playlist.addSound(std::shared_ptr<Sound>
                        {new FileSound(SoundSample::Frequency::Hz250,
                         QString(":/soundSamples/soundSamples/Hz250Left.wav"),
                         QString(":/soundSamples/soundSamples/Hz250Right.wav"))});

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

    volumesSPL->addVolume(0);
    volumesSPL->addVolume(60);
    volumesSPL->addVolume(65);
    volumesSPL->addVolume(100);

    playlist.setVolumeAlgoritm(volumesSPL);
}

AppEngine::AppEngine(QObject *rootQmlObj, QObject *parent)
    : QObject(parent), rootObj(rootQmlObj),
      volumesPercent{new VolumePercentLevel},
      volumesSPL{new VolumeDecibelSoundPressureLevel}
{
    audioFormat.setSampleRate(44100);
    audioFormat.setChannelCount(2);
    audioFormat.setSampleSize(16);
    audioFormat.setCodec("audio/pcm");
    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    audioFormat.setSampleType(QAudioFormat::SignedInt);

    player = new SoundPlayer(audioFormat, QAudioDeviceInfo::defaultOutputDevice(), this);
    createPlaylist();
    player->setPlaylist(&playlist);

    connect(rootObj, SIGNAL(playSequence()), this, SLOT(playPlaylist()));
    connect(player, SIGNAL(currentPlaylistElement(AudiogramData)), this, SLOT(onCurrentPlaylistElement(AudiogramData)));
}

AppEngine::~AppEngine()
{
    delete player;
}

void AppEngine::playPlaylist()
{
    player->playPlaylist(SoundSample::Direction::Left);
}

void AppEngine::onCurrentPlaylistElement(const AudiogramData &data)
{
    qDebug() << data;
}

