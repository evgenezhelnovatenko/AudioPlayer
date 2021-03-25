#include <QTextStream>
#include <QDebug>
#include <QMediaPlayer>

#include "audioplayer.h"

AudioPlayer::AudioPlayer(QObject *parent)
    : QAbstractListModel {parent}
    , mySongsFile (new QFile(this))
{
    if (!readingSongsFromMySongsFile()) {
        qDebug() << "Файл з музикою пошкоджено або видалено!";
        return;
    }

    m_filepath = m_playlist[currentSongIndex];
}

AudioPlayer::~AudioPlayer()
{
    delete mySongsFile;
}

int AudioPlayer::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_playlist.size();
}

QVariant AudioPlayer::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return {};
    }

    const auto index_row {static_cast<size_t>(index.row())};

    if (!isPositionValid(index_row)) {
        return {};
    }

    return QVariant(m_playlist[index_row]);
}

void AudioPlayer::switchToNextSong()
{
    currentSongIndex++;
    songChange();
}

void AudioPlayer::switchToPrevSong()
{
    currentSongIndex--;
    songChange();
}

bool AudioPlayer::isPositionValid(const size_t position) const
{
    return (position < m_playlist.size() && position >= 0);
}

bool AudioPlayer::readingSongsFromMySongsFile()
{
    mySongsFile->setFileName(PATH_TO_MY_SONGS_FILE);

    if (!mySongsFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(mySongsFile);
    while (!in.atEnd()) {
        QString path = in.readLine();
        m_playlist.push_back(path);
    }

    return true;
}

void AudioPlayer::songChange()
{
    if (!isPositionValid(currentSongIndex)) {
        emit songsAreOver();
        currentSongIndex = 0;
    }
    m_filepath = m_playlist[currentSongIndex];
    emit filepathChanged(m_filepath);

}
