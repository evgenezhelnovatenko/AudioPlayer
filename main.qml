import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12


ApplicationWindow {

    id: _mainWindow
    width: 700
    height: 480
    minimumWidth: 700
    visible: true
    title: qsTr("NeVr0t1k Player")

    Material.theme: Material.Light
    Material.accent: Material.DeepOrange

    property string playIcoSource: "/qml/resources/img/svg/music_player_play_icon.svg"
    property string pauseIcoSource: "/qml/resources/img/svg/music_player_pause_icon.svg"
    property string prevOrNextSoundIcoSource: "/qml/resources/img/svg/next_sound_arrow_icon_123825.svg"
    property string volumeIcoSource: "/qml/resources/img/svg/volume_control_sound_icon_.svg"

    Player {
        id: _mainInteface
        anchors.fill: parent

        gradient: Gradient {
            GradientStop { position: 0.0; color: Material.color(Material.DeepOrange, Material.Shade200) }
            GradientStop { position: 1.0; color: Material.color(Material.Yellow, Material.Shade400) }
        }
    }



}
