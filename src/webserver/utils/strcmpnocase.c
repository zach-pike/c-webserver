#include "strcmpnocase.h"

int strcmp_ignore_case(string_slice_t s1, const char* s2) {
    if (s1.length != strlen(s2)) return 0;

    for (size_t i=0; i<s1.length; i++) {
        char c1 = s1.string_ptr[i];
        char c2 = s2[i];

        if (c1 >= 'a' && c1 <= 'z') c1 -= 32;
        if (c2 >= 'a' && c2 <= 'z') c2 -= 32;

        if (c1 != c2) return 0;
    }

    return 1;
}