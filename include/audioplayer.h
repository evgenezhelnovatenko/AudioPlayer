#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QAbstractListModel>
#include <QObject>
#include <QFile>
#include <QList>
#include <QUrl>

class AudioPlayer : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString filepath READ filepath NOTIFY filepathChanged)
    Q_PROPERTY(int currentSongIndex READ currentSongIndex WRITE setCurrentSongIndex NOTIFY currentSongIndexChanged)
    Q_PROPERTY(QList<QUrl> newSongsList READ newSongsList WRITE setnewSongsList NOTIFY newSongsListChanged)
public:
    AudioPlayer(QObject *parent = nullptr);
    ~AudioPlayer();

    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QString filepath() const
    {
        return m_filepath;
    }

    int currentSongIndex() const
    {
        return m_currentSongIndex;
    }

    QList<QUrl> newSongsList() const
    {
        return m_newSongsList;
    }

    void switchToNextSong();
    void switchToPrevSong();
    void songChange();
    void setCurrentSongIndex(int index);
    void setnewSongsList(QList<QUrl> newSongsList);
    void addNewSongs();

signals:
    void filepathChanged(QString filepath);
    void songsAreOver();
    void currentSongIndexChanged(int index);
    void newSongsListChanged(QList<QUrl> newSongsList);
    void newSongsWasAdded();

private:

    bool isPositionValid(const size_t position) const;
    bool readingSongsFromMySongsFile();

    QString pathToMySongsFile = "";

    std::vector<QString> m_playlist;
    int m_currentSongIndex;
    QString m_filepath;
    QList<QUrl> m_newSongsList;
    QFile *mySongsFile = nullptr;

};

#endif // AUDIOPLAYER_H
