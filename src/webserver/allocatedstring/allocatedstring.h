#pragma once

#include "../buffer/buffer.h"
#include <stddef.h>

typedef struct allocated_string {
    buffer_t string_buffer;
} allocated_string_t;

void allocated_string_initialize(allocated_string_t* this, const char* str);
void allocated_string_from_str(allocated_string_t* this, const char* str);
void allocated_string_from_ptr(allocated_string_t* this, const char* ptr, size_t length);

void allocated_string_destroy(allocated_string_t* this);

size_t allocated_string_length(allocated_string_t* this);

void allocated_string_move(allocated_string_t* newloc, allocated_string_t* oldloc);