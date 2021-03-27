#include <QTextStream>
#include <QDebug>
#include <QMediaPlayer>
#include <QDir>
#include <QCoreApplication>

#include "audioplayer.h"

AudioPlayer::AudioPlayer(QObject *parent)
    : QAbstractListModel {parent}
    , m_currentSongIndex (0)
    , mySongsFile (new QFile(this))
{
    pathToMySongsFile = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()) + "/mySongs.txt";
    mySongsFile->setFileName(pathToMySongsFile);

    if (!readingSongsFromMySongsFile()) {
        qDebug() << "Файл з музикою пошкоджено або видалено!";
        return;
    }

    m_filepath = m_playlist[m_currentSongIndex];


    connect(this, &AudioPlayer::newSongsListChanged, this, &AudioPlayer::addNewSongs);
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

    return QVariant::fromValue(m_playlist[index_row]);
}

void AudioPlayer::switchToNextSong()
{
    m_currentSongIndex++;
    songChange();
}

void AudioPlayer::switchToPrevSong()
{
    m_currentSongIndex--;
    songChange();
}

void AudioPlayer::setCurrentSongIndex(int index)
{
    if (index == m_currentSongIndex)
        return;

    m_currentSongIndex = index;
    emit currentSongIndexChanged(m_currentSongIndex);
}

void AudioPlayer::setnewSongsList(QList<QUrl> newSongsList)
{
    if (m_newSongsList == newSongsList)
        return;

    m_newSongsList = newSongsList;
    emit newSongsListChanged(m_newSongsList);
}

void AudioPlayer::addNewSongs()
{
    if (!mySongsFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        qDebug() << "Помилка при додаванні нових файлів. Файл з музикою пошкоджено або видалено!";
        return;
    }

    QTextStream out(mySongsFile);
    beginInsertRows(QModelIndex(), m_playlist.size(), m_playlist.size() + m_newSongsList.size() - 1);
    for (int i = 0; i < m_newSongsList.size(); i++) {

        QString newSong = "\n" + m_newSongsList.at(i).toLocalFile();
        out << newSong;
        m_playlist.push_back(newSong);
    }
    endInsertRows();

    mySongsFile->close();

}

bool AudioPlayer::isPositionValid(const size_t position) const
{
    return (position < m_playlist.size() && position >= 0);
}

bool AudioPlayer::readingSongsFromMySongsFile()
{

    if (!mySongsFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(mySongsFile);
    while (!in.atEnd()) {
        QString path = in.readLine();
        m_playlist.push_back(path);
    }

    mySongsFile->close();

    return true;
}

void AudioPlayer::songChange()
{
    if (!isPositionValid(m_currentSongIndex)) {
        emit songsAreOver();
        m_currentSongIndex = 0;
    }
    m_filepath = m_playlist[m_currentSongIndex];
    emit filepathChanged(m_filepath);
    emit currentSongIndexChanged(m_currentSongIndex);
}
