import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12


ApplicationWindow {
    id: _mainWindow

    property bool isDarkTheme: false

    width: 700
    height: 480
    minimumWidth: 700
    visible: true
    title: qsTr("NeVr0t1k Player")

    Material.theme: setMaterialTheme()

    Material.accent: Material.DeepOrange

    property string playIcoSource: "/qml/resources/img/svg/music_player_play_icon.svg"
    property string pauseIcoSource: "/qml/resources/img/svg/music_player_pause_icon.svg"
    property string prevOrNextSoundIcoSource: "/qml/resources/img/svg/next_sound_arrow_icon_123825.svg"
    property string volumeIcoSource: "/qml/resources/img/svg/volume_control_sound_icon_.svg"
    property string noneSongImageImgSource: "/qml/resources/img/songImageNotFoundImg.jpg"

    Player {
        id: _mainInteface
        anchors.fill: parent

        Gradient {
            id: _gradient
            GradientStop { position: 0.0; color: Material.color(Material.DeepOrange, Material.Shade200) }
            GradientStop { position: 1.0; color: Material.color(Material.Yellow, Material.Shade400) }
        }
    }

    function setMaterialTheme() {
        if (isDarkTheme) {
            _mainInteface.color = Material.color(Material.BlueGrey)
            return Material.Dark
        } else {
            setPlayerGradient()
            return Material.Light
        }
    }

    function setPlayerGradient() {
        _mainInteface.gradient = _gradient
    }

}
