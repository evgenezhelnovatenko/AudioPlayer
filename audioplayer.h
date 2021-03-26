#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QAbstractListModel>
#include <QObject>
#include <QFile>

class AudioPlayer : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString filepath READ filepath NOTIFY filepathChanged)
    Q_PROPERTY(int currentSongIndex READ currentSongIndex WRITE setCurrentSongIndex NOTIFY currentSongIndexChanged)
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

    void switchToNextSong();
    void switchToPrevSong();
    void songChange();
    void setCurrentSongIndex(int index);

signals:
    void filepathChanged(QString filepath);
    void songsAreOver();
    void currentSongIndexChanged(int index);

private:

    bool isPositionValid(const size_t position) const;
    bool readingSongsFromMySongsFile();

    std::vector<QString> m_playlist;
    int m_currentSongIndex;
    QString m_filepath;
    QFile *mySongsFile = nullptr;
    const QString PATH_TO_MY_SONGS_FILE = ":/filesWithMusic/mySongs.txt";

};

#endif // AUDIOPLAYER_H
