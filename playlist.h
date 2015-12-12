#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <QList>
#include <QPair>
#include <memory>
#include "sound.h"
#include "volume.h"

class Playlist;
class QIODevice;

class PlaylistIter
{
public:
    PlaylistIter(const Playlist *playlist);
    PlaylistIter(const PlaylistIter &other);
    virtual ~PlaylistIter() {}

    virtual bool hasNext() = 0;
    virtual QPair<QIODevice*, qreal> nextLeft();
    virtual QPair<QIODevice*, qreal> nextRight();

    virtual void skipCurrentSoundSet() = 0;
    virtual void reset();
    virtual int getCurrentFrequency() const;
    virtual qreal getCurrentVolumeDb() const;
    virtual qreal getCurrentVolumePercent() const;

private:
    const QList<std::shared_ptr<Sound>>::const_iterator soundIterEnd;
    const QList<qreal>::const_iterator volumeIterEnd;
    const QList<std::shared_ptr<Sound>>::const_iterator soundIterBegin;
    const QList<qreal>::const_iterator volumeIterBegin;

protected:
    const Playlist *playlist;
    QList<std::shared_ptr<Sound>>::const_iterator soundIterator;
    QList<qreal>::const_iterator volumeIterator;

    QList<std::shared_ptr<Sound>>::const_iterator soundEnd() const { return soundIterEnd; }
    QList<qreal>::const_iterator volumeEnd() const { return volumeIterEnd; }
    QList<std::shared_ptr<Sound>>::const_iterator soundBegin() const { return soundIterBegin; }
    QList<qreal>::const_iterator volumeBegin() const { return volumeIterBegin; }
    void resetSoundIterator();
    void resetVolumeIterator();
    qreal calibrateVolume(qreal input);
};

class PlaylistIterVolumeSequence : public PlaylistIter
{
    bool startIterationPointFlag {true};

public:
    PlaylistIterVolumeSequence(const Playlist *playlist);
    PlaylistIterVolumeSequence(const PlaylistIterVolumeSequence &other);
    ~PlaylistIterVolumeSequence();

    bool hasNext();

    void skipCurrentSoundSet();
    void reset();

    PlaylistIterVolumeSequence operator=(const PlaylistIterVolumeSequence &p);
};

class Playlist : public QObject
{
    Q_OBJECT

    friend class PlaylistIter;

public:
    explicit Playlist(QObject *parent = 0);
    Playlist(const Playlist &playlist, QObject *parent = 0);
    Playlist(Playlist &&playlist, QObject *parent = 0);
    ~Playlist();

    void addSound(const std::shared_ptr<Sound> &sound);
    void addSound(const QList<std::shared_ptr<Sound>> &sounds);
    void setVolumeAlgoritm(const std::shared_ptr<VolumeAlgorithm> &volumeAlgorith);

    PlaylistIterVolumeSequence *iterator() const;

private:
    QList<std::shared_ptr<Sound>> soundOrder;
    std::shared_ptr<VolumeAlgorithm> volumeOrder;
};

#endif // PLAYLIST_H
