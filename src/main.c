#include "requests/requests.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>


void server_start(struct Server *server);


struct Server server;


void handle_signal(int sig) {
    printf("\nShutting down server...\n");
    close(server.sock);
    exit(0);
}

int main() {
   
    signal(SIGINT, handle_signal);  
    signal(SIGTERM, handle_signal); 

    server = server_constructor(
        AF_INET,           
        SOCK_STREAM,       
        0,                 
        PORT,             
        10,                
        INADDR_ANY,        
        server_start       
    );
    
    printf("Simple HTTP Server initialized\n");
    printf("Listening on all interfaces, port %d\n", PORT);
    printf("Press Ctrl+C to shutdown the server\n");
    

    server.start(&server);
    
  
    return 0;
}