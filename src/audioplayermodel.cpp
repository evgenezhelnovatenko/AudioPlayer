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
    , m_pointerToActiveList(&m_playlist)
    , indexOfIndices (-1)
    , mySongsFile (new QFile(this))
{
    m_clientInitialization();

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
    roles[AudioPlayerRoles::SourceRole] = "source";
    roles[AudioPlayerRoles::TitleRole] = "title";
    roles[AudioPlayerRoles::UrlRole] = "url";
    roles[AudioPlayerRoles::YearRole] = "year";
    roles[AudioPlayerRoles::LengthRole] = "length";

    return roles;
}

int AudioPlayerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_pointerToActiveList->size();
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

    const Song& song {m_pointerToActiveList->at(index_row)};

    switch (role) {
    case AudioPlayerRoles::TitleRole: {
        return QVariant::fromValue(song.title());
    }
    case AudioPlayerRoles::UrlRole: {
        return QVariant::fromValue(song.url());
    }
    case AudioPlayerRoles::YearRole: {
        return QVariant::fromValue(song.year());
    }
    case AudioPlayerRoles::LengthRole: {
        return QVariant::fromValue(song.length());
    }
    default: {
        return true;
    }
    }
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

void AudioPlayerModel::setSonglistAsActiveSongList(const QList<Song>& songlist)
{
    receiveSongListFromServer(songlist);

    beginResetModel();
    m_pointerToActiveList = &m_songlist;
    endResetModel();
}

void AudioPlayerModel::setPlaylistAsActiveSongList()
{
    beginResetModel();
    m_pointerToActiveList = &m_playlist;
    endResetModel();
}

void AudioPlayerModel::onConnectionFailed()
{
    delete m_client;

    m_clientInitialization();

    emit connectionFailed();
}

void AudioPlayerModel::setDownloadFolder(QString downloadFolder)
{
    if (m_downloadFolder == downloadFolder)
        return;

    m_downloadFolder = downloadFolder;
    emit downloadFolderChanged(m_downloadFolder);
}

void AudioPlayerModel::receiveSongListFromServer(const QList<Song>& songlist)
{
    m_songlist.clear();

    for (const auto& song : songlist) {
        m_songlist.push_back(song);
    }
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

Song* AudioPlayerModel::getRow(int index)
{
    Song *song = new Song(m_songlist.at(index), this);
    return song;
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
        QString newSongUrl = m_newSongsList.at(i).toLocalFile();
        QString newSongTitle = newSongUrl.split('/').last();
        Song song(newSongTitle, newSongUrl);
        out << newSongUrl << "\n";
        m_playlist.push_back(song);
        indices.push_back(indices.size());
    }
    endInsertRows();

    m_newSongsList.clear();
    mySongsFile->close();

}

void AudioPlayerModel::deleteSong(int songIndex)
{
    std::vector<Song>::iterator iteratorOfTheItemToRemoveFromPlaylist; // Ітератор елемента, що необхідно видалити з вектора m_playlist.
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

std::vector<Song> AudioPlayerModel::playlist()
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
//        QUrl path = QUrl::fromLocalFile(in.readLine());
//        m_playlist.append(path);
        QString url = in.readLine();
        QString title = url.split('/').last();
        Song song(title, url);
        m_playlist.push_back(song);
    }

    mySongsFile->close();

    return true;
}

FtpClient *AudioPlayerModel::client()
{
    return m_client;
}

QString AudioPlayerModel::downloadFolder() const
{
    return m_downloadFolder;
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
    for (int i = 0; i < static_cast<int>(m_playlist.size()); i++) {
        QString line = m_playlist.at(i).url() + "\n";
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

void AudioPlayerModel::m_clientInitialization()
{
    m_client = new FtpClient;

    m_client->moveToThread(&serverThread);

    setDownloadFolder(m_client->getDEFAULT_FOLDER());

    connect(this, &AudioPlayerModel::getMusicFile, m_client, &FtpClient::getMusicFile);
    connect(this, &AudioPlayerModel::getAllMusicFiles, m_client, &FtpClient::getAllMusicFilesInfo);
    connect(this, &AudioPlayerModel::connectToServer, m_client, &FtpClient::connectToServer);
    connect(m_client, &FtpClient::sendListOfMusicFromServerToModel, this, &AudioPlayerModel::setSonglistAsActiveSongList);
    connect(m_client, &FtpClient::connectionFailed, this, &AudioPlayerModel::onConnectionFailed);
    connect(m_client, &FtpClient::serverReadyToRequest, this, &AudioPlayerModel::serverReadyToRequest);
    connect(this, &AudioPlayerModel::downloadFolderChanged, m_client, &FtpClient::setDEFAULT_FOLDER);
}


