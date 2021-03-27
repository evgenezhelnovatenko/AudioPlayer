import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

Rectangle {
    id: root
    color: "transparent"
    clip: true

    Component {
        id: highlight
        Rectangle {
            width: 180; height: 40
            color: "lightsteelblue"
            radius: 5
        }
    }

    ScrollView {
        id: _scrollView
        anchors.fill: _songsListBackground
        clip: true

        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff


        ListView {
            id: _songsList
            anchors.fill: parent
            delegate: SongsListDelegate { }
            boundsBehavior: Flickable.StopAtBounds

            highlight: highlight

            activeFocusOnTab: true

            MouseArea {
                visible: !_songsList.activeFocus
                anchors.fill: parent
                onClicked: {
                    _songsList.forceActiveFocus()
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

    Rectangle {
        id: _songsListBackground
        color: Material.color(Material.Lime)
        opacity: 0.25
        radius: 5
        anchors.fill: root
        anchors.margins: 10
    }

}
