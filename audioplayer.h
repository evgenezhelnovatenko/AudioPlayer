#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QAbstractListModel>
#include <QObject>
#include <QFile>
#include <QStringList>

class AudioPlayer : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString filepath READ filepath NOTIFY filepathChanged)
    Q_PROPERTY(int currentSongIndex READ currentSongIndex WRITE setCurrentSongIndex NOTIFY currentSongIndexChanged)
    Q_PROPERTY(QStringList newSongsList READ newSongsList WRITE setnewSongsList NOTIFY newSongsListChanged)
public:
    AudioPlayer(QObject *parent = nullptr);
    ~AudioPlayer();

    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QString filepath() const
    {
        return m_filepath;
    }

    int currentSongIndex() const
    {
        return m_currentSongIndex;
    }

    QStringList newSongsList() const
    {
        return m_newSongsList;
    }

    void switchToNextSong();
    void switchToPrevSong();
    void songChange();
    void setCurrentSongIndex(int index);

    void setnewSongsList(QStringList newSongsList);

signals:
    void filepathChanged(QString filepath);
    void songsAreOver();
    void currentSongIndexChanged(int index);

    void newSongsListChanged(QStringList newSongsList);

private:

    bool isPositionValid(const size_t position) const;
    bool readingSongsFromMySongsFile();

    std::vector<QString> m_playlist;
    int m_currentSongIndex;
    QString m_filepath;
    QStringList m_newSongsList;
    QFile *mySongsFile = nullptr;
    const QString PATH_TO_MY_SONGS_FILE = ":/filesWithMusic/mySongs.txt";

};

#endif // AUDIOPLAYER_H
