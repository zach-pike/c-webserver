cmake_minimum_required(VERSION 3.22.1)

project(WebServer)

include_directories(src/webserver)

file(GLOB_RECURSE WEBSERVER_SRC src/webserver/*.c)

add_executable(main src/main.c ${WEBSERVER_SRC})
add_executable(testing src/testing.c ${WEBSERVER_SRC})