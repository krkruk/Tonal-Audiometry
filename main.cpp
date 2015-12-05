#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QAudioOutput>
#include "filesound.h"
#include "soundplayer.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QAudioFormat m_format;
    m_format.setSampleRate(44100);
    m_format.setChannelCount(2);
    m_format.setSampleSize(16);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::SignedInt);

    SoundPlayer player(m_format, QAudioDeviceInfo::defaultOutputDevice());
    Playlist playlist;
    playlist.addSound(std::shared_ptr<Sound>{new FileSound(SoundSample::Frequency::Hz1000,
                                             QString(":/soundSamples/soundSamples/Hz1000Left.wav"),
                                             QString(":/soundSamples/soundSamples/Hz1000Right.wav"))});
    playlist.addSound(std::shared_ptr<Sound>{new FileSound(SoundSample::Frequency::Hz2000,
                                             QString(":/soundSamples/soundSamples/Hz2000Left.wav"),
                                             QString(":/soundSamples/soundSamples/Hz2000Right.wav"))});
    playlist.addSound(std::shared_ptr<Sound>{new FileSound(SoundSample::Frequency::Hz500,
                                             QString(":/soundSamples/soundSamples/Hz500Left.wav"),
                                             QString(":/soundSamples/soundSamples/Hz500Right.wav"))});
    playlist.addSound(std::shared_ptr<Sound>{new FileSound(SoundSample::Frequency::Hz250,
                                             QString(":/soundSamples/soundSamples/Hz250Left.wav"),
                                             QString(":/soundSamples/soundSamples/Hz250Right.wav"))});
    playlist.addVolumeInPercent(100);
    playlist.addVolumeInPercent(30);

    player.setPlaylist(&playlist);

    player.playPlaylist(SoundSample::Direction::Left);

    return app.exec();
}

