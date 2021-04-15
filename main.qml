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

    Material.accent: Material.Pink

    Player {
        id: _mainInteface
        anchors.fill: parent

        color: (Material.theme === Material.Dark)
               ? Material.color(Material.BlueGrey)
               : ""

        gradient: (Material.theme === Material.Light)
                  ? _playerGradient
                  : ""

        Gradient {
            id: _playerGradient
            GradientStop { position: 0.0; color: Material.color(Material.DeepOrange, Material.Shade200) }
            GradientStop { position: 1.0; color: Material.color(Material.Yellow, Material.Shade400) }
        }


    }

}
