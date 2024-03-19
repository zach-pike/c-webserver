#include "allocatedstring.h"

#include <string.h>

void allocated_string_initialize(allocated_string_t* this, const char* str) {
    size_t strl = strlen(str);
    buffer_initialize(&this->string_buffer, strl);
    memcpy(this->string_buffer.buffer, str, strl);
}

void allocated_string_from_str(allocated_string_t* this, const char* str) {
    allocated_string_from_ptr(this, str, strlen(str));
}

void allocated_string_from_ptr(allocated_string_t* this, const char* ptr, size_t length) {
    buffer_initialize(&this->string_buffer, length);
    memcpy(this->string_buffer.buffer, ptr, length);
}

void allocated_string_destroy(allocated_string_t* this) {
    buffer_destroy(&this->string_buffer);
}

void allocated_string_move(allocated_string_t* newloc, allocated_string_t* oldloc) {
    buffer_move(&newloc->string_buffer, &oldloc->string_buffer);
}