#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdint.h>        // for uint32_t

#define PORT 8080
#define BUFFER_SIZE 1024

struct Server {
    int domain;
    int type;
    int protocol;

    int service; 
    int port;
    int backlog;
    uint32_t interface;
    struct sockaddr_in address;
    int sock;
    void (*start)(struct Server *server);
};

struct Server server_constructor(int domain, int service, int protocol,
                                 int port, int backlog, uint32_t interface,
                                 void (*start)(struct Server *server));

#endif
