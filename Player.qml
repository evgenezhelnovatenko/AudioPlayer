import QtQuick 2.0
import QtMultimedia 5.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Material 2.12
import Player 1.0

Rectangle {

    property bool isSongPlaying: false
    property var audioPlayer: _audioPlayerController.getModel()

    id: root
    gradient: Gradient {
        GradientStop { position: 0.0; color: Material.color(Material.DeepOrange, Material.Shade200) }
        GradientStop { position: 1.0; color: Material.color(Material.Yellow, Material.Shade400) }
    }

    AudioPlayerController {
        id: _audioPlayerController;

        onStopPlayingMusic: {
            isSongPlaying = false
            _player.stop();
        }

    }

    Audio {
        id: _player
        source: audioPlayer.filepath
        volume: 0.7

        onPlaybackStateChanged: {
            if (playbackState === Audio.StoppedState) {
                if (_player.position === _player.duration) {
                    _audioPlayerController.switchToNextSong()
                    if (isSongPlaying)
                        _player.play()
                }
            }
        }
    }


    Column {

        PlayBar {
            id: _playbar
            width: root.width
            height: 50
        }

        MainMenu {
            id: _mainMenu
            width: root.width
            height: 25
        }

        SongsList {
            id: _songsList
            anchors.margins: 20
            width: root.width
            height: root.height - (_playbar.height + _mainMenu.height)

        }
    }
}
