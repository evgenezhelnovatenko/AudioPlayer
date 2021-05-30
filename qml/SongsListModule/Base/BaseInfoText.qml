import QtQuick 2.0
import QtQuick.Controls.Material 2.12

Text {
    color: (Material.theme === Material.Dark)
           ? "#fff"
           : "#000"
    font.family: "Helvetica"
    font.pointSize: 11
    horizontalAlignment: Text.AlignLeft
    elide: Text.ElideRight
    wrapMode: Text.Wrap
}
