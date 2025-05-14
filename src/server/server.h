#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

#define PORT 8080
#define BUFFER_SIZE 1024

struct Server{

    int domain;
    int type;
    int protocol;

    int service; 
    int port;
    int backlog;
    u_long interface;
    struct sockaddr_in address;
    int sock;
    void(*start)(struct Server *server);

};


struct Server server_constructor(int domain, int service, int protocol,
                                 int port, int backlog, u_long interface,
                                 void (*start)(struct Server *server));


#endif 