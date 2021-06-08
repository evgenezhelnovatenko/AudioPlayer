import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

Item {

    property alias menu1: _menu1
    property alias menu2: _menu2

    id: root

    Menu {

        id: _menu1
        property bool isMySongsSelected: true
        property string listName: "mySongList"

        Action {
            id: _addSongsAction
            text: qsTr("Додати треки")
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
                root.setSongInfo()
                if (_loader.status === Loader.Null)
                    _loader.sourceComponent = _dialogComponent
                _loader.item.open()
            }
        }
        Action {
            text: qsTr("Завантажити трек")
            onTriggered: {
                var song = _audioPlayerController.getSong(_songsList.currentIndex)
                var url = song.url + ".mp3"
                console.log(url)
                _audioPlayerController.getMusicFileFromServer(url)
            }
        }

        delegate: PopupMenuDelegate { }



    }

    function setSongInfo() {
        _songsList.song = _audioPlayerController.getSong(_songsList.currentIndex)
        _songsList.autor = _audioPlayerController.getAutorOfSong(_songsList.currentIndex)
        _songsList.genres = _audioPlayerController.getGenresOfSong(_songsList.currentIndex)
        _songsList.co_autors = _audioPlayerController.getCoAutorsOfSong(_songsList.currentIndex)
    }

}

