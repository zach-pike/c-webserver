#include "strstrwithlength.h"

#include <string.h>

const char* strstr_with_length(const char* haystack, const char* needle, size_t length) {
    size_t needle_length = strlen(needle);

    for(size_t i=0; i<length-needle_length; i++) {
        if (memcmp(haystack+i, needle, needle_length) == 0) return haystack+i;
    }

    return NULL;
}