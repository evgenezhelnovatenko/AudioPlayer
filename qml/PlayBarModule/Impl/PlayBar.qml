import QtQuick 2.0
import QtQuick.Layouts 1.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

Rectangle {
    id: root
    gradient: Gradient {
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
            Layout.maximumWidth: 300

            color: "transparent"

            Row {
                width: 350
                spacing: 6
                anchors.fill: parent
                anchors.margins: 15

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
                Slider {
                    id: _songVolume
                    from: 1
                    value: 70
                    to: 100

                    anchors.verticalCenter: parent.verticalCenter

                    background: Rectangle {
                        x: _songVolume.leftPadding
                        y: _songVolume.topPadding + _songVolume.availableHeight / 2 - height / 2
                        implicitWidth: 125
                        implicitHeight: 4
                        width: _songVolume.availableWidth
                        height: implicitHeight
                        radius: 2
                        color: Material.color(Material.Grey)

                        Rectangle {
                            width: _songVolume.visualPosition * parent.width
                            height: parent.height
                            color: Material.color(Material.Brown)
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

            color: Material.color(Material.DeepOrange, Material.ShadeA100)
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredWidth: root.width / 3
            Layout.minimumWidth: 350

            color: "transparent"

            Slider {
                id: _songLine
                from: 1
                value: _player.position / 1000
                to: _player.duration / 1000

                anchors.fill: parent
                anchors.rightMargin: 15

                background: Rectangle {
                    x: _songLine.leftPadding
                    y: _songLine.topPadding + _songLine.availableHeight / 2 - height / 2
                    implicitWidth: 200
                    implicitHeight: 4
                    width: _songLine.availableWidth
                    height: implicitHeight
                    radius: 2
                    color: "#bdbebf"

                    Rectangle {
                        width: _songLine.visualPosition * parent.width
                        height: parent.height
                        color: Material.color(Material.Brown, Material.Shade400)
                        radius: 2
                    }
                }
                handle: Rectangle {
                    x: _songLine.leftPadding + _songLine.visualPosition * (_songLine.availableWidth - width)
                    y: _songLine.topPadding + _songLine.availableHeight / 2 - height * 0.75
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
