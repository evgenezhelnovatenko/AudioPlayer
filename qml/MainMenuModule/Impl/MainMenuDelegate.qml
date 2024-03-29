import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

MenuBarItem {
    id: _menuBarItem

    contentItem: Text {
        anchors.fill: parent
        text: _menuBarItem.text
        font.family: "Helvetica"
        font.pointSize: 9
        font.weight: Font.DemiBold
        lineHeight: 0.1
        opacity: enabled ? 1.0 : 0.3
        color: (Material.theme === Material.Dark)
               ? Material.color(Material.Grey, Material.Shade200)
               : Material.color(Material.Pink)
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
    background: Rectangle {
        implicitWidth: 20
        height: root.height
        opacity: enabled ? 1 : 0.3
        color: _menuBarItem.highlighted
               ? (Material.theme === Material.Dark)
                 ? Material.color(Material.Indigo)
                 : Material.color(Material.Lime, Material.Shade300)
               : "transparent"
    }
}
