import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

MenuBar {
    id: root

    property int menuHeight: 30
    Menu {
        title: qsTr("Файл")

        Rectangle {
            height: 3
            color: "transparent"
        }
        Action {
            id: _addAction
            text: qsTr("Додати пісні")
            shortcut: "Ctrl+G"
            onTriggered: {
                _fileDialog.visible = true
            }
        }
        MenuSeparator {
            id: _separator
            objectName: "separator"
            contentItem: Rectangle {
                anchors.topMargin: 0
                anchors.bottomMargin: 0
                implicitWidth: 200
                implicitHeight: 1
                color: "#1E000000"
            }
        }

        Action {
            id: _exitAction
            text: qsTr("Вихід")
            onTriggered: {
                Qt.quit()
            }
        }
        Rectangle {
            height: 3
            color: "transparent"
        }

        delegate: MainMenuItemDelegate { }

    }
    Menu {
        title: qsTr("Довідка")

        Rectangle {
            height: 3
            color: "transparent"
        }
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
        Rectangle {
            height: 3
            color: "transparent"
        }

        delegate: MainMenuItemDelegate { }

    }


    delegate: MainMenuDelegate {}

    background: Rectangle {

        implicitHeight: root.height
        color: Material.color(Material.Grey)
        opacity: 0.15
    }

}
