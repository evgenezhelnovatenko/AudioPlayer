import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

Menu {


    Rectangle {
        height: 3
        color: "transparent"
    }
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
//            if (currentIndex === audioPlayer.currentSongIndex) {
//                _player.stop()
//                audioPlayer.currentSongIndex = -1
//            }
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

    Rectangle {
        height: 3
        color: "transparent"
    }

    delegate: PopupMenuDelegate { }

}
