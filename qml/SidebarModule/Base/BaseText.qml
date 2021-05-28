import QtQuick 2.0
import QtQuick.Controls.Material 2.12

Rectangle {

    property alias text: _text.text

    id: _root
    width: parent.width
    height: 30
    color: "transparent"

    Text {
        id: _text
        x: 5
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        font.family: "Helvetica"
        font.pointSize: 14
        color: "#000"
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                _background.visible = true
            }
            onExited: {
                _background.visible = false
            }

        }
    }

    Rectangle {
        id: _background
        visible: false
        anchors.fill: parent
        color: Material.color(Material.Grey, Material.ShadeA200)
        opacity: 0.25
    }
}
