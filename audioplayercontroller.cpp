#include "audioplayercontroller.h"

AudioPlayerController::AudioPlayerController(QObject *parent) : QObject(parent)
{

    connect(&player, &AudioPlayer::songsAreOver, this, &AudioPlayerController::songsAreOver);
}

void AudioPlayerController::switchToNextSong()
{
    player.switchToNextSong();
}

void AudioPlayerController::switchToPrevSong()
{
    player.switchToPrevSong();
}

AudioPlayer *AudioPlayerController::getModel()
{
    return &player;
}

void AudioPlayerController::songsAreOver()
{
    emit stopPlayingMusic();
}