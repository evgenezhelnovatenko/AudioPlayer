import QtQuick 2.14
import QtQuick.Window 2.14
import QtMultimedia 5.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Material 2.12


ApplicationWindow {

    id: _mainWindow
    width: 700
    height: 480
    minimumHeight: 480
    visible: true
    title: qsTr("NeVr0t1k Player")

    Material.theme: Material.Light
    Material.accent: Material.DeepOrange

    property string playIcoSource: "/img/svg/music_player_play_icon.svg"
    property string pauseIcoSource: "/img/svg/music_player_pause_icon.svg"
    property string prevOrNextSoundIcoSource: "/img/svg/next_sound_arrow_icon_123825.svg"
    property string volumeIcoSource: "/img/svg/volume_control_sound_icon_.svg"

    Player {
        id: _mainInteface
        anchors.fill: parent
    }

    FileDialog {
        id: _fileDialog
        title: "Please choose a file"
        folder: shortcuts.music
        selectMultiple: true
        nameFilters: "Music files (*.mp3 *.mp4 *.wpa)"

        onAccepted: {
            console.log("You chose: " + _fileDialog.fileUrls)

        }
        onRejected: {
            console.log("Canceled")
        }
    }

}
