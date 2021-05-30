import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

MenuBar {
    property int menuHeight: 30

    id: root

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
        Action {
            id: _getRequest
            text: qsTr("На сервер ->")
            onTriggered: {
                var message = "mr-nyoma-gr-dombaj-priora-remix-mp3.mp3"
                _audioPlayerController.getMusicFile(message)
            }
        }
        Action {
            id: _changeTheme
            text: qsTr("Змінити тему")
            onTriggered: {
                _changeThemeDialog.open()
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

        delegate: MainMenuItemDelegate { }

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

        delegate: MainMenuItemDelegate { }

    }


    delegate: MainMenuDelegate {}

    background: Rectangle {
        height: root.height
        color: Material.color(Material.Grey)
        opacity: 0.15
    }

}
