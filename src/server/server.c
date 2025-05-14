#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

struct Server server_constructor(int domain, int service, int protocol,
                                 int port, int backlog, u_long interface,
                                void (*start)(struct Server *server)){


struct Server server;
server.domain = domain;
server.service =  service;
server.protocol = protocol;
server.port = port;
server.backlog = backlog;
server.interface = interface;
server.start = start;



server.address.sin_family = domain;
server.address.sin_port = htons(port); //converts int to network byte type htons
server.address.sin_addr.s_addr = htonl(interface);


server.socket = socket(domain , service, protocol);

if (server.socket == 0){

    perror("Failed to connect to socket..\n");
    exit(1);
}


if ((bind(server.socket, (struct sockaddr*)&server.address, sizeof(server.address))) < 0){

    perror("Failed to bind socket... \n");
    exit(1);

}

if((listen(server.socket, server.backlog)) <0){

    perror("Failed to start listening...\n");
    exit(1);
}

server.launch = launch;

return server;





                                    
                                }