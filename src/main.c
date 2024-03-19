#include <stdlib.h>
#include <stdio.h>

#include "webserver/webserver.h"

#include "stringlist/stringlist.h"

int main(int argc, char** argv) {
    webserver_t ws;
    webserver_initialize(&ws, "127.0.0.1", 8080);

    while (1) {
        webserver_tick(&ws);
    }

    webserver_destroy(&ws);

    return 0;
}