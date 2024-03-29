#include "client.h"

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include "allocatedstring/allocatedstring.h"
#include "httptypes/httptypes.h"
#include "handle/handle.h"

void client_initialize(client_t* this, int socket, struct sockaddr_in address) {
    this->socket = socket;
    this->address = address;
    this->in_buffer_bytes_read = 0;

    buffer_initialize(&this->in_buffer, 2048);
    buffer_zero(&this->in_buffer);
}

void client_destroy(client_t* this) {
    close(this->socket);

    buffer_destroy(&this->in_buffer);
}

void client_move(client_t* newloc, client_t* oldloc) {
    newloc->socket = oldloc->socket;
    oldloc->socket = 0;

    newloc->address = oldloc->address;
    memset(&oldloc->address, 0, sizeof(struct sockaddr_in));

    buffer_move(&newloc->in_buffer, &oldloc->in_buffer);

    newloc->in_buffer_bytes_read = oldloc->in_buffer_bytes_read;
    oldloc->in_buffer_bytes_read = 0;
}

void client_tick(client_t* this) {
    // Try and read data
    ssize_t read_bytes = read(this->socket, this->in_buffer.buffer + this->in_buffer_bytes_read, this->in_buffer.size - this->in_buffer_bytes_read);
    if (read_bytes > 0) this->in_buffer_bytes_read += read_bytes;

    // If theres a finished request then process it
    char* terminator_ptr = strstr(this->in_buffer.buffer, "\r\n\r\n");

    // If full http request has arrived
    if (terminator_ptr != NULL) {
        // Convert request into owned string excluding the double CRLF terminator
        string_slice_t request_text;
        size_t request_text_length = (uint8_t*)terminator_ptr - this->in_buffer.buffer;
        string_slice_create(&request_text, this->in_buffer.buffer, request_text_length);

        // Parse request
        http_request_t http_request;
        http_request_from_text(&http_request, &request_text);

        // Read body into buffer_t in request object now that content-length has been read
        if (headers_exists_c_str(&http_request.headers, "content-length")) {
            string_slice_t content_length_slice;
            headers_get_by_c_str(&http_request.headers, "content-length", &content_length_slice);

            // If we have a content length then we have more data to read
            // read length and create a buffer
            size_t body_length = string_slice_parse_size_t(&content_length_slice);

            buffer_t body_buffer;
            buffer_initialize(&body_buffer, body_length);

            // Copy over any read body data into new buffer
            size_t body_read_length = this->in_buffer_bytes_read - (request_text_length + 4);
            memcpy(body_buffer.buffer, terminator_ptr + 4, body_read_length);

            // Read rest of data into new buffer
            while (body_read_length < body_length) {
                size_t b = read(this->socket, body_buffer.buffer + body_read_length, body_length - body_read_length);
                if (b > 0) body_read_length += b;
            }

            // attach buffer to request object
            buffer_move(&http_request.request_body, &body_buffer);
        } else {
            // If buffer isnt used then mark as uninitialized
            buffer_mark_uninitialized(&http_request.request_body);
        }

        // Route Request
        http_response_t http_response;
        handle_response(&http_request, &http_response);

        // Set server headers

        // Set content-length header if buffer not uninitialized
        if (!buffer_is_uninitialized(&http_response.response_body)) {
            string_slice_t content_length_sl;
            string_slice_from_c_str(&content_length_sl, "content-length");

            // convert length to string
            string_slice_t content_length_val_sl;
            char content_len_str[8] = { 0 };
            size_t content_len_str_len = sprintf(content_len_str, "%d", http_response.response_body.size);
            string_slice_create(&content_length_val_sl, content_len_str, content_len_str_len);

            // Add content length header
            headers_add(&http_response.headers, &content_length_sl, &content_length_val_sl);
        }

        // Convert http_response_t to buffer_t
        buffer_t response_buffer;
        http_response_to_buffer(&http_response, &response_buffer);

        // Write buffer to client
        write(this->socket, response_buffer.buffer, response_buffer.size);

        // Cleanup
        buffer_destroy(&response_buffer);
        http_response_destroy(&http_response);
        http_request_destroy(&http_request);
        
        // Reset buffer
        buffer_zero(&this->in_buffer);
        this->in_buffer_bytes_read = 0;
    }
}