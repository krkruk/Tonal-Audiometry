#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <QList>
#include <QPair>
#include <memory>
#include "sound.h"

class Playlist;
class QIODevice;

class PlaylistIter
{
public:
    PlaylistIter(const Playlist *playlist);
    PlaylistIter(const PlaylistIter &other);
    virtual ~PlaylistIter() {}

    virtual bool hasNext() = 0;
    virtual QPair<QIODevice*, int> nextLeft() = 0;
    virtual QPair<QIODevice*, int> nextRight() = 0;

    virtual void skipCurrentSoundSet() = 0;
    virtual void reset();
    virtual int getCurrentFrequency() const;
    virtual int getCurrentVolumeDb() const;
    virtual int getCurrentVolumePercent() const;

private:
    const QList<std::shared_ptr<Sound>>::const_iterator soundIterEnd;
    const QList<int>::const_iterator volumeIterEnd;
    const QList<std::shared_ptr<Sound>>::const_iterator soundIterBegin;
    const QList<int>::const_iterator volumeIterBegin;

protected:
    const Playlist *playlist;
    QList<std::shared_ptr<Sound>>::const_iterator soundIterator;
    QList<int>::const_iterator volumeIterator;

    QList<std::shared_ptr<Sound>>::const_iterator soundEnd() const { return soundIterEnd; }
    QList<int>::const_iterator volumeEnd() const { return volumeIterEnd; }
    QList<std::shared_ptr<Sound>>::const_iterator soundBegin() const { return soundIterBegin; }
    QList<int>::const_iterator volumeBegin() const { return volumeIterBegin; }
    void resetSoundIterator();
    void resetVolumeIterator();
};

class PlaylistIterVolumeSequence : public PlaylistIter
{
    bool startIterationPointFlag {true};

public:
    PlaylistIterVolumeSequence(const Playlist *playlist);
    PlaylistIterVolumeSequence(const PlaylistIterVolumeSequence &other);
    ~PlaylistIterVolumeSequence();

    bool hasNext();
    QPair<QIODevice *, int> nextLeft();
    QPair<QIODevice *, int> nextRight();

    void skipCurrentSoundSet();
    void reset();

    PlaylistIterVolumeSequence operator=(const PlaylistIterVolumeSequence &p);
};

class Playlist : public QObject
{
    Q_OBJECT

    friend class PlaylistIter;

public:
    static int decibelToPercent(int decibel);
    static int percentToDecibel(int percent);

    explicit Playlist(QObject *parent = 0);
    Playlist(const Playlist &playlist, QObject *parent = 0);
    Playlist(Playlist &&playlist, QObject *parent = 0);
    ~Playlist();

    void addSound(const std::shared_ptr<Sound> &sound);
    void addVolumeInPercent(int volume);
    void addVolumeInDb(int volume);

    QList<int> getVolumeListPercent() const;
    QList<int> getVolumeListDb();

    PlaylistIterVolumeSequence *iterator() const;

private:
    QList<std::shared_ptr<Sound>> soundOrder;
    QList<int> volumePercentOrder;
};

#endif // PLAYLIST_H
