#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QObject>
#include <memory>
#include "ftp.h"
#include "serializer.h"

#define CONNECT_ON_PORT 8888

class FtpClient : public Ftp
{
    Q_OBJECT

public:
    FtpClient();
    ~FtpClient();
    bool doDownload(Frame);
    bool doShutdown();
    int toCommandCode(QString);
    bool createConnection();

public slots:
    void connectToServer();
    void getMusicFile(QString filename);
    void getAllMusicFilesInfo();

signals:
    void sendListOfMusicFromServerToModel(const QList<Song>& songlist);
    void connectionFailed();
    void serverReadyToRequest();

private:
    struct sockaddr_in ServerAddr;		/* Server address */
    char remoteHostname[HOSTNAME_LENGTH] = "192.168.0.100";
    int sock;
    int debug;

};

#endif // FTPCLIENT_H
