#include "audioplayercontroller.h"

AudioPlayerController::AudioPlayerController(QObject *parent)
    : QObject(parent)
    , m_model (new AudioPlayer(this))
{
    connect(m_model, &AudioPlayer::stopThePlayer, this, &AudioPlayerController::stopThePlayer);
    connect(m_model, &AudioPlayer::rowsAboutToBeInserted, this, &AudioPlayerController::modelHasBeenChanged);
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

AudioPlayer *AudioPlayerController::model()
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

void AudioPlayerController::sendMessageToServer(QString msg)
{
    m_model->client()->sendMessage(msg);
}

void AudioPlayerController::stopThePlayer()
{
    emit stopPlayingMusic();
}

