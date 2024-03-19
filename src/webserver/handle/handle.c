#include "handle.h"

#include <string.h>

#include <stdio.h>

static const char* DATA = "<p>Hello World</p><h1>aa</h1>";

void handle_response(const http_request_t* request, http_response_t* response) {
    allocated_string_t message;
    allocated_string_from_ptr(&message, "OK", 2);

    allocated_string_t main_content;
    allocated_string_from_ptr(&main_content, DATA, strlen(DATA));

    // Create the response
    http_response_initialize(response, 200, &message, &main_content.string_buffer);

    // Set headers
    string_slice_t content_length_sl;
    string_slice_from_static_str(&content_length_sl, "content-length");

    // convert length to string
    string_slice_t content_length_val_sl;
    char content_len_str[5] = { 0 };
    size_t content_len_str_len = sprintf(content_len_str, "%d", response->response_body.size);
    string_slice_create(&content_length_val_sl, content_len_str, content_len_str_len);

    // Add content length header
    headers_add_header(&response->headers, &content_length_sl, &content_length_val_sl);

    // Add content type header
    string_slice_t content_type_sl;
    string_slice_t content_type_val_sl;
    string_slice_from_static_str(&content_type_sl, "content-type");
    string_slice_from_static_str(&content_type_val_sl, "text/html");
    headers_add_header(&response->headers, &content_type_sl, &content_type_val_sl);
}