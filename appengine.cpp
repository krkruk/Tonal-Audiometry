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

    playlist.addVolumeInPercent(100);
    playlist.addVolumeInPercent(30);
}

AppEngine::AppEngine(QObject *rootQmlObj, QObject *parent)
    : QObject(parent), rootObj(rootQmlObj)
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

