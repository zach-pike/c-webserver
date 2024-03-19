#include "stringslice.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void string_slice_create(string_slice_t* this, const char* data, size_t length) {
    this->string_ptr = data;
    this->length = length;
}

void string_slice_println(const string_slice_t* this) {
    putchar('"');
    for (size_t i=0; i<this->length; i++) {
        putchar(this->string_ptr[i]);
    }
    putchar('"');
    putchar('\n');
}

void string_slice_from_c_str(string_slice_t* this, const char* str) {
    this->string_ptr = str;
    this->length = strlen(str);
}

void string_slice_ltrim(string_slice_t* this) {
    size_t i;
    for(i=0; i<this->length; i++) {
        // Find first non whitespace character
        if (this->string_ptr[i] > 32) break;
    }
    this->string_ptr += i;
    this->length -= i;
}

void string_slice_rtrim(string_slice_t* this) {
    size_t i;
    for (i=this->length-1; i>=0; i--) {
        if (this->string_ptr[i] > 32) break;
    }

    this->length = i + 1;
}

void string_slice_trim(string_slice_t* this) {
    string_slice_ltrim(this);
    string_slice_rtrim(this);
}
