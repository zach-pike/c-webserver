#include "buffer.h"

#include <stdlib.h>

#include <string.h>

void buffer_initialize(buffer_t* this, uint32_t size) {
    this->buffer = (uint8_t*)malloc(size);
    this->size = size;
}

void buffer_destroy(buffer_t* this) {
    free((void*)this->buffer);

    this->buffer = NULL;
    this->size = 0;
}

void buffer_zero(buffer_t* this) {
    memset(this->buffer, 0, this->size);
}

void buffer_append_string_slice(buffer_t* this, string_slice_t str) {
    size_t old_length = this->size;
    this->size = this->size + str.length;
    this->buffer = (uint8_t*)realloc((void*)this->buffer, this->size);

    // memcpy time!!1
    memcpy(this->buffer + old_length, str.string_ptr, str.length);
}

void buffer_move(buffer_t* newloc, buffer_t* oldloc) {
    newloc->buffer = oldloc->buffer;
    oldloc->buffer = NULL;

    newloc->size = oldloc->size;
    oldloc->size = 0;
}

void buffer_concat(buffer_t* this, const buffer_t* buffer) {
    size_t old_length = this->size;
    this->size = this->size + buffer->size;
    this->buffer = (uint8_t*)realloc((void*)this->buffer, this->size);

    // memcpy time!!1
    memcpy(this->buffer + old_length, buffer->buffer, buffer->size);
}