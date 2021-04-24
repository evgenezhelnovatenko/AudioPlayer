#include "audioplayercontroller.h"

AudioPlayerController::AudioPlayerController(QObject *parent) : QObject(parent)
{
    connect(&player, &AudioPlayer::stopThePlayer, this, &AudioPlayerController::stopThePlayer);
    connect(&player, &AudioPlayer::rowsAboutToBeInserted, this, &AudioPlayerController::modelHasBeenChanged);
}

void AudioPlayerController::changeIndexToNext()
{
    player.changeIndexToNext();
}

void AudioPlayerController::changeIndexToPrevious()
{
    player.changeIndexToPrevious();
}

void AudioPlayerController::changeCurrentSongIndex()
{
    player.changeCurrentSongIndex();
}

AudioPlayer *AudioPlayerController::getModel()
{
    return &player;
}

void AudioPlayerController::downloadJsonData()
{
    player.downloadJsonData();
}

void AudioPlayerController::deleteSong(int songIndex)
{
    player.deleteSong(songIndex);
}

void AudioPlayerController::shuffleSongs()
{
    player.shuffleSongsIndices();
//    emit playlistHasBeenChanged();
}

void AudioPlayerController::sortSongs()
{
    player.sortSongsIndices();
//    emit playlistHasBeenChanged();
}

void AudioPlayerController::setIndexOfIndices(int songIndex)
{
    int indexOfIndices = player.calculateIndexOfIndices(songIndex);
    player.setIndexOfIndices(indexOfIndices);
}

QList<QUrl> AudioPlayerController::getPlaylist()
{
    return player.playlist();
}

void AudioPlayerController::stopThePlayer()
{
    emit stopPlayingMusic();
}

