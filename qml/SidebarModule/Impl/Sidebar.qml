import QtQuick 2.15
import QtQuick.Controls.Material 2.12
import SidebarModule.Base 1.0

Rectangle {

    property alias padding: _column.padding

    id: root
    color: "transparent"
    clip: true

    Column {
        id: _column
        anchors.fill: parent
        topPadding: 5
        bottomPadding: 5


        BaseText {
            id: _myMusic
            text: qsTr("Музика на пристрої")

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    if (!_myMusic.isSelected)
                        _audioPlayerController.getMyMusic();
                    _myMusic.isSelected = true
                    _musicForDownload.isSelected = false

                    _songsListView.changePopupMenu()
                }
            }
        }
        BaseText {
            id: _musicForDownload
            text: qsTr("Музика на сервері")

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    if (!_musicForDownload.isSelected)
                        _audioPlayerController.getAllMusicFilesInfoFromServer();
                    _musicForDownload.isSelected = true
                    _myMusic.isSelected = false

                    _songsListView.changePopupMenu()
                }
            }
        }
    }

    Rectangle {
        z: 1
        id: _songsListBackground
        anchors.fill: root
        color: (Material.theme === Material.Light)
               ? Material.color(Material.Lime, Material.Shade200)
               : Material.color(Material.DeepPurple, Material.Shade200)
        opacity: 0.25
        radius: 5
    }
}
