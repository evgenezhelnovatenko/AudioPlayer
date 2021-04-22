#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QAbstractListModel>
#include <QObject>
#include <QFile>
#include <QList>
#include <QUrl>
#include <QNetworkAccessManager>

class AudioPlayer : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentSongIndex READ currentSongIndex WRITE setCurrentSongIndex NOTIFY currentSongIndexChanged)
    Q_PROPERTY(QList<QUrl> newSongsList READ newSongsList WRITE setnewSongsList NOTIFY newSongsListChanged)
public:
    AudioPlayer(QObject *parent = nullptr);
    ~AudioPlayer();

    enum AudioPlayerRoles {
        SourceRole = Qt::UserRole + 1
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
    QList<QUrl> playlist();
    void downloadJsonData();
    void startRequest(const QUrl &requestedUrl);


public slots:
    void setnewSongsList(QList<QUrl> newSongsList); // Зміна списку нової музики.
    void setCurrentSongIndex(int index); // Зміна поточного індексу музики.
    void setIndexOfIndices(int songIndex); // Встановлення індексу вектора індексів.

signals:
    void stopThePlayer();
    void currentSongIndexChanged(int index);
    void newSongsListChanged(QList<QUrl> newSongsList);
    void newSongsWasAdded();


private slots:

    void httpReadyRead();
    void httpFinished();
    void slotAuthenticationRequired(QNetworkReply *, QAuthenticator *authenticator);
    #ifndef QT_NO_SSL
        void sslErrors(QNetworkReply *, const QList<QSslError> &errors);
    #endif

private:
    void dubbingToSongsFile(); // Перезапис пісень у файл mySongs.txt.
    void fillingTheVectorOfIndices(); // Заповнення вектору індексів.

    std::unique_ptr<QFile> openFileForWrite(const QString &fileName);
    QJsonObject parseReply(QNetworkReply *reply);
    QNetworkRequest createRequest();

    bool isPositionValid(const size_t position) const;
    bool readingSongsFromMySongsFile();

    QString pathToMySongsFile = "";

    std::vector<QString> m_playlist;
    std::vector<size_t> indices;
    int indexOfIndices;
    int m_currentSongIndex;
    QList<QUrl> m_newSongsList;
    QFile *mySongsFile = nullptr;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QUrl url;
    std::unique_ptr<QFile> file;
    QString filename;
    QSslConfiguration *sslConfig;


};

#endif // AUDIOPLAYER_H
