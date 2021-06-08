import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15

import PopupMenuModule.Impl 1.0
import SongsListModule.Base 1.0
import Song 1.0

Rectangle {

    property var popupMenu: _popup.menu1

    id: root
    color: "transparent"
    clip: true

    Component {
        id: _highlight
        Rectangle {
            id: _qwer
            width: _songsList.width; height: 40
            color: (Material.theme === Material.Dark)
                   ? Material.color(Material.Pink, Material.Shade100)
                   : Material.color(Material.LightBlue, Material.Shade200)
            radius: 5
            y: _songsList.currentItem ? _songsList.currentItem.y : _qwer.y
            Behavior on y {
                SmoothedAnimation {
                    easing.type: Easing.Linear
                    duration:200;
                    maximumEasingTime:300
                    velocity : 1000
                }
            }
        }
    }

    ScrollView {
        id: _scrollView
        anchors.fill: _songsListBackground
        z: 99
        clip: true

        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff


        ListView {

            property var song
            property var autor
            property var genres: []
            property string genresStr: ""
            property var co_autors: []
            property string co_autorsStr: ""

            id: _songsList
            anchors.fill: parent
            delegate: SongsListDelegate {
                id: _songListDelegate
            }
            boundsBehavior: Flickable.StopAtBounds

            highlight: _highlight
            highlightFollowsCurrentItem: false

            activeFocusOnTab: true

            removeDisplaced: Transition {
                NumberAnimation { properties: "x,y"; duration: 150 }
            }

            Keys.onPressed: {
                if ((event.key === Qt.Key_Enter) || (event.key === Qt.Key_Return)) {

                    _audioPlayerController.setIndexOfIndices(_songsList.currentIndex)
                    _audioPlayerController.changeCurrentSongIndex()
                    isSongPlaying = true;
                    _player.play()
                }
            }

            Component.onCompleted: {
                _songsList.model = audioPlayer
            }

            Component {
                id: _dialogComponent

                Dialog {

                    id: _infoAboutSongDialog
                    width: 300

                    title: qsTr("Інформація про композицію")
                    standardButtons: Dialog.Ok
                    x: (_songsList.width - _infoAboutSongDialog.width) / 2
                    y: (_songsList.height - _infoAboutSongDialog.height) / 2

                    contentItem: Rectangle {

                        color: "transparent"
                        ColumnLayout {
                            id: _infoAboutSong
                            width: parent.width

                            spacing: 2
                            clip: true

                            RowLayout {

                                Layout.fillWidth: true
                                Layout.maximumHeight: 200
                                spacing: 2

                                BaseInfoText {
                                    text: qsTr("Назва: ")
                                    Layout.alignment: Qt.AlignTop
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.maximumWidth: _infoAboutSong.width / 3
                                }

                                BaseInfoText {
                                    text: _songsList.song.title
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.maximumWidth: _infoAboutSong.width * 2/3
                                }
                            }
                            BaseDemarcationLine {
                                Layout.fillWidth: true
                            }
                            RowLayout {

                                Layout.fillWidth: true
                                Layout.maximumHeight: 200
                                spacing: 2

                                BaseInfoText {
                                    text: qsTr("Рік: ")
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.maximumWidth: _infoAboutSong.width / 3
                                }
                                BaseInfoText {
                                    text: _songsList.song.year
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.maximumWidth: _infoAboutSong.width * 2/3
                                }
                            }
                            BaseDemarcationLine {
                                Layout.fillWidth: true
                            }
                            RowLayout {

                                Layout.fillWidth: true
                                Layout.maximumHeight: 200
                                spacing: 2

                                BaseInfoText {
                                    text: qsTr("Довжина: ")
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.maximumWidth: _infoAboutSong.width / 3
                                }
                                BaseInfoText {
                                    text: _songsList.getSongLengthStr(_songsList.song.length)
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.maximumWidth: _infoAboutSong.width * 2/3
                                }
                            }
                            BaseDemarcationLine {
                                Layout.fillWidth: true
                            }
                            RowLayout {

                                Layout.fillWidth: true
                                Layout.maximumHeight: 200
                                spacing: 2

                                BaseInfoText {
                                    text: qsTr("Автор: ")
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.maximumWidth: _infoAboutSong.width / 3
                                }
                                BaseInfoText {
                                    text: _songsList.autor.pseudonym === ""
                                            ? _songsList.getFullAutorName(_songsList.autor.firstname, _songsList.autor.lastname)
                                            : _songsList.autor.pseudonym
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.maximumWidth: _infoAboutSong.width * 2/3
                                }
                            }
                            BaseDemarcationLine {
                                Layout.fillWidth: true
                            }
                            RowLayout {

                                Layout.fillWidth: true
                                Layout.maximumHeight: 200
                                spacing: 2

                                BaseInfoText {
                                    text: qsTr("Жанр: ")
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.maximumWidth: _infoAboutSong.width / 3
                                }
                                BaseInfoText {
                                    text: _songsList.genresStr
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.maximumWidth: _infoAboutSong.width * 2/3
                                }
                            }
                            BaseDemarcationLine {
                                Layout.fillWidth: true
                            }
                            RowLayout {

                                Layout.fillWidth: true
                                Layout.maximumHeight: 200
                                spacing: 2

                                BaseInfoText {
                                    text: qsTr("Співвиконавці: ")
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.maximumWidth: _infoAboutSong.width / 3
                                }
                                BaseInfoText {
                                    text: _songsList.co_autorsStr
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.maximumWidth: _infoAboutSong.width * 2/3
                                }
                            }
                            BaseDemarcationLine {
                                Layout.fillWidth: true
                            }
                        }
                    }



                }

            }
            Loader {
                id: _loader

            }


            onGenresChanged: {
                genresStr = getGenresString()
            }
            onCo_autorsChanged: {
                co_autorsStr = getCoAutorsString()
            }

            function getGenresString() {
                var genresString = ""
                genres.forEach((genre, index, arr) => {
                                   genresString += genre.name
                                   if (index < arr.length - 1)
                                        genresString += ", "
                               })
                return genresString
            }
            function getCoAutorsString() {
                var co_autorsString = ""
                co_autors.forEach((co_autor, index, arr) => {
                                      if (co_autor.pseudonym === "")
                                          co_autorsString += getFullAutorName(co_autor.firstname, co_autor.lastname)
                                      else {
                                          co_autorsString += co_autor.pseudonym
                                      }
                                      if (index < arr.length - 1)
                                          co_autorsString += ", "
                                  })
                return co_autorsString
            }
            function getFullAutorName(firstname, lastname) {
                return firstname + " " + lastname
            }
            function getSongLengthStr(length) {
                return Math.floor(length / 60000).toString() + ":" + (length / 1000 % 60).toString()
            }

        }

    }

    PopupMenuView {
        id: _popup

    }

    Rectangle {
        z: 1
        id: _songsListBackground
        color: (Material.theme === Material.Light)
               ? Material.color(Material.Lime, Material.Shade200)
               : Material.color(Material.DeepPurple, Material.Shade200)
        opacity: 0.25
        radius: 5
        anchors.fill: root
    }





    function deleteSong(index) {
        var currentSongPosition = _player.position

        _player.playlist.removeItem(index)
        _audioPlayerController.deleteSong(index)

        _player.seek(currentSongPosition)
    }

    function changePopupMenu() {
        if (popupMenu === _popup.menu1)
            popupMenu = _popup.menu2
        else if (popupMenu === _popup.menu2)
            popupMenu = _popup.menu1
    }
}
