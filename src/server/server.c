#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define PORT 8080
#define BUFFER_SIZE 1024

int main(){

    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_size;
    char buffer[BUFFER_SIZE];
    int bytes_recieved;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd==-1){

        perror("socket");
        exit(1);
    }

    //binding to 8080 port

    server_addr.sin_family  = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; //adddress to accept any incoming messages
    memset(&(server_addr.sin_zero), 0, 8);
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        exit(1);
    }

    printf("Server is listening on Port %d...\n", PORT);
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        close(server_fd);
        exit(1);
    }

    //accepting a aconnection
    sin_size = sizeof(struct sockaddr_in);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr,&sin_size);
    if (client_fd==-1){

        perror("Accept");
        close(server_fd);
        exit(1);

    }

    printf("got connection from %s\n", inet_ntoa(client_addr.sin_addr));

    //recieving data

    memset(buffer,0, BUFFER_SIZE);
    bytes_recieved = recv(client_fd, buffer, BUFFER_SIZE-1,0);
    if (bytes_recieved==-1){
        perror("recv");

    }
    else{
        printf("client sent: %s\n", buffer);
    };

    char *response = "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/plain\r\n"
                 "Content-Length: 13\r\n"
                 "\r\n"
                 "Hello, world!";

    send(client_fd, response, strlen(response), 0);

    close(client_fd);
    close(server_fd);
    
}