import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

Menu {

    Action {
        id: _addSongsAction
        text: qsTr("Додати пісні")
        shortcut: "Ctrl+G"
        onTriggered: {
            _fileDialog.visible = true
        }
    }
    Action {
        id: _deleteSongAction
        text: qsTr(isEditModeEnabled ? "Режим редагування: Вимк." : "Режим редагування: Увімк.")
        onTriggered: {
            isEditModeEnabled = !isEditModeEnabled
            _player.pause()
            isSongPlaying = false

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

    delegate: PopupMenuDelegate { }

}
