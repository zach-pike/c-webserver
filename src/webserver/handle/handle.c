#include "handle.h"

#include <string.h>

#include <stdio.h>

static const char* DATA = "<p>Hello World</p><h1>aa</h1>";

void handle_response(const http_request_t* request, http_response_t* response) {
    if (request->verb != post) {
        // response code
        allocated_string_t response_message;
        allocated_string_from_str(&response_message, "Error");

        buffer_t main_content;
        buffer_initialize(&main_content, 5);
        memcpy(main_content.buffer, "Error", 5);

        http_response_initialize(response, 400, &response_message, &main_content);
        return;
    }

    // response code
    allocated_string_t response_message;
    allocated_string_from_str(&response_message, "OK");

    // response data
    buffer_t main_content;
    buffer_initialize(&main_content, request->request_body.size);
    memcpy(main_content.buffer, request->request_body.buffer, request->request_body.size);

    // Create the response object (takes ownership of response
    http_response_initialize(response, 200, &response_message, &main_content);

    // Set headers
    // Add content type header
    headers_add_c_str(&response->headers, "content-type", "text/plain");
}