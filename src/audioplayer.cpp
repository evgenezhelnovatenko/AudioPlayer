#include <QTextStream>
#include <QDebug>
#include <QMediaPlayer>
#include <QDir>
#include <QCoreApplication>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QMetaEnum>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <algorithm>
#include <random>

#include "audioplayer.h"

AudioPlayer::AudioPlayer(QObject *parent)
    : QAbstractListModel {parent}
    , indexOfIndices (-1)
    , mySongsFile (new QFile(this))
    , file (nullptr)
    , m_client (new Client(this))
{
    filename = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()) + "/file.txt";

    pathToMySongsFile = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()) + "/mySongs.txt";
    mySongsFile->setFileName(pathToMySongsFile);

    if (!readingSongsFromMySongsFile()) {
        qDebug() << "Файл з музикою пошкоджено або видалено!";
        return;
    }

    indices.resize(m_playlist.size());
    fillingTheVectorOfIndices();
    m_currentSongIndex = -1;

    connect(this, &AudioPlayer::newSongsListChanged, this, &AudioPlayer::addNewSongs);

}

AudioPlayer::~AudioPlayer()
{
    delete m_client;
    delete mySongsFile;
}

QHash<int, QByteArray> AudioPlayer::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[AudioPlayerRoles::SourceRole] = "source";

    return roles;
}

int AudioPlayer::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_playlist.size();
}

QVariant AudioPlayer::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() > rowCount(index)) {
        return {};
    }

    const auto index_row {static_cast<size_t>(index.row())};

    if (!isPositionValid(index_row)) {
        return {};
    }

    return QVariant::fromValue(m_playlist.at(index_row));
}

void AudioPlayer::changeIndexToNext()
{
    indexOfIndices++;
    changeCurrentSongIndex();
}

void AudioPlayer::changeIndexToPrevious()
{
    indexOfIndices--;
    changeCurrentSongIndex();
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

void AudioPlayer::setIndexOfIndices(int indexOfIndices)
{
    if (!isPositionValid(indexOfIndices)) {
        return;
    }

    this->indexOfIndices = indexOfIndices;
}

int AudioPlayer::calculateIndexOfIndices(int songIndex)
{
    if (!isPositionValid(songIndex)) {
        return -1;
    }

    for (size_t i = 0; i < indices.size(); i++) {
        if (indices[i] == static_cast<size_t>(songIndex)) {
            return i;
        }
    }

    return -1;
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

        QString newSong = m_newSongsList.at(i).toLocalFile();
        out << newSong << "\n";
        m_playlist.append(newSong);
        indices.push_back(indices.size());
    }
    endInsertRows();

    m_newSongsList.clear();
    mySongsFile->close();

}

void AudioPlayer::deleteSong(int songIndex)
{
    QList<QUrl>::iterator iteratorOfTheItemToRemoveFromPlaylist; // Ітератор елемента, що необхідно видалити з вектора m_playlist.
    iteratorOfTheItemToRemoveFromPlaylist = m_playlist.begin() + songIndex;
    std::vector<size_t>::const_iterator iteratorOfTheItemToRemoveFromIndices; // Ітератор елемента, що необхідно видалити з вектора indidces.
    int indexOfTheItemToRemoveFromIndices = calculateIndexOfIndices(m_playlist.size() - 1); // Індекс елемента, що необхідно видалити з вектора indidces.
    iteratorOfTheItemToRemoveFromIndices = indices.cbegin() + indexOfTheItemToRemoveFromIndices;

    beginRemoveRows(QModelIndex(), songIndex, songIndex);
    m_playlist.erase(iteratorOfTheItemToRemoveFromPlaylist);
    endRemoveRows();

    indices.erase(iteratorOfTheItemToRemoveFromIndices);

    dubbingToSongsFile();

    if (songIndex < m_currentSongIndex) {
        m_currentSongIndex--;
        indexOfIndices = calculateIndexOfIndices(m_currentSongIndex);
        emit currentSongIndexChanged(m_currentSongIndex);
    } else if (songIndex == m_currentSongIndex) {
        m_currentSongIndex = -1;
        indexOfIndices = -1;
        emit currentSongIndexChanged(m_currentSongIndex);
        emit stopThePlayer();
    }
}

void AudioPlayer::shuffleSongsIndices()
{
    static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);

    indices.erase(indices.begin() + indexOfIndices);
    std::shuffle(indices.begin(), indices.end(), generator);
    indices.insert(indices.begin(), m_currentSongIndex);
    indexOfIndices = 0;
}

void AudioPlayer::sortSongsIndices()
{
    indexOfIndices = m_currentSongIndex;

    std::sort(indices.begin(), indices.end());
}

QList<QUrl> AudioPlayer::playlist()
{
    return m_playlist;
}


std::unique_ptr<QFile> AudioPlayer::openFileForWrite(const QString &fileName)
{
    std::unique_ptr<QFile> file(new QFile(fileName));
    if (!file->open(QIODevice::WriteOnly)) {
        qDebug() << "Файл для результатів не відкривається";
        return nullptr;
    }
    return file;
}

bool AudioPlayer::isPositionValid(const size_t position) const
{
    return (position < indices.size() && position >= 0);
}

bool AudioPlayer::readingSongsFromMySongsFile() // Зчитування списку пісень з файлу та запис їх у m_playlist.
{

    if (!mySongsFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(mySongsFile);
    while (!in.atEnd()) {
        QUrl path = QUrl::fromLocalFile(in.readLine());
        m_playlist.append(path);
    }

    mySongsFile->close();

    return true;
}

Client *AudioPlayer::client()
{
    return m_client;
}

void AudioPlayer::changeCurrentSongIndex()
{
    if (!isPositionValid(indexOfIndices)) {
        emit stopThePlayer();
        indexOfIndices = -1;
        m_currentSongIndex = -1;
        emit currentSongIndexChanged(m_currentSongIndex);
        return;
    }
    m_currentSongIndex = indices[indexOfIndices];
    emit currentSongIndexChanged(m_currentSongIndex);
}

void AudioPlayer::dubbingToSongsFile()
{
    if (!mySongsFile->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        qDebug() << "Помилка при перезаписі файлу. Файл з музикою пошкоджено або видалено!";
        return ;
    }

    QTextStream out(mySongsFile);
    for (int i = 0; i < m_playlist.size(); i++) {
        QString line = m_playlist.at(i).toString(QUrl::None) + "\n";
        out << line;
    }

    mySongsFile->close();
}

void AudioPlayer::fillingTheVectorOfIndices()
{
    for (size_t i = 0; i < indices.size(); i++) {
        indices[i] = i;
    }
}


