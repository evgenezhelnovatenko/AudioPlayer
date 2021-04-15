pragma Singleton
import QtQuick 2.0

Item {
    property alias playbar: _playbar
    property alias songsList: _songsList

    QtObject {
        id: _playbar
        readonly property string playIco: "qrc:/qml/resources/img/svg/music_player_play_icon.svg"
        readonly property string pauseIco: "qrc:/qml/resources/img/svg/music_player_pause_icon.svg"
        readonly property string prevOrNextSoundIco: "qrc:/qml/resources/img/svg/next_sound_arrow_icon_123825.svg"
        readonly property string loopIco: "qrc:/qml/resources/img/svg/loop_icon.svg"
        readonly property string shuffleSongsIco: "qrc:/qml/resources/img/svg/shuffle_songs_icon.svg"
        readonly property string noneSongImageImg: "qrc:/qml/resources/img/songImageNotFoundImg.jpg"
    }

    QtObject {
        id: _songsList
        readonly property string volumeSoundIco: "qrc:/qml/resources/img/svg/volume_control_sound_icon_.svg"
    }
}
