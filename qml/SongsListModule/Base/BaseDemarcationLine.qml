import QtQuick 2.0
import QtQuick.Controls.Material 2.12

Rectangle {
    height: 1
    color: (Material.theme === Material.Dark)
           ? Material.color(Material.Pink, Material.Shade200)
           : Material.color(Material.Pink, Material.Shade200)
    opacity: 0.8
}
