#ifndef SERVER_SOCKET_SERVER_H
#define SERVER_SOCKET_SERVER_H

#include <stdint.h>

/**
 * nw : north-west
 * ne : north-east
 * sw : south-west
 * se : south-east
 */
enum direction {none, north, west, east, south, nw, ne, sw, se};

class Server {
public:
    Server();
    ~Server();

    int sendCoordinate(uint32_t x, uint32_t y, uint8_t d, uint16_t x_l, uint16_t y_l, uint16_t length, uint16_t angle);
    int sendNextStep();

private:
    int init_socket();
    int close_socket();

    int client_fd;
    int server_fd;
};

#endif //SERVER_SOCKET_SERVER_H
