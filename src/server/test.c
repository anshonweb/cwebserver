#include <stdio.h>
#include "server.h"


void launch(struct Server *server){

    printf("==== WAITING FOR CONNECTION====");
    int addres_length = sizeof(server->address);
    int new_socket = accept(server->sock, (struct sockaddr*)&server.address, sizeof(server->address, (socklen_t*)addres_length))
}

int main()

{



    struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0 , INADDR_ANY, 80, 10);

}