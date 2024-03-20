#pragma once

#include <stddef.h>

typedef struct string_slice {
    const char* string_ptr;
    size_t length;
} string_slice_t;

// Does not own memory so does not need to be destroyed
void string_slice_create(string_slice_t* this, const char* data, size_t length);

// Print string slice (encases in quotes)
void string_slice_println(const string_slice_t* this);

// Create a string slice from a C string
void string_slice_from_c_str(string_slice_t* this, const char* str);

size_t string_slice_parse_size_t(const string_slice_t* this);

// Whitespace trimming functions
void string_slice_ltrim(string_slice_t* this);
void string_slice_rtrim(string_slice_t* this);
void string_slice_trim(string_slice_t* this);
