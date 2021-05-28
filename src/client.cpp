#include "client.h"
#include <QDataStream>
#include <QSaveFile>

Client::Client(QObject *parent)
    : QObject (parent)
    , recvbuflen (DEFAULT_BUFLEN)
{


    if (!initializeWinsock())
        emit serverErrorDetected();

    setupHints();

    if (!resolveTheServerAddressAndPort())
        emit serverErrorDetected();

    if (!connectToServer())
        emit serverErrorDetected();

}

Client::~Client()
{
//    delete sendbuf;
    cleanup();
}

bool Client::sendMessage(QString msg)
{
    QByteArray ba = msg.toLocal8Bit();
    const char* message = ba.data();
    iResult = send(ConnectSocket, message, (int)strlen(message), 0);
    if (iResult == SOCKET_ERROR) {
        printErrorMessage("send failed with error: ", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return false;
    }
    qDebug() << "Bytes Sent: " << iResult;

    shutdownConnectionUntilNewMsg();
    downloadFileFromServer();
    delete message;
    return true;
}

void Client::downloadFileFromServer()
{
    QSaveFile fileFromServer;
    QByteArray data;

    receiveDataFromServer(data, true);

    QString filename(data);

    fileFromServer.setFileName(filename);
    if (!fileFromServer.open(QIODevice::WriteOnly)) {
        qDebug() << "Музичний файл не було створено!";
        return;
    }

    data.clear();
    receiveDataFromServer(data);
    fileFromServer.write(data);
    fileFromServer.commit();
}

void Client::setupHints()
{
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
}


bool Client::isWinsockError()
{
    return (iResult != 0);
}

void Client::printErrorMessage(QString errorMsg, int errorNumber)
{
    if (errorNumber == 0) {
        qDebug() << errorMsg;
        return;
    }
    qDebug() << errorMsg << errorNumber;
}

bool Client::validateTheParameters(int argc)
{
    return (argc == 2);
}

bool Client::initializeWinsock()
{
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (isWinsockError()) {
        printErrorMessage("WSAStartup failed with error: ", iResult);
        return false;
    }
    return true;
}

bool Client::resolveTheServerAddressAndPort()
{
    const char* hostUrl = "192.168.0.100";
    iResult = getaddrinfo(hostUrl, DEFAULT_PORT, &hints, &result);
    if (isWinsockError()) {
        printErrorMessage("getaddrinfo failed with error: ", iResult);
        WSACleanup();
        return false;
    }
    return true;
}

bool Client::connectToServer()
{
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printErrorMessage("socket failed with error: ", WSAGetLastError());
            WSACleanup();
            return false;
        }

        // Connect to server.
        iResult = ::connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen); // Завдяки :: метод connect Winsock-у не кофліктує з методом connect QObject-а.

        if (iResult == SOCKET_ERROR) {
            printErrorMessage("connect failed with error: ", WSAGetLastError());
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printErrorMessage("Unable to connect to server!");
        WSACleanup();
        return false;
    }

    return true;
}

bool Client::shutdownConnectionUntilNewMsg()
{
    // Вимкнути з'єднання доки не будуть відправлені нові дані.
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printErrorMessage("shutdown failed with error: ", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return false;
    }
    return true;
}

void Client::receiveDataFromServer(QByteArray &dataFromServer, bool isDataAFilename)
{

    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);

        if ( iResult > 0 ) {
            dataFromServer.append(recvbuf, iResult);
            if (isDataAFilename)
                // Якщо це передається ім'я файлу, то необхідно отримати його та завершити приймати дані.
                break;
        }
        else if ( iResult == 0 )
            qDebug() << "Connection closed";
        else
            printErrorMessage("recv failed with error: ", WSAGetLastError());

    } while( iResult > 0 );


}

void Client::cleanup()
{
    closesocket(ConnectSocket);
    WSACleanup();
}
