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
            isSelected: true

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    if (!_myMusic.isSelected) {
                        _audioPlayerController.getMyMusic();
                        changeCurrentSelectedItem()
                        unblockingUnnecessaryFunctions()
                    }

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
                    if (!_musicForDownload.isSelected) {
                        _audioPlayerController.connectToServer()
                        changeCurrentSelectedItem()
                        blockingUnnecessaryFunctions()
                    }
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

    function changeCurrentSelectedItem() {
        _musicForDownload.isSelected= !_musicForDownload.isSelected
        _myMusic.isSelected = !_myMusic.isSelected
    }

    function blockingUnnecessaryFunctions() {
        _mainInteface.isServerMusicListSelected = true
        _player.pause()
        isSongPlaying = false
        if (_mainInteface.isEditModeEnabled)
            _mainInteface.isEditModeEnabled = false
    }
    function unblockingUnnecessaryFunctions() {
        _mainInteface.isServerMusicListSelected = false
    }

}
