import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

Menu {

    Action {
        id: _addAction
        text: qsTr("Додати пісні")
        shortcut: "Ctrl+G"
        onTriggered: {
            _fileDialog.visible = true
        }
    }
    Action {
        id: _getRequest
        text: qsTr("Видалити пісню")
        onTriggered: {
            var currentIndex = _songsList.currentIndex
//            _audioPlayerController.showPlaylist()
//            console.log(_playlist.currentIndex + "; " + audioPlayer.currentSongIndex)
//            _player.playlist.moveItem(currentIndex, _player.playlist.currentIndex)
//            _audioPlayerController.showPlaylist()
//            _playlist.removeItem(_player.playlist.currentIndex + 1)
//            console.log(_playlist.currentIndex + "; " + audioPlayer.currentSongIndex)
//            _audioPlayerController.deleteSong(currentIndex)
//            console.log(_playlist.currentIndex + "; " + audioPlayer.currentSongIndex)
//            _audioPlayerController.showPlaylist()


            console.log(_player.playlist.currentIndex + "; " + audioPlayer.currentSongIndex)
            _player.playlist.removeItem(currentIndex)
            console.log(_player.playlist.currentIndex + "; " + audioPlayer.currentSongIndex)
            _audioPlayerController.deleteSong(currentIndex)
        }
    }
    MenuSeparator {
        id: _separator
        objectName: "separator"
        contentItem: Rectangle {
            anchors.topMargin: 0
            anchors.bottomMargin: 0
            implicitWidth: 200
            implicitHeight: 1
            color: "#1E000000"
        }
    }

    Action {
        id: _exitAction
        text: qsTr("Вихід")
        onTriggered: {
            Qt.quit()
        }
    }

    delegate: PopupMenuDelegate { }

}
