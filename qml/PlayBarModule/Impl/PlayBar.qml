import QtQuick 2.0
import QtQuick.Layouts 1.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import PlayBarModule.Base 1.0

Rectangle {
    property int currMinute: _player.position / 60000
    property int currSecond: _player.position / 1000 % 60
    property int minutesLeftUntilToTheEnd: (_player.duration - _player.position) / 60000
    property int secondsLeftUntilToTheEnd: (_player.duration - _player.position) / 1000 % 60

    function convertSecondsToText(seconds) {
        var secondsAbs = Math.abs(seconds)
        if (secondsAbs < 10)
            return "0" + secondsAbs
        else
            return secondsAbs
    }

    id: root

    color: (Material.theme === Material.Dark)
           ? Material.color(Material.BlueGrey, Material.Shade400)
           : ""

    gradient: (Material.theme === Material.Light)
              ? _playbarGradient
              : ""

    Gradient {
        id: _playbarGradient
        GradientStop { position: 0.0; color: Material.color(Material.Yellow) }
        GradientStop { position: 1.0; color: Material.color(Material.DeepOrange, Material.ShadeA100) }
    }
    RowLayout {
        anchors.fill: parent
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: root.width / 3
            Layout.minimumWidth: 215

            color: "transparent"

            RowLayout {
                anchors.fill: parent

                Row {
                    width: 350
                    spacing: 6

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.margins: 15

                    Rectangle {
                        id: prevMusic
                        width: 20
                        height: 20
                        color: "transparent"

                        Image {
                            width: parent.width
                            height: parent.height
                            source: prevOrNextSoundIcoSource
                            mirror: true
                            fillMode: Image.PreserveAspectFit
                        }

                        MouseArea {
                            id: prevArea
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked:  {
                                _audioPlayerController.switchToPrevSong()
                                if (isSongPlaying)
                                    _player.play()
                            }
                        }
                    }
                    Rectangle {
                        id: _playOrPauseMusic
                        width: 20
                        height: 20
                        color: "transparent"

                        Image {
                            width: parent.width
                            height: parent.height
                            source: isSongPlaying ? pauseIcoSource
                                                  : playIcoSource
                            fillMode: Image.PreserveAspectFit
                        }

                        MouseArea {
                            id: playArea
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked:  {
                                if (!_player.hasAudio)
                                    return

                                if (isSongPlaying) {
                                    isSongPlaying = false
                                    _player.pause()
                                } else {
                                    isSongPlaying = true
                                    _player.play()
                                }

                            }
                        }
                    }
                    Rectangle {
                        id: nextMusic
                        width: 20
                        height: 20
                        color: "transparent"

                        Image {
                            width: parent.width
                            height: parent.height
                            source: prevOrNextSoundIcoSource
                            fillMode: Image.PreserveAspectFit
                        }

                        MouseArea {
                            id: nextArea
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked:  {
                                _audioPlayerController.switchToNextSong()
                                if (isSongPlaying)
                                    _player.play()
                            }
                        }
                    }
                }

                Slider {
                    id: _songVolume
                    width: 200
                    from: 1
                    value: 70
                    to: 100


                    Layout.alignment: Qt.AlignCenter

                    background: Rectangle {
                        x: _songVolume.leftPadding
                        y: _songVolume.topPadding + _songVolume.availableHeight / 2 - height / 2
                        implicitWidth: 125
                        implicitHeight: 4
                        width: _songVolume.availableWidth
                        height: implicitHeight
                        radius: 2
                        color: (Material.theme === Material.Dark)
                               ? Material.color(Material.Grey, Material.Shade200)
                               : Material.color(Material.Grey)
                        Rectangle {
                            width: _songVolume.visualPosition * parent.width
                            height: parent.height
                            color: (Material.theme === Material.Dark)
                                   ? Material.color(Material.Pink, Material.Shade100)
                                   : Material.color(Material.Brown)
                            radius: 2
                        }
                    }

                    handle: Rectangle {
                        x: _songVolume.leftPadding + _songVolume.visualPosition * (_songVolume.availableWidth - width)
                        y: _songVolume.topPadding + _songVolume.availableHeight / 2 - height / 2
                        implicitWidth: 20
                        implicitHeight: 20
                        radius: 13
                        color: _songVolume.pressed ? "#f0f0f0" : "#f6f6f6"
                        border.color: "#bdbebf"
                    }

                    onMoved: {
                        _player.volume = _songVolume.value / 100
                    }
                }
            }

        }
        Rectangle {
            Layout.preferredWidth: 2
            Layout.preferredHeight: 40
            Layout.alignment: Qt.AlignHCenter

            color: (Material.theme === Material.Dark)
                   ? Material.color(Material.Gray, Material.Shade100)
                   : Material.color(Material.DeepOrange, Material.ShadeA100)
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredWidth: root.width / 3
            Layout.minimumWidth: 350

            color: "transparent"

            RowLayout {
                anchors.fill: parent
                spacing: 2

                Rectangle {

                    Layout.maximumWidth: 50
                    Layout.maximumHeight: 50
                    Layout.minimumWidth: 50
                    Layout.minimumHeight: 50
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter

                    color: "transparent"
                    Image {
                        id: _noneSongImg
                        width: parent.width
                        height: parent.height
                        source: noneSongImageImgSource
                        fillMode: Image.PreserveAspectFit
                        visible: true
                    }
                }

                Slider {
                    id: _songLine
                    from: 1
                    value: _player.position / 1000
                    to: _player.duration / 1000

                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignBottom

                    background: Rectangle {
                        x: _songLine.leftPadding
                        y: _songLine.topPadding + _songLine.availableHeight / 2 - height / 2

                        implicitWidth: 200
                        implicitHeight: 4
                        width: _songLine.availableWidth
                        height: implicitHeight
                        radius: 2
                        color: "#bdbebf"
                        z: 1
                        Rectangle {
                            z: 2
                            width: _songLine.visualPosition * parent.width
                            height: parent.height
                            color: Material.color(Material.Brown, Material.Shade400)
                            radius: 2
                        }
                    }
                    handle: Rectangle {
                        x: _songLine.leftPadding + _songLine.visualPosition * (_songLine.availableWidth - width)
                        y: _songLine.topPadding + _songLine.availableHeight / 2 - height * 0.75
                        z: 99
                        implicitWidth: 4
                        implicitHeight: 10
                        color: Material.color(Material.Brown)
                    }

                    onMoved: {
                        _player.stop()
                        _player.seek(_songLine.value * 1000)
                        if (isSongPlaying)
                            _player.play()

                    }
                    BaseText {
                        x: 0
                        y: 0 - implicitHeight
                        text: "%1:%2".arg(currMinute).arg(convertSecondsToText(currSecond))
                    }
                    BaseText {
                        x: _songLine.width - implicitWidth
                        y: 0 - implicitHeight
                        text: "-%1:%2".arg(minutesLeftUntilToTheEnd)
                                     .arg(convertSecondsToText(secondsLeftUntilToTheEnd))
                    }
                }
            }

        }
        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredWidth: root.width / 3

            color: "transparent"
        }
    }
}
