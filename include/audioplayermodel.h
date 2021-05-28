#ifndef AUDIOPLAYERMODEL_H
#define AUDIOPLAYERMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QFile>
#include <QList>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QThread>
#include <list>

#include "ftpclient.h"
#include "serializer.h"

class Client;

class AudioPlayerModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentSongIndex READ currentSongIndex WRITE setCurrentSongIndex NOTIFY currentSongIndexChanged)
    Q_PROPERTY(QList<QUrl> newSongsList READ newSongsList WRITE setnewSongsList NOTIFY newSongsListChanged)


public:
    AudioPlayerModel(QObject *parent = nullptr);
    ~AudioPlayerModel();

    enum AudioPlayerModelRoles {
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

    void changeIndexToNext(); // Р—РјС–РЅР° РїРѕС‚РѕС‡РЅРѕРіРѕ С–РЅРґРµРєСЃСѓ РїС–СЃРЅС– РЅР° РЅР°СЃС‚СѓРїРЅРёР№.
    void changeIndexToPrevious(); // Р—РјС–РЅР° РїРѕС‚РѕС‡РЅРѕРіРѕ С–РЅРґРµРєСЃСѓ РїС–СЃРЅС– РЅР° РїРѕРїРµСЂРµРґРЅС–Р№.
    void changeCurrentSongIndex(); // Р—РјС–РЅР° РїРѕС‚РѕС‡РЅРѕРіРѕ С–РЅРґРµРєСЃСѓ.
    void addNewSongs(); // Р”РѕРґР°РІР°РЅСЏ СЃРїРёСЃРєСѓ РЅРѕРІРѕС— РјСѓР·РёРєРё Сѓ РїРѕС‚РѕС‡РЅРёР№ СЃРїРёСЃРѕРє РјСѓР·РёРєРё.
    void deleteSong(int songIndex); // Р’РёРґР°Р»РµРЅРЅСЏ РїС–СЃРЅС– Р·Р° С–РЅРґРµРєСЃРѕРј.
    void shuffleSongsIndices();
    void sortSongsIndices();
    QList<QUrl> playlist();
    void setIndexOfIndices(int indexOfIndices); // Р’СЃС‚Р°РЅРѕРІР»РµРЅРЅСЏ С–РЅРґРµРєСЃСѓ РІРµРєС‚РѕСЂР° С–РЅРґРµРєСЃС–РІ.
    int calculateIndexOfIndices(int songIndex);

    FtpClient *client();


public slots:
    void setnewSongsList(QList<QUrl> newSongsList); // Р—РјС–РЅР° СЃРїРёСЃРєСѓ РЅРѕРІРѕС— РјСѓР·РёРєРё.
    void setCurrentSongIndex(int index); // Р—РјС–РЅР° РїРѕС‚РѕС‡РЅРѕРіРѕ С–РЅРґРµРєСЃСѓ РјСѓР·РёРєРё.

signals:
    void stopThePlayer();
    void currentSongIndexChanged(int index);
    void newSongsListChanged(QList<QUrl> newSongsList);
    void newSongsWasAdded();
    void getMusicFile(QString);
    void getAllMusicFiles();

private:
    void dubbingToSongsFile(); // РџРµСЂРµР·Р°РїРёСЃ РїС–СЃРµРЅСЊ Сѓ С„Р°Р№Р» mySongs.txt.
    void fillingTheVectorOfIndices(); // Р—Р°РїРѕРІРЅРµРЅРЅСЏ РІРµРєС‚РѕСЂСѓ С–РЅРґРµРєСЃС–РІ.

    bool isPositionValid(const size_t position) const;
    bool readingSongsFromMySongsFile();

    QString pathToMySongsFile = "";

    QList<QUrl> m_playlist;
    std::vector<size_t> indices;
    int indexOfIndices;
    int m_currentSongIndex;
    QList<QUrl> m_newSongsList;
    QFile *mySongsFile = nullptr;
    std::unique_ptr<QFile> file;
    QString filename;
    FtpClient* m_client = nullptr;
    QThread serverThread;

};

#endif // AUDIOPLAYERMODEL_H
