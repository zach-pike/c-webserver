#pragma once

#include "stringslice/stringslice.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct string_list {
    char* strings;
    size_t strings_length;     // Number of items in list
    size_t strings_buffer_size; // Size of buffer

    size_t* string_sizes;
    size_t string_sizes_length;      // Number of items in list
    size_t string_sizes_max_length;
} string_list_t;

void string_list_initialize(string_list_t* this);
void string_list_destroy(string_list_t* this);
void _string_list_realloc_strings(string_list_t* this);
void _string_list_realloc_string_sizes(string_list_t* this);

// Returns index of item
size_t string_list_add(string_list_t* this, const string_slice_t* value);

void string_list_delete(string_list_t* this, size_t index);
void string_list_get(const string_list_t* this, string_slice_t* val, size_t index);

// Returns -1 on not found
size_t string_list_find(const string_list_t* this, string_slice_t needle);

size_t string_list_get_length(const string_list_t* this);


