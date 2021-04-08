import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

Rectangle {
    id: root
    width: _songsList.width
    height: 40

    color: "transparent"

    function getFileName () {
        var arr = display.split('/');
        return arr[arr.length - 1];
    }

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
                source: volumeIcoSource
                fillMode: Image.PreserveAspectFit

                visible: audioPlayer.currentSongIndex === index ? true : false
            }
        }

        Text {
            text: '<b>' + root.getFileName() + '<\b>'
            font.family: "Helvetica"
            font.pointSize: 13
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    MouseArea {
        visible: true // _songsList.activeFocus
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onPressed: {
            if (mouse.button == Qt.RightButton) {
                var newPopupCoords = getNewPopupCoord(mouse.x, mouse.y)
                _popup.popup(_scrollView.parent, newPopupCoords.x, newPopupCoords.y)
            }

            _songsList.currentIndex = index
            _songsList.forceActiveFocus()
        }

        onDoubleClicked: {
            audioPlayer.currentSongIndex = index
            _audioPlayerController.songChange()
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

        if (positionInSongsList.x + _popup.width > _scrollView.parent.width) {
            newCoords.x = _scrollView.parent.width - _popup.width
        }
        if (positionInSongsList.y + _popup.height > _scrollView.parent.height) {
            newCoords.y = positionInSongsList.y - _popup.height
        }

        return newCoords;
    }

}

