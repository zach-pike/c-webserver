#include "webserver.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/socket.h>

int set_socket_blocking_enabled(int fd, int blocking) {
   if (fd < 0) return 0;
   int flags = fcntl(fd, F_GETFL, 0);
   if (flags == -1) return 0;
   flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
   return (fcntl(fd, F_SETFL, flags) == 0);
}

void webserver_initialize(webserver_t* ws, const char* host, uint16_t port) {
    client_list_initialize(&ws->clientlist);

    // Create socket
    ws->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (ws->socket < 0) {
        perror("Socket Failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    int opt = 1;
    if (setsockopt(ws->socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    ws->address.sin_family = AF_INET;
    ws->address.sin_addr.s_addr = inet_addr(host);
    ws->address.sin_port = htons(port);

    if (bind(ws->socket, (struct sockaddr*)&ws->address, sizeof(ws->address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(ws->socket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Set socket to no blocking
    set_socket_blocking_enabled(ws->socket, 0);
}

void webserver_destroy(webserver_t* this) {
    client_list_destroy(&this->clientlist);
    close(this->socket);
}

void webserver_tick(webserver_t* this) {
    // Try to accept new connection
    socklen_t addrlen = sizeof(struct sockaddr_in);
    struct sockaddr_in connection_address;
    int new_socket = accept(this->socket, (struct sockaddr*)&connection_address, &addrlen);

    // If new user accepted
    if (new_socket != -1) {
        // Accepted new user!
        client_t client;
        client_initialize(&client, new_socket, connection_address);
        client_list_add(&this->clientlist, &client);
    }

    // Tick all the clients
    client_list_tick_all(&this->clientlist);
}