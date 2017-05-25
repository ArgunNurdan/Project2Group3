#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server.h"
#include "ConstructorErrException.h"

#define PORT 8080

Server::Server() : server_fd(-1), client_fd(-1)
{
    if(init_socket() != 0)
        throw ConstructorErrException();
}

Server::~Server()
{
    close_socket();
}

int Server::close_socket()
{
    if(server_fd != -1 && close(server_fd) != 0){
        perror("");
        return 1;
    }

    if(client_fd != -1 && close(client_fd) != 0){
        perror("");
        return 1;
    }

    return 0;
}

int Server::init_socket()
{
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        return 1;
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        return 1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        perror("bind failed");
        return 1;
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        return 1;
    }
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        return 1;
    }
}

int Server::sendCoordinate(uint32_t x, uint32_t y, uint8_t d, uint16_t x_l, uint16_t y_l, uint16_t length, uint16_t angle)
{
    unsigned char buf[18];

    //init buffer
    *buf = 1;
    *((uint32_t*)(buf+1)) = htonl(x);
    *((uint32_t*)(buf+5)) = htonl(y);
	buf[9] = d;
    *((uint16_t*)(buf+10)) = htons(x_l);
    *((uint16_t*)(buf+12)) = htons(y_l);
    *((uint16_t*)(buf+14)) = htons(length);
    *((uint16_t*)(buf+16)) = htons(angle);

    //send buffer
    send(client_fd, buf, 18, 0);

    return 0;
}

int Server::sendNextStep()
{
    unsigned char buf = 0;

    send(client_fd, &buf, 1, 0);

    return 0;
}

