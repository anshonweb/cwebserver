#include "requests/requests.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

// Forward declaration of server_start function defined in server_impl.c
void server_start(struct Server *server);

// Global server variable to handle cleanup on signal
struct Server server;

// Signal handler for graceful shutdown
void handle_signal(int sig) {
    printf("\nShutting down server...\n");
    close(server.sock);
    exit(0);
}

int main() {
    // Register signal handlers for graceful shutdown
    signal(SIGINT, handle_signal);  // Ctrl+C
    signal(SIGTERM, handle_signal); // kill command
    
    // Create the server
    server = server_constructor(
        AF_INET,           // IPv4
        SOCK_STREAM,       // TCP
        0,                 // Default protocol
        PORT,              // Port from server.h (8080)
        10,                // Connection backlog
        INADDR_ANY,        // Listen on all interfaces
        server_start       // Start function
    );
    
    printf("Simple HTTP Server initialized\n");
    printf("Listening on all interfaces, port %d\n", PORT);
    printf("Press Ctrl+C to shutdown the server\n");
    
    // Start the server (this calls the server_start function)
    server.start(&server);
    
    // This point is never reached in normal operation because server_start contains an infinite loop
    return 0;
}