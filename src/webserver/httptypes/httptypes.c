#include "httptypes.h"

#include <stdio.h>

#include "utils/strstrwithlength.h"
#include "utils/strcmpnocase.h"

void http_request_from_text(http_request_t* this, const string_slice_t* text) {
    // Parse request line

    // Find first CRLF
    const char* crlf = strstr_with_length(text->string_ptr, "\r\n", text->length);
    bool hasHeaders = true;

    if (crlf == NULL) { 
        crlf = text->string_ptr + text->length;
        hasHeaders = false;
    }

    // Length of request line
    size_t line_length = crlf - text->string_ptr;

    // Get request verb
    string_slice_t request_verb;
    const char* verb_end = strstr_with_length(text->string_ptr, " ", line_length);
    size_t verb_length = verb_end - text->string_ptr;
    string_slice_create(&request_verb, text->string_ptr, verb_length);

    // get path
    string_slice_t request_path;
    const char* path_end = strstr_with_length(verb_end + 1, " ", line_length - verb_length - 1);
    size_t path_length = path_end - verb_end - 1;
    string_slice_create(&request_path, verb_end + 1, path_length);

    // http version
    string_slice_t request_version;
    size_t version_length = line_length - path_length - verb_length - 2;
    string_slice_create(&request_version, path_end + 1, version_length);

    // headers
    string_slice_t header_slice;
    string_slice_create(&header_slice, crlf + 2, text->length - (line_length + 2));

    // parse verb
    if (strcmp_ignore_case(request_verb, "get")) {
        this->verb = get;
    } else if (strcmp_ignore_case(request_verb, "post")) {
        this->verb = post;
    }

    // request path
    this->path = request_path;

    if(strcmp_ignore_case(request_version, "HTTP/1.1")) {
        this->http_version = http11;
    } else if (strcmp_ignore_case(request_version, "HTTP/2")) {
        this->http_version = http2;
    }

    // Parse headers
    if (hasHeaders)
        headers_initialize_from_text(&this->headers, header_slice);
    else
        headers_initialize(&this->headers);
}

void http_request_destroy(http_request_t* this) {
    headers_destroy(&this->headers);
    buffer_destroy(&this->request_body);
}

void http_response_initialize(http_response_t* this, uint16_t code, allocated_string_t* message, buffer_t* body) {
    this->code = code;
    allocated_string_move(&this->message, message);
    headers_initialize(&this->headers);
    buffer_move(&this->response_body, body);
}

void http_response_destroy(http_response_t* this) {
    allocated_string_destroy(&this->message);
    headers_destroy(&this->headers);
    buffer_destroy(&this->response_body);
}

// Create buffer from http response
void http_response_to_buffer(const http_response_t* this, buffer_t* buffer) {
    const char* fmt = "HTTP/1.1 %d %s\r\n";

    size_t response_line_length = snprintf(NULL, 0, fmt, this->code, this->message.string_buffer.buffer);

    // Create initial the buffer
    buffer_t request_line_buffer;
    buffer_initialize(&request_line_buffer, response_line_length + 1);

    // Write into buffer
    sprintf(request_line_buffer.buffer, fmt, this->code, this->message.string_buffer.buffer);

    // Shrink buffer by 1 byte to get rid of null
    // Hacky fix unnessary realloc
    buffer_realloc(&request_line_buffer, request_line_buffer.size - 1);

    // Move request line to buffer
    buffer_move(buffer, &request_line_buffer);
    // insert headers

    // Stringified headers
    buffer_t headers_buf;
    headers_to_buffer(&this->headers, &headers_buf);

    // Concat with request line
    buffer_concat(buffer, &headers_buf);

    buffer_destroy(&headers_buf);

    // attach crlf as headers code ends with
    // inserting a crlf so we only need to add 1 more

    string_slice_t crlf_ss;
    string_slice_from_c_str(&crlf_ss, "\r\n");

    // Concat with response data
    buffer_append_string_slice(buffer, crlf_ss);

    //insert body data if is initialised
    if (!buffer_is_uninitialized(&this->response_body))
        buffer_concat(buffer, &this->response_body);
}