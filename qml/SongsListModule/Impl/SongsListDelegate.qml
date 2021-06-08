import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import ResourceProvider 1.0

Rectangle {

    property int year: year

    id: root
    width: _songsList.width
    height: 40

    color: "transparent"

    Row {
        leftPadding: 5
        rightPadding: 5
        spacing: 10
        anchors.fill: parent
        Rectangle {
            width: 20
            height: 20
            anchors.verticalCenter: parent.verticalCenter
            color: "transparent"
            Image {
                id: _songPlaying
                width: 20
                height: 20
                source: Resources.songsList.volumeSoundIco
                fillMode: Image.PreserveAspectFit

                visible: (_player.playlist.currentIndex === index && !isEditModeEnabled && !isServerMusicListSelected) ? true : false
            }
        }

        Text {
            text: '<b>' + title + '<\b>'
            font.family: "Helvetica"
            font.pointSize: 13
            anchors.verticalCenter: parent.verticalCenter
        }
        Rectangle {
            width: 20
            height: 20
            anchors.verticalCenter: parent.verticalCenter
            color: "transparent"
            radius: width * 0.5
            Image {
                id: _deleteSongImg
                anchors.fill: parent
                source: Resources.songsList.deleteSongImg
                fillMode: Image.PreserveAspectFit

                visible: isEditModeEnabled
                z: 4

                MouseArea {
                    id: _deleteSongArea
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        console.log(index)
                        _songsListView.deleteSong(index)
                    }
                }
            }

            Rectangle {
                id: _deleteSongHoverBackground
                anchors.fill: parent
                opacity: 0.4
                color: _deleteSongArea.containsMouse ? Material.color(Material.Brown) : "transparent"
                radius: width * 0.5
                z: 5
            }
        }
    }

    MouseArea {
        id: _mouseArea
        visible: !_deleteSongArea.containsMouse
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onPressed: {
            if (mouse.button == Qt.RightButton) {
                var newPopupCoords = getNewPopupCoord(mouse.x, mouse.y)
                popupMenu.popup(_scrollView.parent, newPopupCoords.x, newPopupCoords.y)
            }

            if (isEditModeEnabled)
                return

            _songsList.currentIndex = index
            _songsList.forceActiveFocus()
        }

        onDoubleClicked: {

            if (isEditModeEnabled || isServerMusicListSelected)
                return

            _audioPlayerController.setIndexOfIndices(index)
            _audioPlayerController.changeCurrentSongIndex()

            if (isShuffle)
                _audioPlayerController.shuffleSongs()

            isSongPlaying = true
            _player.play()
        }

    }


    function getNewPopupCoord(mouseX, mouseY) {

        var positionInSongsList = mapToItem(_scrollView.parent, mouseX, mouseY)

        var newCoords = {
            x: positionInSongsList.x,
            y: positionInSongsList.y
        }

        if (positionInSongsList.x + popupMenu.width > _scrollView.parent.width) {
            newCoords.x = _scrollView.parent.width - popupMenu.width
        }
        if (positionInSongsList.y + popupMenu.height > _scrollView.parent.height) {
            newCoords.y = positionInSongsList.y - popupMenu.height
        }

        return newCoords
    }

}

