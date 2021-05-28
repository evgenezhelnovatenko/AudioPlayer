import QtQuick 2.15
import QtQuick.Controls.Material 2.12
import SidebarModule.Base 1.0

Rectangle {

    property alias padding: _column.padding

    id: root
    color: "transparent"
    clip: true

    Column {
        id: _column
        anchors.fill: parent
        topPadding: 5
        bottomPadding: 5


        BaseText {
            text: qsTr("Музика на пристрої")
        }
        BaseText {
            text: qsTr("Музика на сервері")
        }
    }

    Rectangle {
        z: 1
        id: _songsListBackground
        anchors.fill: root
        color: (Material.theme === Material.Light)
               ? Material.color(Material.Lime, Material.Shade200)
               : Material.color(Material.DeepPurple, Material.Shade200)
        opacity: 0.25
        radius: 5
    }
}
