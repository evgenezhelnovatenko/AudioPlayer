#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>

#include "audioplayercontroller.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8888"

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

    bool validateTheParameters(int argc);
    void setupHints();
    bool initializeWinsock();
    bool resolveTheServerAddressAndPort();
    bool connectToServer();
    bool shutdownConnectionUntilNewMsg();
    void receiveDataFromServer(QByteArray &dataFromServer, bool isDataAFilename = false);
    void cleanup();

    bool sendMessage(QString msg);
    void downloadFileFromServer();

signals:
    void serverErrorDetected();

private:
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen;

    // private methods
    bool isWinsockError();
    void printErrorMessage(QString errorMsg, int errorNumber = 0);

};

#endif // CLIENT_H
