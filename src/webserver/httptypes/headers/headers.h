#pragma once

#include <stdbool.h>

#include "buffer/buffer.h"
#include "stringlist/stringlist.h"
#include "stringslice/stringslice.h"

typedef struct headers {
    string_list_t header_keys;
    string_list_t header_values;
} headers_t;

void headers_initialize(headers_t* this);
void headers_destroy(headers_t* this);

void headers_initialize_from_text(headers_t* this, string_slice_t text);

void headers_add_header(headers_t* this, const string_slice_t* key, const string_slice_t* value);
void headers_add_header_static_str(headers_t* this, const char* key, const char* value);

void headers_delete_header(headers_t* this, string_slice_t key);
void headers_get_header(const headers_t* this, string_slice_t key, string_slice_t* value);

void headers_get_by_index(const headers_t* this, size_t idx, string_slice_t* key, string_slice_t* value);

bool headers_has_header(const headers_t* this, string_slice_t key);


void headers_to_buffer(const headers_t* this, buffer_t* buffer);