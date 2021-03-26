import QtQuick 2.15

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
        anchors.fill: parent
        onClicked: {
            _songsList.currentIndex = index
        }
        onDoubleClicked: {
            audioPlayer.currentSongIndex = index
            _audioPlayerController.songChange()
            isSongPlaying = true
            _player.play()
        }
    }

}