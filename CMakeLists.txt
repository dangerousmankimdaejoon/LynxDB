cmake_minimum_required(VERSION 3.10)
project(lynx)

set(CMAKE_C_STANDARD 99)

include_directories(include)
add_subdirectory(src)

add_executable(lynx src/main.c)
add_executable(xsql src/client.c)