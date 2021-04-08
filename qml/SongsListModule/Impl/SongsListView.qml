import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

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

//            MouseArea {
//                visible: false // !_songsList.activeFocus
//                anchors.fill: parent
//                onClicked: {
//                    _songsList.forceActiveFocus()
//                }

//            }

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

    Popup {
        id: _popup
        height: 200
        width: 200
        padding: 5
        background: Rectangle {

            implicitWidth: 200
            implicitHeight: 200
            border.color: "#444"
            color: Material.color(Material.Cyan, Material.ShadeA100)
        }
        enter: Transition {
            NumberAnimation { property: "opacity"; from: 0.7; to: 1.0; duration: 50 }
        }
        exit: Transition {
            NumberAnimation { property: "opacity"; from: 1.0; to: 0.7; duration: 50 }
        }

        contentItem: Column {
            Button {
                id: _deleteSong
                anchors.left: parent.left
                anchors.right: parent.right

                height: 20
                text: qsTr("Видалити пісню")
                contentItem: Text {
                    text: _deleteSong.text
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    var currentIndex = _songsList.currentIndex
                    _audioPlayerController.deleteSong(currentIndex)
                    _popup.close()
                }
            }
        }

        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
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
