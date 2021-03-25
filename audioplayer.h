#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QAbstractListModel>
#include <QObject>
#include <QFile>

class AudioPlayer : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString filepath READ filepath NOTIFY filepathChanged)
public:
    AudioPlayer(QObject *parent = nullptr);
    ~AudioPlayer();

    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QString filepath() const
    {
        return m_filepath;
    }

    void switchToNextSong();
    void switchToPrevSong();

signals:
    void filepathChanged(QString filepath);
    void songsAreOver();

private:

    bool isPositionValid(const size_t position) const;
    bool readingSongsFromMySongsFile();
    void songChange();

    std::vector<QString> m_playlist;
    int currentSongIndex = 0;
    QString m_filepath;
    QFile *mySongsFile = nullptr;
    const QString PATH_TO_MY_SONGS_FILE = ":/filesWithMusic/mySongs.txt";


};

#endif // AUDIOPLAYER_H
