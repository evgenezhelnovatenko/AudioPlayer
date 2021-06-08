#include "audioplayercontroller.h"

AudioPlayerController::AudioPlayerController(QObject *parent)
    : QObject(parent)
    , m_model (new AudioPlayerModel(this))
{
    connect(m_model, &AudioPlayerModel::stopThePlayer, this, &AudioPlayerController::stopThePlayer);
    connect(m_model, &AudioPlayerModel::rowsAboutToBeInserted, this, &AudioPlayerController::modelHasBeenChanged);
    connect(this, &AudioPlayerController::getMusicFile, m_model, &AudioPlayerModel::getMusicFile);
    connect(this, &AudioPlayerController::getAllMusicFiles, m_model, &AudioPlayerModel::getAllMusicFiles);
    connect(this, &AudioPlayerController::createServerConn, m_model, &AudioPlayerModel::connectToServer);
    connect(m_model, &AudioPlayerModel::connectionFailed, this, &AudioPlayerController::connectionFailed);
    connect(m_model, &AudioPlayerModel::serverReadyToRequest, this, &AudioPlayerController::serverReadyToRequest);
}

AudioPlayerController::~AudioPlayerController()
{
    delete m_model;
}

void AudioPlayerController::changeIndexToNext()
{
    m_model->changeIndexToNext();
}

void AudioPlayerController::changeIndexToPrevious()
{
    m_model->changeIndexToPrevious();
}

void AudioPlayerController::changeCurrentSongIndex()
{
    m_model->changeCurrentSongIndex();
}

AudioPlayerModel *AudioPlayerController::model()
{
    return m_model;
}

void AudioPlayerController::deleteSong(int songIndex)
{
    m_model->deleteSong(songIndex);
}

void AudioPlayerController::shuffleSongs()
{
    m_model->shuffleSongsIndices();
}

void AudioPlayerController::sortSongs()
{
    m_model->sortSongsIndices();
}

void AudioPlayerController::setIndexOfIndices(int songIndex)
{
    int indexOfIndices = m_model->calculateIndexOfIndices(songIndex);
    m_model->setIndexOfIndices(indexOfIndices);
}

QList<QUrl> AudioPlayerController::getPlaylist()
{
    QList<QUrl> playlist;

    for (auto& song : m_model->playlist()) {
        playlist.append(QUrl::fromLocalFile(song.url()));
    }

    return playlist;
}

void AudioPlayerController::getMyMusic()
{
    m_model->setPlaylistAsActiveSongList();
}

Song *AudioPlayerController::getSong(int index)
{
    return m_model->getRow(index);
}

Autor *AudioPlayerController::getAutorOfSong(int index)
{
    Autor* autor = new Autor(getSong(index)->autor(), this);

    return autor;
}

QList<Genre*> AudioPlayerController::getGenresOfSong(int index)
{
    QList<Genre*> genres;
    for (auto& genre : getSong(index)->genres()) {
        Genre *g = new Genre(genre, this);
        genres.push_back(g);
    }

    return genres;
}

QList<CoAutor*> AudioPlayerController::getCoAutorsOfSong(int index)
{
    QList<CoAutor*> co_autors;
    for (auto& co_autor : getSong(index)->co_autors()) {
        CoAutor *c_a = new CoAutor(co_autor, this);
        co_autors.push_back(c_a);
    }

    return co_autors;
}

void AudioPlayerController::connectToServer()
{
    emit createServerConn();
}

void AudioPlayerController::getAllMusicFilesInfoFromServer()
{
    emit getAllMusicFiles();
}

void AudioPlayerController::getMusicFileFromServer(QString msg)
{
    emit getMusicFile(msg);
}

void AudioPlayerController::stopThePlayer()
{
    emit stopPlayingMusic();
}

