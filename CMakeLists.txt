cmake_minimum_required(VERSION 3.10)
project(lynx)

set(CMAKE_C_STANDARD 99)

include_directories(include)
add_subdirectory(src)

<<<<<<< HEAD
add_executable(lynx src/main.c src/common.c)
add_executable(xsql src/client.c src/common.c)
=======
add_executable(lynx src/main.c)
add_executable(xsql src/client.c)
>>>>>>> ddd2b897fd389935dc4b026d87101d04b9c3c7a3
