#ifndef AUDIOPLAYERCONTROLLER_H
#define AUDIOPLAYERCONTROLLER_H

#include <QObject>
#include <QList>
#include <QUrl>
#include "audioplayermodel.h"

class AudioPlayer;

class AudioPlayerController : public QObject
{
    Q_OBJECT

public:
    explicit AudioPlayerController(QObject *parent = nullptr);
    ~AudioPlayerController();

    Q_INVOKABLE void changeIndexToNext();
    Q_INVOKABLE void changeIndexToPrevious();
    Q_INVOKABLE void changeCurrentSongIndex();
    Q_INVOKABLE AudioPlayerModel *model();
    Q_INVOKABLE void deleteSong(int songIndex);
    Q_INVOKABLE void shuffleSongs();
    Q_INVOKABLE void sortSongs();
    Q_INVOKABLE void setIndexOfIndices(int songIndex);
    Q_INVOKABLE QList<QUrl> getPlaylist();
    Q_INVOKABLE void getMyMusic();
    Q_INVOKABLE Song* getSong(int index);
    Q_INVOKABLE Autor* getAutorOfSong(int index);
    Q_INVOKABLE QList<Genre*> getGenresOfSong(int index);
    Q_INVOKABLE QList<CoAutor*> getCoAutorsOfSong(int index);

    /* Методи запитів до сервера */
    Q_INVOKABLE void connectToServer();
    Q_INVOKABLE void getMusicFileFromServer(QString msg);
    Q_INVOKABLE void getAllMusicFilesInfoFromServer();

public slots:
    void stopThePlayer();
signals:
    void stopPlayingMusic();
    void currentSongIndexChanged();
    void playlistHasBeenChanged();
    void modelHasBeenChanged();
    void createServerConn();
    void connectionFailed();
    void serverReadyToRequest();
    void getMusicFile(QString);
    void getAllMusicFiles();
private:
    AudioPlayerModel *m_model;
    QString m_filepath;

};

#endif // AUDIOPLAYERCONTROLLER_H
