import QtQuick 2.0
import QtMultimedia 5.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Material 2.12
import Player 1.0
import PlayBarModule.Impl 1.0
import MainMenuModule.Impl 1.0
import SongsListModule.Impl 1.0

Rectangle {

    property bool isSongPlaying: false
    property var audioPlayer: _audioPlayerController.getModel()

    id: root

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

        MainMenuView {
            id: _mainMenu
            width: root.width
            height: 25
        }

        SongsListView {
            id: _songsListView
            anchors.margins: 20
            width: root.width
            height: root.height - (_playbar.height + _mainMenu.height)

        }
    }

    FileDialog {
        id: _fileDialog
        title: "Please choose a file"
        folder: shortcuts.music
        selectMultiple: true
        nameFilters: "Music files (*.mp3 *.mp4 *.wpa)"

        onAccepted: {
            audioPlayer.newSongsList =  _fileDialog.fileUrls

        }
        onRejected: {
            console.log("Canceled")
        }
    }
}
