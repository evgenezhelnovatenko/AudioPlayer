import QtQuick 2.0
import QtMultimedia 5.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.15
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
            audioPlayer.filepath = ""
        }
        onModelHasBeenChanged: {
            audioPlayer = _audioPlayerController.getModel()
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

    Dialog {
        id: _changeThemeDialog

        title: qsTr("Змінити тему")
        standardButtons: Dialog.Apply | Dialog.Cancel
        x: (_mainWindow.width - _changeThemeDialog.width) / 2
        y: (_mainWindow.height - _changeThemeDialog.height) / 2

        contentItem: Rectangle {
            color: "transparent"
            ButtonGroup {
                id: _btnGroup
                buttons: _columnOfRadioBtn.children
            }

            Column {
                id: _columnOfRadioBtn
                anchors.left: parent.left

                RadioButton { text: qsTr("Dark") }
                RadioButton { text: qsTr("Light");  checked: true }
            }
        }

        onApplied: {
            var checkedBtn = _btnGroup.checkedButton
            if (checkedBtn.text === "Light")
                _mainWindow.Material.theme = Material.Light
            else if (checkedBtn.text === "Dark")
                _mainWindow.Material.theme = Material.Dark
        }
    }
}
