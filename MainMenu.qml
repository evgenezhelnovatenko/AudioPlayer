import QtQuick 2.0
import QtQuick.Controls 2.15

MenuBar {
    Menu {
        title: qsTr("Файл")
        Action {
            id: _addAction
            text: qsTr("Додати пісні")
            shortcut: "Ctrl+G"
            onTriggered: {
                _fileDialog.visible = true
            }
        }
    }
    Menu {
        title: qsTr("Довідка")
        Action {
            id: _helpAction
            text: qsTr("Інструкція")
            onTriggered: {
                console.log("Інструкція pressed!")
            }
        }
        Action {
            id: _aboutAction
            text: qsTr("Про нас")
            onTriggered: {
                console.log("Про нас pressed!")
            }
        }
    }


    delegate: MainMenuDelegate {}

    background: Rectangle {
        implicitWidth: 40
        implicitHeight: 40
        color: "transparent"

        Rectangle {
            color: "#21be2b"
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
        }
    }
}
