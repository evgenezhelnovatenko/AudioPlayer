#ifndef FTP_H
#define FTP_H

#pragma comment(lib,"Ws2_32.lib")

#include <QObject>

#include <winsock2.h>
#include <windows.h>

#define HOSTNAME_LENGTH 30
#define FILENAME_LENGTH 30
#define BUFFER_SIZE 512
#define LISTEN_PORT 1234

typedef struct {
    int header;
    char buffer[BUFFER_SIZE];
    int footer;
} Packet;


typedef struct {
    int dir;
    char filename[FILENAME_LENGTH];
} Frame;

class Ftp : public QObject
{
    Q_OBJECT

    private:
        int sock;						/* Socket descriptor */
        struct sockaddr_in ServAddr;	/* server socket address */
        unsigned short ServPort;		/* server port */
        WSADATA wsaData;
        char myHostname[HOSTNAME_LENGTH];

        bool bOptVal = 1;
        int bOptLen = sizeof (bool);


    public:
        void Init();
        int createSocket();
        unsigned long ResolveName(char name[]);
        bool file_exists(char * filename);
        bool recvFileAndWrite(char * filename, int sock);
        void err_sys(char *fmt,...);

};

#endif // FTP_H
