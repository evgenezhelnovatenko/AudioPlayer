#ifndef AUDIOPLAYERCONTROLLER_H
#define AUDIOPLAYERCONTROLLER_H

#include <QObject>
#include <audioplayer.h>

class AudioPlayerController : public QObject
{
    Q_OBJECT

public:
    explicit AudioPlayerController(QObject *parent = nullptr);

    Q_INVOKABLE void switchToNextSong();
    Q_INVOKABLE void switchToPrevSong();
    Q_INVOKABLE void songChange();
    Q_INVOKABLE AudioPlayer *getModel();
    Q_INVOKABLE void downloadJsonData();

public slots:
    void songsAreOver();
signals:
    void stopPlayingMusic();
private:
    AudioPlayer player;
    QString m_filepath;


};

#endif // AUDIOPLAYERCONTROLLER_H
