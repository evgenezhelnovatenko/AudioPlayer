#include "ftpclient.h"
#include <QDebug>
#include <QByteArray>
#include <QThread>
#include <QTextStream>
#include <QBuffer>
#include <QFile>
#include <QList>

FtpClient::FtpClient()
{

}

FtpClient::~FtpClient(){
    WSACleanup();
    qDebug() << "FtpClient::~FtpClient()";
}


void FtpClient::connectToServer()
{

    if (connectionStatus() == 1) {
        emit serverReadyToRequest();
        return;
    }

    if (!createConnection()) {
        emit connectionFailed();
        return;
    }

    emit serverReadyToRequest();
}

void FtpClient::getMusicFile(QString filename){
    char buf[1024];

    qDebug() << "\nFTP > ";
    QString cmd = "download " + filename;

    QByteArray ba = cmd.toLocal8Bit();

    send(sock, ba.data(), (int)strlen(ba.data()), 0);
    debug = recv(sock, buf, sizeof(buf), 0);
    buf[debug] = 0;

    if (strcmp(buf, "\nData stream to download established") == 0) {
        qDebug() << "\nDownloading...";
        send(sock, "ok", sizeof(char) * 3, 0);
        debug = recv(sock, buf, sizeof(buf), 0);
        buf[debug] = 0;
        Ftp::recvFileAndWrite(buf, sock);
    }
    if (strcmp(buf, "\nData stream to get value from DB established") == 0) {
        qDebug("\nData from DB received");
    }

    qDebug() << buf;

}

void FtpClient::getAllMusicFilesInfo()
{
    Packet p;
    p.header = 1;
    p.footer = 0;

    char buf[1024];

    qDebug() << "\nFTP > ";

    QString cmd = "get all-music";
    QByteArray ba = cmd.toLocal8Bit();

    send(sock, ba.data(), (int)strlen(ba.data()), 0);
    debug = recv(sock, buf, sizeof(buf), 0);
    buf[debug] = 0;

    if (strcmp(buf, "\nData stream to send JSON established") == 0) {
        qDebug() << "\nGet the file...";
        send(sock, "ok", sizeof(char) * 2, 0);

        QString jsonStr = "";
        QTextStream out(&jsonStr);
        QBuffer buffer;

        if (!buffer.open(QIODevice::WriteOnly)) {
            qDebug() << "\nError opening buffer to write json\n";
            return;
        }

//        QFile f("json.txt");
//        if (!f.open(QIODevice::WriteOnly)) {
//            qDebug() << "error open file";
//            return;
//        }

        while (true) {
            debug = recv(sock, (char*)&p, sizeof(Packet),0);
            if (p.footer == 0) {
                // write to string
                buffer.write(p.buffer, BUFFER_SIZE);

                // write to file
//                f.write(p.buffer, BUFFER_SIZE);

            }
            else {
                // write to string
                buffer.write(p.buffer, p.footer);

                // write to file
//                f.write(p.buffer, p.footer);
                break;
            }
        }

        out << buffer.buffer();

        buffer.close();
//        f.close();

        QList<Song> songlist;

        if (!Serializer::JSONDesirealizer(songlist, jsonStr))
            return;

        emit sendListOfMusicFromServerToModel(songlist);
    }

}


bool FtpClient::doDownload(Frame f){
    return 1;
}


bool FtpClient::doShutdown(){
    qDebug() << "\nClosing! <3";

    // shutdown the send half of the connection since no more data will be sent
    if (shutdown(sock, 2) == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(sock);
    WSACleanup();
    return 0;
}

bool FtpClient::createConnection()
{
    // Load basic starting routine  (WSAstartup & local hostname)
    if (!Ftp::Init())
        return false;

    // Create the socket
    sock = Ftp::createSocket();

    if (sock == SOCKET_ERROR || (SOCKET)sock == INVALID_SOCKET)
        return false;

    //connect to the server
    memset(&ServerAddr, 0, sizeof(ServerAddr));     /* Zero out structure */

    ServerAddr.sin_family      = AF_INET;             /* Internet address family */
    ServerAddr.sin_addr.s_addr = ResolveName(remoteHostname);   /* Server IP address */
    ServerAddr.sin_port        = htons(CONNECT_ON_PORT); /* Server port */

    int iResult = 0;
    iResult = ::connect(sock, (struct sockaddr *) &ServerAddr, sizeof(ServerAddr));
    if (iResult < 0) {
        QString errorMsg("Socket Creating Error");
//        err_sys((char *)errorMsg.data());
        return false;
    }
    qDebug() << "\nConnected to the server successfully";
    status = 1;

    return true;
}
