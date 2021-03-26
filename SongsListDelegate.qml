import QtQuick 2.15

Item {
    id: root
    width: _songsList.width
    height: 40

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
                id: _songPlayingImg
                width: 20
                height: 20
                source: volumeIcoSource
                fillMode: Image.PreserveAspectFit

                function q() {
                    console.log("audioPlayer.currentSongIndex: " + audioPlayer.currentSongIndex)
                    console.log("index: " + index)
                    console.log("---------")
                    if (audioPlayer.currentSongIndex === index)
                        return true
                    else
                        return false
                }

                visible: _songPlayingImg.q()
            }
        }

        Text {
            text: '<b>' + root.getFileName() + '<\b>'
            font.family: "Helvetica"
            font.pointSize: 13
            anchors.verticalCenter: parent.verticalCenter
        }
    }

}
