#pragma once

#include <stdint.h>

#include "stringslice/stringslice.h"

typedef struct buffer {
    uint8_t* buffer;
    uint32_t size;
} buffer_t;

void buffer_initialize(buffer_t* this, uint32_t size);
void buffer_destroy(buffer_t* this);

void buffer_zero(buffer_t* this);
void buffer_append_string_slice(buffer_t* this, string_slice_t str);
void buffer_move(buffer_t* newloc, buffer_t* oldloc);

// Does not destroy buffer
void buffer_concat(buffer_t* this, const buffer_t* buffer);