import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import PopupMenuModule.Impl 1.0

Rectangle {
    id: root
    color: "transparent"
    clip: true

    Component {
        id: _highlight
        Rectangle {
            id: _qwer
            width: _songsList.width; height: 40
            color: "lightsteelblue"
            radius: 5
            y: _songsList.currentItem ? _songsList.currentItem.y : _qwer.y
            Behavior on y {
                SmoothedAnimation {
                    easing.type: Easing.Linear
                    duration:200;
                    maximumEasingTime:300
                    velocity : 1000
                }
            }
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

            highlight: _highlight
            highlightFollowsCurrentItem: false

            activeFocusOnTab: true

            removeDisplaced: Transition {
                NumberAnimation { properties: "x,y"; duration: 150 }
            }

            Keys.onPressed: {
                if ((event.key === Qt.Key_Enter) || (event.key === Qt.Key_Return)) {

                    _songsList.model.currentSongIndex = _songsList.currentIndex
                    _audioPlayerController.songChange()
                    isSongPlaying = true;
                    _player.play()
                }
            }

            Component.onCompleted: {
                _songsList.model = audioPlayer
            }

        }

    }

    PopupMenuView {
        id: _popup

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
