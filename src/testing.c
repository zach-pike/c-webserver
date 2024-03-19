#include <stdlib.h>
#include <stdio.h>

#include "httptypes/headers/headers.h"

#include "stringlist/stringlist.h"

int main(int argc, char** argv) {
    // string_list_t list;
    // string_list_initialize(&list);

    // string_slice_t sslice1;
    // string_slice_t sslice2;
    // string_slice_t sslice3;
    // string_slice_t sslice4;
    // string_slice_t sslice5;
    // string_slice_t sslice6;
    // string_slice_from_static_str(&sslice1, "abcd");
    // string_slice_from_static_str(&sslice2, "efgh");
    // string_slice_from_static_str(&sslice3, "ijkl");
    // string_slice_from_static_str(&sslice4, "mnop");
    // string_slice_from_static_str(&sslice5, "qrst");
    // string_slice_from_static_str(&sslice6, "uvwx");

    // string_list_add(&list, &sslice1);
    // string_list_add(&list, &sslice2);
    // string_list_add(&list, &sslice3);
    // string_list_add(&list, &sslice4);
    // string_list_add(&list, &sslice5);
    // string_list_add(&list, &sslice6);

    // // Get index of "ijkl"
    // size_t idx = string_list_find(&list, sslice3);
    // printf("%ld\n", idx);

    // // Remove the element
    // string_list_delete(&list, idx);

    // for (size_t i=0; i<string_list_get_length(&list); i++) {
    //     string_slice_t a;
    //     string_list_get(&list, &a, i);
    //     string_slice_println(&a);
    // }

    // string_list_destroy(&list);

    const char* header_data = "  poppy-cock  :                   \b\r     hello world\r\n\tpoopy-dick: penis\r\naaaah :dick\t  ";
    string_slice_t header_slice;
    string_slice_from_static_str(&header_slice, header_data);


    headers_t headers;
    headers_initialize_from_text(&headers, header_slice);


    headers_destroy(&headers);

    return 0;
}