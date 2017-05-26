#ifndef CLIENT_SOCKET_CLIENT_H
#define CLIENT_SOCKET_CLIENT_H

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#else

#include <stdint.h>

#endif

/**
 * nw : north-west
 * ne : north-east
 * sw : south-west
 * se : south-east
 */
enum direction {none, north, west, east, south, nw, ne, sw, se};

class Client {
public:
    Client();
    ~Client();
    /**
     * @return 0 on success, 1 on error
     */
    int getCoordinates(uint32_t& x, uint32_t& y, uint8_t& d, uint16_t& x_l, uint16_t& y_l, uint16_t& length, uint16_t& angle);
    /**
     * @return opcode on success, -1 on error
     */
    int waitDataFromServer(); //returns opcode

private:
    /**
     * @return 0 on success, 1 on error
     */
    int init_client();
    /**
     * @return 0 on success, 1 on error
     */
    int close_client();
#ifdef _WIN32
    SOCKET ConnectSocket;
#else
    int sock;
#endif
};

#endif //CLIENT_SOCKET_CLIENT_H