import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

Rectangle {

    clip: true
    Component {
        id: highlight
        Rectangle {
            width: 180; height: 40
            color: "lightsteelblue"; radius: 5
            y: _songsList.currentItem.y
            Behavior on y {
                SpringAnimation {
                    spring: 3
                    damping: 0.2
                }
            }
        }
    }
    ListView {
        id: _songsList
        anchors.fill: parent
        delegate: SongsListDelegate { }
        boundsBehavior: Flickable.StopAtBounds

        highlight: highlight

        focus: true
        activeFocusOnTab: true

        MouseArea {
            visible: !_songsList.focus
            anchors.fill: parent
            onClicked: {
                _songsList.focus = true
            }
        }

        Keys.onPressed: {
            if ((event.key === Qt.Key_Enter) || (event.key === Qt.Key_Return)) {
                audioPlayer.currentSongIndex = _songsList.currentIndex
                _audioPlayerController.songChange()
                isSongPlaying = true;
                _player.play()
            }
        }

        Component.onCompleted: {
            _songsList.model = _audioPlayerController.getModel()
        }

    }

}
