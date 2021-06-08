#include "ftp.h"
#include <QDebug>
#include <QFile>


QString Ftp::getDEFAULT_FOLDER() const
{
    return DEFAULT_FOLDER;
}

void Ftp::setDEFAULT_FOLDER(const QString &value)
{
    DEFAULT_FOLDER = value;
}

bool Ftp::Init(){
    qDebug() << "/***********************************/";
    qDebug() << "/*  Welcome to FTP Client   */";
    qDebug() << "/***********************************/";

    // Initialize WinSock
    if (WSAStartup(0x0202,&wsaData) != 0)	{
        WSACleanup();
        QString errorMsg("Error in starting WSAStartup()");
//        err_sys((char *)errorMsg.data());
        return false;
    }
    qDebug() << "Winsock started.\n";
    // Get and print our own hostname
    if(gethostname(myHostname, HOSTNAME_LENGTH) != 0 ) {
        QString errorMsg("can not get the host name,program exit");
//        err_sys((char *)errorMsg.data());
        return false;
    }
    qDebug() << "Starting at host: " << myHostname;

    return true;
}


int Ftp::createSocket(){
    int iResult = 0;
    //Create the socket
    sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if((SOCKET)sock == INVALID_SOCKET ) {
        QString errorMsg("Invalid Socket ");
//        err_sys((char *)errorMsg.data(),WSAGetLastError());
        return sock;
    }
    else if(sock==SOCKET_ERROR) {
        QString errorMsg("Socket Error)");
//        err_sys((char *)errorMsg.data(),WSAGetLastError());
        return sock;
    }
    else
        qDebug()<<"SOCKET Established";

    iResult = setsockopt(sock,SOL_SOCKET,SO_REUSEADDR, (char *) &bOptVal, bOptLen);
    if (iResult == SOCKET_ERROR) {
        QString errorMsg("setsockopt for SO_REUSEADDR failed with error: ");
//        err_sys((char *)errorMsg.data(), WSAGetLastError());
        return sock;
    }
    else
        qDebug()<<"Set SO_REUSEADDR: ON";

    return sock;
}


unsigned long Ftp::ResolveName(char name[]) {
    struct hostent *host;            /* Structure containing host information */

    if ((host = gethostbyname(name)) == NULL) {
        QString errorMsg("gethostbyname() failed");
        err_sys((char *)errorMsg.data());
    }

    /* Return the binary, network byte ordered address */
    return *((unsigned long *) host->h_addr_list[0]);
}



//bool Ftp::file_exists(char * filename){
//    QFile ifile(filename);
//    return (bool) ifile;
//}


bool Ftp::recvFileAndWrite(char* filename, int sock){
    Packet p;
    p.header = 1;
    p.footer = 0;
    int test;

    char fname[FILENAME_LENGTH+1] = "";
    strcat(fname, DEFAULT_FOLDER.toUtf8().data());
    strcat(fname, "\\");
    strcat(fname, filename);

    // Open
    qDebug() << "Opening the file for writing...\n";
    QFile os (fname);

    // Check if error
    if (!os.open(QIODevice::WriteOnly))  {
        qDebug() << "Error opening the file to write :/\n";
        return 0;
    }

    while ( true ) {
        test = recv(sock,(char*)&p, sizeof(Packet),0);

        if(p.footer == 0)
            os.write(p.buffer, BUFFER_SIZE);
        else{
            os.write(p.buffer, p.footer);
            break;
        }
    }
    qDebug() << "closing written file.\n";
    os.close();
    return 1;
}

void Ftp::err_sys(char *fmt,...)
{
    perror(NULL);
    va_list args;
    va_start(args,fmt);
    fprintf(stderr,"error: ");
    vfprintf(stderr,fmt,args);
    fprintf(stderr,"\n");
    va_end(args);
    exit(1);
}

int Ftp::connectionStatus()
{
    return status;
}



