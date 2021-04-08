#include "audioplayercontroller.h"

AudioPlayerController::AudioPlayerController(QObject *parent) : QObject(parent)
{

    connect(&player, &AudioPlayer::songsAreOver, this, &AudioPlayerController::songsAreOver);
    connect(&player, &AudioPlayer::rowsAboutToBeInserted, this, &AudioPlayerController::modelHasBeenChanged);
}

void AudioPlayerController::switchToNextSong()
{
    player.switchToNextSong();
}

void AudioPlayerController::switchToPrevSong()
{
    player.switchToPrevSong();
}

void AudioPlayerController::songChange()
{
    player.songChange();
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

void AudioPlayerController::songsAreOver()
{
    emit stopPlayingMusic();
}

