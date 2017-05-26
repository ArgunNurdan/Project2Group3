#define IP "192.168.43.42"

#ifdef _WIN32

#include "client.h"
#include <stdio.h>
#include <string.h>
#include "ConstructorErrException.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_PORT "8080"
#define DEFAULT_BUFLEN 512


Client::Client()
{
    if(init_client() != 0)
        throw ConstructorErrException();
}

Client::~Client()
{
    close_client();
}

int Client::init_client()
{

    ConnectSocket = INVALID_SOCKET;
    WSADATA wsaData;
    struct addrinfo *result = NULL,
            *ptr = NULL,
            hints;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(IP, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
                               ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }
    return 0;
}

int Client::close_client()
{
    closesocket(ConnectSocket);
    WSACleanup();
    return 0;
}

#else

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <cstdlib>
#include "client.h"
#include "ConstructorErrException.h"

#define PORT 8080

Client::Client() : sock(-1)
{
    if(init_client() != 0)
        throw ConstructorErrException();
}

Client::~Client()
{
    close_client();
}

int Client::init_client()
{
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, IP, &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("conneciton failed");
        exit(1);
    }

    return 0;
}

int Client::close_client()
{
    return (sock != -1) && (close(sock) != 0);
}


#endif

int Client::getCoordinates(uint32_t& x, uint32_t& y, uint8_t& d, uint16_t& x_l, uint16_t& y_l, uint16_t& length, uint16_t& angle)
{

#ifdef _WIN32
    char buf[17];
    if(recv(ConnectSocket, buf, 17, 0) != 17)
#else
        unsigned char buf[17];
    if(recv(sock, buf, 17, 0) != 17)
#endif
        return 1;

    x = ntohl(*(uint32_t*)buf);
    y = ntohl(*(uint32_t*)(buf+4));
    d = buf[8];
    x_l = ntohs(*(uint16_t*)(buf+9));
    y_l = ntohs(*(uint16_t*)(buf+11));
    length = ntohs(*(uint16_t*)(buf+13));
    angle = ntohs(*(uint16_t*)(buf+15));

    return 0;
}

int Client::waitDataFromServer()
{
#ifdef _WIN32
    char buf[1];
    if(recv(ConnectSocket, buf, 1, 0) == 1)
#else
        unsigned char buf[1];
    if(recv(sock, buf, 1, 0) == 1)
#endif
        return *buf;

    return -1;
}