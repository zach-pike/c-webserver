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

void client_tick(client_t* this, webserver_t* ws) {
    // Try and read data
    ssize_t read_bytes = read(this->socket, this->in_buffer.buffer + this->in_buffer_bytes_read, this->in_buffer.size - this->in_buffer_bytes_read);
    if (read_bytes > 0) this->in_buffer_bytes_read += read_bytes;

    // If theres a finished request then process it
    char* terminator_ptr = strstr(this->in_buffer.buffer, "\r\n\r\n");

    if (terminator_ptr != NULL) {
        // Convert request into owned string excluding the double CRLF terminator
        string_slice_t request_text;
        size_t request_text_length = (uint8_t*)terminator_ptr - this->in_buffer.buffer;
        string_slice_create(&request_text, this->in_buffer.buffer, request_text_length);

        // Parse request
        http_request_t http_request;
        http_request_from_text(&http_request, &request_text);

        // Route Request
        http_response_t http_response;
        handle_response(&http_request, &http_response);

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