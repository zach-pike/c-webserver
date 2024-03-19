#include "clientlist.h"

#include <stdlib.h>
#include <string.h>

void client_list_initialize(client_list_t* this) {
    this->max_size = 32;
    this->connected_count = 0;
    this->client_list = (client_t*)malloc(sizeof(client_t)*this->max_size);
}

void client_list_destroy(client_list_t* this) {
    if (this->client_list == NULL) return;

    // Destroy all elements 
    for (uint32_t i=0; i<this->connected_count; i++) {
        client_destroy(&this->client_list[i]);
    }

    free((void*)this->client_list);
    this->client_list = NULL;
}

void _client_list_realloc(client_list_t* this) {
    this->max_size += 10;
    this->client_list = (client_t*)realloc((void*)this->client_list, sizeof(client_t)*this->max_size);
}

uint32_t client_list_add(client_list_t* this, client_t* client_to_add) {
    // Resize if needed
    if (this->connected_count == (this->max_size - 1)) _client_list_realloc(this);

    client_move(&this->client_list[this->connected_count], client_to_add);

    this->connected_count += 1;
    return this->connected_count - 1;
}

void client_list_remove(client_list_t* this, uint32_t index) {
    // 1. Deinit the item to remove
    client_destroy(&this->client_list[index]);

    // 2. Shift rest of data
    memcpy(&this->client_list[index], &this->client_list[index + 1], this->connected_count - index);

    // 3. Subtract 1 from connected count
    this->connected_count -= 1;
}

void client_list_tick_all(client_list_t* this) {
    for (uint32_t i=0; i<this->connected_count; i++) {
        client_tick(&this->client_list[i]);
    }
}