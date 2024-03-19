#pragma once

#include <netinet/in.h>

#include "buffer/buffer.h"

typedef struct client {
    int socket;
    struct sockaddr_in address;

    buffer_t in_buffer;
    uint32_t in_buffer_bytes_read;
} client_t;

void client_initialize(client_t* this, int socket, struct sockaddr_in address);
void client_destroy(client_t* this);

// Move internal data
void client_move(client_t* newloc, client_t* oldloc);

// Update client
void client_tick(client_t* this);