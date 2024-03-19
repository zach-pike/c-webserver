#pragma once

#include <stddef.h>

typedef struct string_slice {
    const char* string_ptr;
    size_t length;
} string_slice_t;

// Does not own memory so does not need to be destroyed
void string_slice_create(string_slice_t* this, const char* data, size_t length);

void string_slice_println(const string_slice_t* this);

void string_slice_from_static_str(string_slice_t* this, const char* str);

void string_slice_ltrim(string_slice_t* this);
void string_slice_rtrim(string_slice_t* this);
void string_slice_trim(string_slice_t* this);
