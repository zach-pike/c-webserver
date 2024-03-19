#include "stringlist.h"

#include <string.h>
#include <stdio.h>

#define STRINGS_REALLOC_SIZE 32 // Number of bytes to extend strings list on realloc
#define STRING_SIZES_REALLOC_SIZE 4 // Number of elements to extend string lengths list by

void string_list_initialize(string_list_t* this) {
    // Preallocate 64 bytes to prevent for constant reallocing
    this->strings_buffer_size = 64;
    this->strings_length = 0;

    this->strings = (char*)malloc(this->strings_buffer_size);

    // Preallocate 5 item list
    this->string_sizes_max_length = 5;
    this->string_sizes_length = 0;

    this->string_sizes = (size_t*)malloc(this->string_sizes_max_length * sizeof(size_t));
}

void string_list_destroy(string_list_t* this) {
    if (this->strings == NULL || this->string_sizes == NULL) return;

    free((void*)this->strings);
    free((void*)this->string_sizes);

    this->strings = NULL;
    this->string_sizes = NULL;

    this->strings_buffer_size = 0;
    this->strings_length = 0;

    this->string_sizes_max_length = 0;
    this->string_sizes_length = 0;
}

void _string_list_realloc_strings(string_list_t* this) {
    this->strings_buffer_size += STRINGS_REALLOC_SIZE;
    this->strings = (char*)realloc((void*)this->strings, this->strings_buffer_size);
}

void _string_list_realloc_string_sizes(string_list_t* this) {
    this->string_sizes_max_length += STRING_SIZES_REALLOC_SIZE;
    this->string_sizes = (size_t*)realloc((void*)this->string_sizes, this->string_sizes_max_length * sizeof(size_t));
}

size_t string_list_add(string_list_t* this, const string_slice_t* value) {
    //See if we need to realloc to bring string in
    while (value->length > (this->strings_buffer_size - this->strings_length))
        _string_list_realloc_strings(this);

    // Memcpy the string into the array
    memcpy(this->strings + this->strings_length, value->string_ptr, value->length);
    this->strings_length += value->length; // Update the total length of the string list

    // See if we need to realloc to bring in the length number
    if (this->string_sizes_length == this->string_sizes_max_length)
        _string_list_realloc_string_sizes(this);

    // Place length into list
    this->string_sizes[this->string_sizes_length] = value->length;
    this->string_sizes_length += 1; // Add 1 to length

    return this->string_sizes_length - 1;
}

void string_list_get(const string_list_t* this, string_slice_t* val, size_t index) {
    // Get beginning pointer to text desired
    // to do that, sum up all the lengths of all strings before
    size_t strings_start_index = 0;
    for (size_t i=0; i<index; i++)
        strings_start_index += this->string_sizes[i];
    
    // get length of string
    size_t length = this->string_sizes[index];

    string_slice_create(val, this->strings + strings_start_index, length);
}

void string_list_delete(string_list_t* this, size_t index) {
    // Get beginning pointer to text desired to remove
    // to do that, sum up all the lengths of all strings before
    size_t string_start_idx = 0;
    for (size_t i=0; i<index; i++)
        string_start_idx += this->string_sizes[i];

    size_t string_length = this->string_sizes[index];

    if (index < (this->string_sizes_length - 1)) {
        // Shift all strings to right over to where is being deleted
        memcpy(this->strings + string_start_idx, this->strings + string_start_idx + string_length, this->strings_length - (string_start_idx + string_length));

        memcpy(&this->string_sizes[index], &this->string_sizes[index + 1], (this->string_sizes_length - index - 1) * sizeof(size_t));
    }

    // Resize both arrays
    this->strings = (char*)realloc((void*)this->strings, this->strings_buffer_size - string_length);
    this->strings_buffer_size -= string_length;

    this->string_sizes_max_length -= 1;
    this->string_sizes_length -= 1;
    this->string_sizes = (size_t*)realloc((void*)this->string_sizes, this->string_sizes_max_length * sizeof(size_t));
    
}

size_t string_list_find(const string_list_t* this, string_slice_t needle) {
    size_t current_index = 0;

    for (size_t i=0; i<this->string_sizes_length; i++) {
        if (this->string_sizes[i] != needle.length) continue;

        // printf("shoud br 1ce\n");

        if (memcmp(this->strings + current_index, needle.string_ptr, needle.length) == 0) return i;

        current_index += this->string_sizes[i];
    }

    return -1;
}

size_t string_list_get_length(const string_list_t* this) {
    return this->string_sizes_length;
}

