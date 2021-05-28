#include <QTextStream>
#include <QDebug>
#include <QMediaPlayer>
#include <QDir>
#include <QCoreApplication>
#include <QAuthenticator>
#include <QMetaEnum>
#include <algorithm>
#include <random>

#include "audioplayermodel.h"

AudioPlayerModel::AudioPlayerModel(QObject *parent)
    : QAbstractListModel {parent}
    , indexOfIndices (-1)
    , mySongsFile (new QFile(this))
    , file (nullptr)
    , m_client (new FtpClient)
{
//    qDebug() << "current thread id: " << QThread::currentThreadId();
    m_client->moveToThread(&serverThread);

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

    connect(this, &AudioPlayerModel::newSongsListChanged, this, &AudioPlayerModel::addNewSongs);
    connect(&serverThread, &QThread::finished, m_client, &QObject::deleteLater);
    connect(this, &AudioPlayerModel::getMusicFile, m_client, &FtpClient::getMusicFile);
    connect(this, &AudioPlayerModel::getAllMusicFiles, m_client, &FtpClient::getAllMusicFilesInfo);

    serverThread.start();
}

AudioPlayerModel::~AudioPlayerModel()
{
    delete m_client;

    serverThread.quit();
    serverThread.wait();

    delete mySongsFile;
}

QHash<int, QByteArray> AudioPlayerModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[AudioPlayerModelRoles::SourceRole] = "source";

    return roles;
}

int AudioPlayerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_playlist.size();
}

QVariant AudioPlayerModel::data(const QModelIndex &index, int role) const
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

void AudioPlayerModel::changeIndexToNext()
{
    indexOfIndices++;
    changeCurrentSongIndex();
}

void AudioPlayerModel::changeIndexToPrevious()
{
    indexOfIndices--;
    changeCurrentSongIndex();
}

void AudioPlayerModel::setCurrentSongIndex(int index)
{
    if (index == m_currentSongIndex)
        return;

    m_currentSongIndex = index;
    emit currentSongIndexChanged(m_currentSongIndex);
}

void AudioPlayerModel::setnewSongsList(QList<QUrl> newSongsList)
{
    if (m_newSongsList == newSongsList)
        return;

    m_newSongsList = newSongsList;
    emit newSongsListChanged(m_newSongsList);
}

void AudioPlayerModel::setIndexOfIndices(int indexOfIndices)
{
    if (!isPositionValid(indexOfIndices)) {
        return;
    }

    this->indexOfIndices = indexOfIndices;
}

int AudioPlayerModel::calculateIndexOfIndices(int songIndex)
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

void AudioPlayerModel::addNewSongs()
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

void AudioPlayerModel::deleteSong(int songIndex)
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

void AudioPlayerModel::shuffleSongsIndices()
{
    static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);

    indices.erase(indices.begin() + indexOfIndices);
    std::shuffle(indices.begin(), indices.end(), generator);
    indices.insert(indices.begin(), m_currentSongIndex);
    indexOfIndices = 0;
}

void AudioPlayerModel::sortSongsIndices()
{
    indexOfIndices = m_currentSongIndex;

    std::sort(indices.begin(), indices.end());
}

QList<QUrl> AudioPlayerModel::playlist()
{
    return m_playlist;
}

bool AudioPlayerModel::isPositionValid(const size_t position) const
{
    return (position < indices.size() && position >= 0);
}

bool AudioPlayerModel::readingSongsFromMySongsFile() // Зчитування списку пісень з файлу та запис їх у m_playlist.
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

FtpClient *AudioPlayerModel::client()
{
    return m_client;
}

void AudioPlayerModel::changeCurrentSongIndex()
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

void AudioPlayerModel::dubbingToSongsFile()
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

void AudioPlayerModel::fillingTheVectorOfIndices()
{
    for (size_t i = 0; i < indices.size(); i++) {
        indices[i] = i;
    }
}


