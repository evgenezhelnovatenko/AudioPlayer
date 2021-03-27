import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

MenuItem {
    id: _menuItem
    contentItem: Text {
        leftPadding: 15
        rightPadding: 15
        text: _menuItem.text
        font.family: "Helvetica"
        font.pointSize: 9
        opacity: enabled ? 1.0 : 0.15
        color: "#000"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitHeight: menuHeight
        color: "transparent"
        Rectangle {
            anchors.fill: parent
            anchors.leftMargin: 3
            anchors.rightMargin: 3
            opacity: enabled ? 1 : 0.3
            color: _menuItem.highlighted ? Material.color(Material.LightBlue, Material.Shade300) : "transparent"
        }
    }
}
