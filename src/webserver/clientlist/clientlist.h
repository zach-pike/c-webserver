#pragma once

#include "client/client.h"
typedef struct client_list {
    client_t* client_list;

    uint32_t connected_count;
    uint32_t max_size;
} client_list_t;

void client_list_initialize(client_list_t* this);
void client_list_destroy(client_list_t* this);

uint32_t client_list_add(client_list_t* this, client_t* clientToAdd);

void client_list_remove(client_list_t* this, uint32_t index);

void client_list_tick_all(client_list_t* this);