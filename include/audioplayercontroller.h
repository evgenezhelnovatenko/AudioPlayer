#ifndef AUDIOPLAYERCONTROLLER_H
#define AUDIOPLAYERCONTROLLER_H

#include <QObject>
#include <QList>
#include <QUrl>
#include <audioplayer.h>

class AudioPlayerController : public QObject
{
    Q_OBJECT

public:
    explicit AudioPlayerController(QObject *parent = nullptr);

    Q_INVOKABLE void changeIndexToNext();
    Q_INVOKABLE void changeIndexToPrevious();
    Q_INVOKABLE void changeCurrentSongIndex();
    Q_INVOKABLE AudioPlayer *getModel();
    Q_INVOKABLE void downloadJsonData();
    Q_INVOKABLE void deleteSong(int songIndex);
    Q_INVOKABLE void shuffleSongs();
    Q_INVOKABLE void sortSongs();
    Q_INVOKABLE void setIndexOfIndices(int songIndex);
    Q_INVOKABLE QList<QUrl> getPlaylist();

public slots:
    void stopThePlayer();
signals:
    void stopPlayingMusic();
    void currentSongIndexChanged();
    void playlistHasBeenChanged();
    void modelHasBeenChanged();
private:
    AudioPlayer player;
    QString m_filepath;


};

#endif // AUDIOPLAYERCONTROLLER_H
