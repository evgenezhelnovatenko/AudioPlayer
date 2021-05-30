import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

Item {

    property alias menu1: _menu1
    property alias menu2: _menu2

    Menu {

        id: _menu1
        property bool isMySongsSelected: true
        property string listName: "mySongList"

        Action {
            id: _addSongsAction
            text: qsTr("Додати пісні")
            shortcut: "Ctrl+G"
            onTriggered: {
                _fileDialog.visible = true
            }
        }
        Action {
            id: _editModeOnAction
            text: qsTr(isEditModeEnabled ? "Режим редагування: Вимк." : "Режим редагування: Увімк.")
            onTriggered: {
                isEditModeEnabled = !isEditModeEnabled
                _player.pause()
                isSongPlaying = false

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

    Menu {

        id: _menu2
        Action {
            text: qsTr("Інформація")
            onTriggered: {
                _loader.sourceComponent = _dialogComponent
                _loader.item.song = _audioPlayerController.getSong(_songsList.currentIndex)
                _loader.item.autor = _audioPlayerController.getAutorOfSong(_songsList.currentIndex)
                _loader.item.genres = _audioPlayerController.getGenresOfSong(_songsList.currentIndex)
                _loader.item.co_autors = _audioPlayerController.getCoAutorsOfSong(_songsList.currentIndex)
                _loader.item.open()
            }
        }

        delegate: PopupMenuDelegate { }

    }

}

