#include "../server.h"
#include "requests.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


void server_start(struct Server *server) {
    printf("Server started on port %d\n", server->port);
    
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd;
    
    while (1) {

        client_fd = accept(server->sock, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd < 0) {
            perror("Failed to accept connection");
            continue;
        }
        

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
        printf("Connection accepted from %s:%d\n", client_ip, ntohs(client_addr.sin_port));
        

        handle_client_request(client_fd);
        
        printf("Connection closed with %s:%d\n", client_ip, ntohs(client_addr.sin_port));
    }
}