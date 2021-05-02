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
            color: (Material.theme === Material.Dark)
                   ? Material.color(Material.Pink, Material.Shade100)
                   : Material.color(Material.LightBlue, Material.Shade200)
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
        z: 99
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

                    _audioPlayerController.setIndexOfIndices(_songsList.currentIndex)
                    _audioPlayerController.changeCurrentSongIndex()
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
        z: 1
        id: _songsListBackground
        color: (Material.theme === Material.Light)
               ? Material.color(Material.Lime, Material.Shade200)
               : Material.color(Material.DeepPurple, Material.Shade200)
        opacity: 0.25
        radius: 5
        anchors.fill: root
        anchors.margins: 10
    }

    function deleteSong(index) {
        var currentSongPosition = _player.position

        _player.playlist.removeItem(index)
        _audioPlayerController.deleteSong(index)

        _player.seek(currentSongPosition)
    }

}
