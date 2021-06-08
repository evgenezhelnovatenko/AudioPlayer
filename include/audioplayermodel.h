#ifndef AUDIOPLAYERMODEL_H
#define AUDIOPLAYERMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QFile>
#include <QList>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QThread>

#include "ftpclient.h"
#include "serializer.h"

class Client;

class AudioPlayerModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentSongIndex READ currentSongIndex WRITE setCurrentSongIndex NOTIFY currentSongIndexChanged)
    Q_PROPERTY(QList<QUrl> newSongsList READ newSongsList WRITE setnewSongsList NOTIFY newSongsListChanged)
    Q_PROPERTY(QString downloadFolder READ downloadFolder WRITE setDownloadFolder NOTIFY downloadFolderChanged)


public:
    AudioPlayerModel(QObject *parent = nullptr);
    ~AudioPlayerModel();

    enum AudioPlayerRoles {
        SourceRole = Qt::UserRole + 1,
        TitleRole,
        UrlRole,
        YearRole,
        LengthRole,

    };

    // QAbstractItemModel interface
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;


    int currentSongIndex() const
    {
        return m_currentSongIndex;
    }

    QList<QUrl> newSongsList() const
    {
        return m_newSongsList;
    }

    void changeIndexToNext(); // Зміна поточного індексу пісні на наступний.
    void changeIndexToPrevious(); // Зміна поточного індексу пісні на попередній.
    void changeCurrentSongIndex(); // Зміна поточного індексу.
    void addNewSongs(); // Додаваня списку нової музики у поточний список музики.
    void deleteSong(int songIndex); // Видалення пісні за індексом.
    void shuffleSongsIndices();
    void sortSongsIndices();
    std::vector<Song> playlist();
    void setIndexOfIndices(int indexOfIndices); // Встановлення індексу вектора індексів.
    int calculateIndexOfIndices(int songIndex);
    Song* getRow(int index);


    FtpClient *client();


    QString downloadFolder() const;

public slots:
    void setnewSongsList(QList<QUrl> newSongsList); // Зміна списку нової музики.
    void setCurrentSongIndex(int index); // Зміна поточного індексу музики.
    void setSonglistAsActiveSongList(const QList<Song>& songlist);
    void setPlaylistAsActiveSongList();
    void onConnectionFailed();

    void setDownloadFolder(QString downloadFolder);

signals:
    void stopThePlayer();
    void currentSongIndexChanged(int index);
    void newSongsListChanged(QList<QUrl> newSongsList);
    void newSongsWasAdded();
    void getMusicFile(QString);
    void getAllMusicFiles();
    void connectToServer();
    void connectionFailed();
    void serverReadyToRequest();

    void downloadFolderChanged(QString downloadFolder);

private:
    void dubbingToSongsFile(); // Перезапис пісень у файл mySongs.txt.
    void fillingTheVectorOfIndices(); // Заповнення вектору індексів.
    void m_clientInitialization();

    void receiveSongListFromServer(const QList<Song>& songlist);

    bool isPositionValid(const size_t position) const;
    bool readingSongsFromMySongsFile();

    QString pathToMySongsFile = "";

    std::vector<Song> m_playlist; // Список пісень на комп'ютері.
    std::vector<Song> m_songlist; // Список пісень з серверу.
    std::vector<Song>* m_pointerToActiveList = nullptr; // Вказівник на необхідний список пісень.
    std::vector<size_t> indices;
    int indexOfIndices;
    int m_currentSongIndex;
    QList<QUrl> m_newSongsList;
    QFile *mySongsFile = nullptr;
    std::unique_ptr<QFile> file;
    QString filename;
    FtpClient* m_client = nullptr;
    QThread serverThread;

    QString m_downloadFolder;
};

#endif // AUDIOPLAYERMODEL_H
