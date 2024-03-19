#pragma once

#include "stringslice/stringslice.h"
#include "headers/headers.h"
#include "allocatedstring/allocatedstring.h"
#include "buffer/buffer.h"

// Request type
typedef enum http_verb {
    get,
    post
} http_verb_t;

// http version
typedef enum http_version {
    http11,
    http2
} http_version_t;

// Request data struct
typedef struct http_request {
    // Request line data
    http_verb_t verb;
    string_slice_t path;
    http_version_t http_version;

    // Header data
    headers_t headers;

    // TBI...
    buffer_t request_body;
} http_request_t;

// Create http request object from message
void http_request_from_text(http_request_t* this, const string_slice_t* text);

void http_request_destroy(http_request_t* this);

typedef struct http_response {
    uint16_t code;
    allocated_string_t message;

    headers_t headers;

    buffer_t response_body;
} http_response_t;

// Allocated string gets moved into http response
void http_response_initialize(http_response_t* this, uint16_t code, allocated_string_t* message, buffer_t* body);

void http_response_destroy(http_response_t* this);

// Create buffer from http response
void http_response_to_buffer(const http_response_t* this, buffer_t* buffer);