#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>  // For htons, htonl

struct Server server_constructor(int domain, int service, int protocol,
                                 int port, int backlog, uint32_t interface,
                                 void (*start)(struct Server *server)) {
    struct Server server;

    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.port = port;
    server.backlog = backlog;
    server.interface = interface;
    server.start = start;

    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(interface);

    server.sock = socket(domain, service, protocol);
    if (server.sock == 0) {
        perror("Failed to create socket");
        exit(1);
    }

    if (bind(server.sock, (struct sockaddr*)&server.address, sizeof(server.address)) < 0) {
        perror("Failed to bind socket");
        exit(1);
    }

    if (listen(server.sock, server.backlog) < 0) {
        perror("Failed to start listening");
        exit(1);
    }

    return server;
}
