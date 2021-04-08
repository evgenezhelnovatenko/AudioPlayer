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
                changePopouCoord(mouse.x, mouse.y)
                _popup.open()
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


    function changePopouCoord(mouseX, mouseY) {
        var positionInSongsList = mapToItem(_scrollView.parent, mouseX, mouseY)

        _popup.x = positionInSongsList.x
        _popup.y = positionInSongsList.y

        if (positionInSongsList.x + _popup.width > _scrollView.parent.width) {
            _popup.x = _scrollView.parent.width - _popup.width
        }
        if (positionInSongsList.y + _popup.height > _scrollView.parent.height) {
            _popup.y = positionInSongsList.y - _popup.height
        }
    }

}

