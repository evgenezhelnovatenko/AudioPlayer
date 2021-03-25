import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

MenuBarItem {
    id: _menuBarItem

    contentItem: Text {
        text: _menuBarItem.text
        font: _menuBarItem.font
        opacity: enabled ? 1.0 : 0.3
        color: Material.color(Material.Pink)
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
    background: Rectangle {
        implicitWidth: 20
        height: root.height
        opacity: enabled ? 1 : 0.3
        color: _menuBarItem.highlighted ? Material.color(Material.Lime, Material.Shade300) : "transparent"
    }
}
