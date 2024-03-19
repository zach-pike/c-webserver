#include "handle.h"

#include <string.h>

#include <stdio.h>

static const char* DATA = "<p>Hello World</p><h1>aa</h1>";

void handle_response(const http_request_t* request, http_response_t* response) {
    // response code
    allocated_string_t response_message;
    allocated_string_from_str(&response_message, "OK");

    // Read user agent
    string_slice_t ua;
    headers_get_header_by_static_str(&request->headers, "user-agent", &ua);

    // response data
    allocated_string_t main_content;
    allocated_string_from_ptr(&main_content, ua.string_ptr, ua.length);

    // Create the response object (takes ownership of response
    http_response_initialize(response, 200, &response_message, &main_content.string_buffer);

    // Set headers
    // Add content type header
    headers_add_header_static_str(&response->headers, "content-type", "text/html");
}