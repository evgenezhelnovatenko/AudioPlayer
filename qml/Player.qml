import QtQuick 2.15
import QtMultimedia 5.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import AudioPlayer 1.0
import PlayBarModule.Impl 1.0
import MainMenuModule.Impl 1.0
import SongsListModule.Impl 1.0
import SidebarModule.Impl 1.0

Rectangle {

    property bool isSongPlaying: false
    property var audioPlayer: _audioPlayerController.model()
    property bool isLoop: false
    property bool isShuffle: false
    property bool isEditModeEnabled: false
    property bool isServerMusicListSelected: false

    id: root

    AudioPlayerController {
        id: _audioPlayerController;
        objectName: "controller"

        onStopPlayingMusic: {
            isSongPlaying = false
            _player.stop()
        }
        onModelHasBeenChanged: {
            audioPlayer = _audioPlayerController.model()
        }

        function showPlaylist() {
            for (var i = 0; i < _player.playlist.itemCount; i++) {
                console.log(_player.playlist.itemSource(i))
            }
            console.log("----------------------------------------------")
        }
        onConnectionFailed: {
            _connFailedMsgDialog.open()
            _sidebar.changeCurrentSelectedItem()
            _sidebar.unblockingUnnecessaryFunctions()
        }
        onServerReadyToRequest: {
            _songsListView.changePopupMenu()
            _audioPlayerController.getAllMusicFilesInfoFromServer();
        }
    }

    Audio {
        id: _player
        audioRole: Audio.MusicRole
        playlist: _playlist
        volume: 0.7
        autoLoad: true
        onPlaybackStateChanged: { // Цей сигнал оброблюється для того щоб, коли пісня дійде кінця, вона переключилася
                                  // на наступну.
            if (playbackState === Audio.StoppedState) {
                if (_player.duration !== 0) { // Ця умова для того щоб не при _player.duration = 0 програма не заходила у цей if.
                                              // Бо _player.position також буде дорівнювати 0.
                    if (_player.position === _player.duration) {
                        _audioPlayerController.changeIndexToNext()
                    }
                }

            }
        }

        Component.onCompleted: {
            _player.playlist.addItems(_audioPlayerController.getPlaylist())
//            _audioPlayerController.showPlaylist()
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

        Rectangle {
            id: _mainContent
            width: root.width
            height: root.height - (_playbar.height + _mainMenu.height)
            color: "transparent"

            RowLayout {
                anchors.fill: parent
                anchors.margins: 5
                spacing: 5

                Sidebar {
                    id: _sidebar
                    Layout.minimumWidth: 200
                    Layout.fillHeight: true

                }

                SongsListView {
                    id: _songsListView
                    Layout.fillHeight: true
                    Layout.fillWidth: true
//                    Layout.minimumWidth: 500
                }
            }
        }
    }

    Playlist {
        id: _playlist

        onCurrentIndexChanged: {
            console.log("currentIndex: " + currentIndex)
            if (isSongPlaying)
                _player.play()
        }
        onItemRemoved: {
//            _songsListView.deleteSong()
            console.log("onItemRemoved: " + _player.playlist.currentIndex + "; " + audioPlayer.currentSongIndex);
        }

    }

    Binding {
        target: _playlist
        property: "currentIndex"
        value: audioPlayer.currentSongIndex

    }

    FileDialog {
        id: _fileDialog
        title: qsTr("Будь-ласка виберіть файл(и)")
        folder: shortcuts.music
        selectMultiple: true
        nameFilters: "Music files (*.mp3 *.mp4 *.wpa)"

        onAccepted: {
            var newSongs = _fileDialog.fileUrls
            audioPlayer.newSongsList = newSongs
            _playlist.addItems(newSongs)
            if (isShuffle)
                _audioPlayerController.shuffleSongs()
        }
        onRejected: {
            console.log("Canceled")
        }
    }

    FileDialog {
        id: _folderOpenDialog
        title: qsTr("Будь-ласка виберіть папку")
        selectFolder: true
        folder: shortcuts.music

        onAccepted: {
            _textField.text = urlToPath(_folderOpenDialog.fileUrl)
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

                RadioButton { text: qsTr("Темна") }
                RadioButton { text: qsTr("Світла");  checked: true }
            }
        }

        onApplied: {
            var checkedBtn = _btnGroup.checkedButton
            if (checkedBtn.text === qsTr("Світла"))
                _mainWindow.Material.theme = Material.Light
            else if (checkedBtn.text === qsTr("Темна"))
                _mainWindow.Material.theme = Material.Dark
        }
    }
    Dialog {
        id: _changeDefaultMusicFolderDialog
        width: 370

        title: qsTr("Змінити шлях для скачування")

        standardButtons: StandardButton.Save | StandardButton.Cancel
        x: (_mainWindow.width - _changeDefaultMusicFolderDialog.width) / 2
        y: (_mainWindow.height - _changeDefaultMusicFolderDialog.height) / 2

        contentItem: Rectangle {

            color: "transparent"
            Column {
                width: parent.width
                Text {
                    width: parent.width
                    text: qsTr("Папка, у яку будуть скачуватися треки з серверу:")
                    font.family: "Helvetica"
                    font.pointSize: 10
                    color: (Material.theme === Material.Dark)
                           ? "#fff"
                           : "#000"
                }

                Row {
                    spacing: 5
                    width: parent.width
                    TextField {
                        id: _textField
                        width: parent.width - _overviewBtn.width - parent.spacing
                        anchors.bottom: parent.bottom
                        text: audioPlayer.downloadFolder
                        placeholderText: qsTr("Введіть шлях до папки")
                        selectByMouse: true
                        focus: true
                    }
                    Button {
                        id: _overviewBtn
                        text: qsTr("Огляд...")
                        font.family: "Helvetica"
                        font.pointSize: 10
                        anchors.bottom: parent.bottom

                        onClicked: {
                            _folderOpenDialog.open()
                        }

                    }

                }

            }
        }

        onAccepted: {
            audioPlayer.downloadFolder = _textField.text

        }
    }
    MessageDialog {
        id: _connFailedMsgDialog
        title: "Server status"
        icon: StandardIcon.Question
        text: "Connect to server failed!"
        detailedText: "Try to connect a little later "
        standardButtons: StandardButton.Ok
    }

    function urlToPath(urlString) {
        var s
        if (urlString.startsWith("file:///")) {
            var k = urlString.charAt(9) === ':' ? 8 : 7
            s = urlString.substring(k)
        } else {
            s = urlString
        }
        return s;
    }
}
