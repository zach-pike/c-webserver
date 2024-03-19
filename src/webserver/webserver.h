#pragma once

#include <stdint.h>
#include <netinet/in.h>

#include "clientlist/clientlist.h"

typedef struct webserver {
    int socket;

    uint16_t port;
    const char* host;

    struct sockaddr_in address;

    client_list_t clientlist;
} webserver_t;

void webserver_initialize(webserver_t* ws, const char* host, uint16_t port);
void webserver_destroy(webserver_t* this);

void webserver_tick(webserver_t* this);