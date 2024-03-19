#include "headers.h"

#include "utils/strcmpgood.h"

#include <stdbool.h>
#include <string.h>

void headers_initialize(headers_t* this) {
    string_list_initialize(&this->header_keys);
    string_list_initialize(&this->header_values);
}
void headers_destroy(headers_t* this) {
    string_list_destroy(&this->header_keys);
    string_list_destroy(&this->header_values);
}

void headers_initialize_from_text(headers_t* this, string_slice_t text) {
    headers_initialize(this);

    const char* read_ptr = text.string_ptr;

    bool finished = false;

    while (true) {
        const char* crlf = strcmpgood(read_ptr, "\r\n", text.length - (read_ptr - text.string_ptr));

        size_t line_length = crlf - read_ptr;

        if (crlf == NULL) {
            crlf = text.string_ptr + text.length;
            finished = true;
        }

        // Find :

        const char* colon = strcmpgood(read_ptr, ":", line_length);

        size_t key_length = colon - read_ptr;
        // printf("%c\n", *colon);

        size_t value_length = crlf - colon - 1;

        string_slice_t key_slice;
        string_slice_t value_slice;
        
        // Gets everything from start of line to colon
        string_slice_create(&key_slice, read_ptr, key_length);

        // After colon to end of line
        string_slice_create(&value_slice, colon+1, value_length);

        string_slice_trim(&key_slice);
        string_slice_trim(&value_slice);
        
        headers_add_header(this, &key_slice, &value_slice);

        read_ptr = crlf + 2;

        if (finished) break;
    }
}

void headers_add_header(headers_t* this, const string_slice_t* key, const string_slice_t* value) {
    string_list_add(&this->header_keys, key);
    string_list_add(&this->header_values, value);
}

void headers_add_header_static_str(headers_t* this, const char* key, const char* value) {
    string_slice_t key_slice;
    string_slice_t value_slice;
    string_slice_from_static_str(&key_slice, key);
    string_slice_from_static_str(&value_slice, value);
    headers_add_header(this, &key_slice, &value_slice);
}

void headers_delete_header(headers_t* this, string_slice_t key) {
    size_t idx = string_list_find(&this->header_keys, key, true);

    if (idx != -1) {
        string_list_delete(&this->header_keys, idx);
        string_list_delete(&this->header_values, idx);
    }
}

void headers_get_header(const headers_t* this, string_slice_t key, string_slice_t* value) {
    size_t idx = string_list_find(&this->header_keys, key, true);

    if (idx != -1)
        string_list_get(&this->header_values, value, idx);
}

void headers_get_by_index(const headers_t* this, size_t idx, string_slice_t* key, string_slice_t* value) {
    string_list_get(&this->header_keys, key, idx);
    string_list_get(&this->header_values, value, idx);
}

bool headers_has_header(const headers_t* this, string_slice_t key) {
    size_t idx = string_list_find(&this->header_keys, key, true);
    return idx != -1;
}


void headers_to_buffer(const headers_t* this, buffer_t* buffer) {
    // HEADERS
    size_t headers_length = 0;

    // Calculate string length
    for (size_t i=0; i<string_list_get_length(&this->header_keys); i++) {
        string_slice_t key;
        string_slice_t value;

        headers_get_by_index(this, i, &key, &value);

        // key.length represents the key strings length
        // 2 is ": "
        // value.length represents the value strings length
        // 2 represents CRLF
        size_t row_size = key.length + 2 + value.length + 2;

        headers_length += row_size;
    }

    buffer_initialize(buffer, headers_length);

    // start writing it!
    size_t char_idx = 0;
    for (size_t i=0; i<string_list_get_length(&this->header_keys); i++) {
        string_slice_t key;
        string_slice_t value;

        headers_get_by_index(this, i, &key, &value);

        // kms
        memcpy(buffer->buffer + char_idx, key.string_ptr, key.length);
        memcpy(buffer->buffer + char_idx + key.length, ": ", 2);
        memcpy(buffer->buffer + char_idx + key.length + 2, value.string_ptr, value.length);
        memcpy(buffer->buffer + char_idx + key.length + 2 + value.length, "\r\n", 2);

        char_idx += key.length + 2 + value.length + 2;
    }
}
