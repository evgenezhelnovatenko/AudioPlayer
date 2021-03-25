import QtQuick 2.14
import QtQuick.Window 2.14
import QtMultimedia 5.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Material 2.12


ApplicationWindow {

    id: _mainWindow
    width: 700
    height: 480
    visible: true
    title: qsTr("Hello World")

    Material.theme: Material.Light
    Material.accent: Material.DeepOrange

    flags: Qt.FramelessWindowHint

    property int previousX
    property int previousY

    Player {
        id: _mainInteface
        anchors.fill: parent

    }

    FileDialog {
        id: _fileDialog
        title: "Please choose a file"
        folder: shortcuts.music
        selectMultiple: true
        nameFilters: "Music files (*.mp3 *.mp4 *.wpa)"

        onAccepted: {
            console.log("You chose: " + _fileDialog.fileUrls)

        }
        onRejected: {
            console.log("Canceled")
        }
    }

//
//            ListView {
//                id: _listMusic
//                width: 300
//                height: 500

//                delegate: Text {
//                    id: _txt

//                    function getFileName () {
//                        var arr = display.split('/');

//                        return arr[arr.length - 1];
//                    }
//                    text: _txt.getFileName()
//                }

//                Component.onCompleted: {
//                    _listMusic.model = _audioPlayerController.getModel()
//                }
//            }

//        }
//
//        Button {
//            text: "Додати"
//            width: 200
//            height: 20

//            onClicked: {
//                _fileDialog.visible = true
//            }

//        }
//    }
    MouseArea {
        id: _topArea
        height: 5
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        cursorShape: Qt.SizeVerCursor

        onPressed: {
            previousY = mouseY
        }
        onMouseYChanged: {
            var dy = mouseY - previousY
            _mainWindow.setY(_mainWindow.y + dy)
            _mainWindow.setHeight(_mainWindow.height - dy)
        }
    }
    MouseArea {
        id: _bottomArea
        height: 5
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        cursorShape: Qt.SizeVerCursor

        onPressed: {
            previousY = mouseY
        }
        onMouseYChanged: {
            var dy = mouseY - previousY
            _mainWindow.setHeight(_mainWindow.height + dy)
        }
    }
    MouseArea {
        id: _leftArea
        width: 5
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        cursorShape: Qt.SizeHorCursor

        onPressed: {
            previousX = mouseX
        }
        onMouseXChanged: {
            var dx = mouseX - previousX
            _mainWindow.setX(_mainWindow.x + dx)
            _mainWindow.setWidth(_mainWindow.width - dx)
        }
    }
    MouseArea {
        id: _rightArea
        width: 5
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        cursorShape: Qt.SizeHorCursor

        onPressed: {
            previousX = mouseX
        }
        onMouseXChanged: {
            var dx = mouseX - previousX
            _mainWindow.setWidth(_mainWindow.width + dx)
        }
    }
    MouseArea {
        id: _moveArea


        onPressed: {
            previousX = mouseX
            previousY = mouseY
        }
        onMouseXChanged: {
            var dx = mouseX - previousX
            _mainWindow.setX(_mainWindow.x + dx)
        }
        onMouseYChanged: {
            var dy = mouseY - previousY
            _mainWindow.setY(_mainWindow.y + dy)
        }
    }
}
