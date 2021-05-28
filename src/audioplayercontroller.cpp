#include "audioplayercontroller.h"

AudioPlayerController::AudioPlayerController(QObject *parent)
    : QObject(parent)
    , m_model (new AudioPlayerModel(this))
{
    connect(m_model, &AudioPlayerModel::stopThePlayer, this, &AudioPlayerController::stopThePlayer);
    connect(m_model, &AudioPlayerModel::rowsAboutToBeInserted, this, &AudioPlayerController::modelHasBeenChanged);
    connect(this, &AudioPlayerController::getMusicFile, m_model, &AudioPlayerModel::getMusicFile);
    connect(this, &AudioPlayerController::getAllMusicFiles, m_model, &AudioPlayerModel::getAllMusicFiles);
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
    return m_model->playlist();
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

