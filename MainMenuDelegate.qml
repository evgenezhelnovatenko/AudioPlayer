import QtQuick 2.0
import QtQuick.Controls 2.12

MenuBarItem {
    id: _menuBarItem

    contentItem: Text {
        text: _menuBarItem.text
        font: _menuBarItem.font
        opacity: enabled ? 1.0 : 0.3
        color: _menuBarItem.highlighted ? "#ffffff" : "red"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
    background: Rectangle {
        implicitWidth: 40
        implicitHeight: 40
        opacity: enabled ? 1 : 0.3
        color: _menuBarItem.highlighted ? "#21be2b" : "transparent"
    }
}
