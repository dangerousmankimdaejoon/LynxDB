cmake_minimum_required(VERSION 3.10)
project(lynx)

set(CMAKE_C_STANDARD 99)

# Bison and Flex output directories
set(BISON_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(FLEX_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)

file(MAKE_DIRECTORY ${BISON_OUTPUT_DIR})
file(MAKE_DIRECTORY ${FLEX_OUTPUT_DIR})

# Bison file process
add_custom_command(
    OUTPUT ${BISON_OUTPUT_DIR}/standard.tab.c ${BISON_OUTPUT_DIR}/standard.tab.h
    COMMAND bison -d -o ${BISON_OUTPUT_DIR}/standard.tab.c ../src/parser/standard.y
    DEPENDS ../src/parser/standard.y
    COMMENT "Generating Bison parser"
)

# Flex file process
add_custom_command(
    OUTPUT ${FLEX_OUTPUT_DIR}/lex.yy.c
    COMMAND flex -o ${FLEX_OUTPUT_DIR}/lex.yy.c ../src/parser/standard.l
    DEPENDS ../src/parser/standard.l
    COMMENT "Generating Flex lexer"
)

# Create an executable
add_executable (
    lynx
    src/main.c 
    src/common.c 
    ${BISON_OUTPUT_DIR}/standard.tab.c 
    ${FLEX_OUTPUT_DIR}/lex.yy.c
)

add_executable(
    xsql
    src/client.c
    src/common.c
)

add_dependencies(lynx ${BISON_OUTPUT_DIR}/standard.tab.c ${FLEX_OUTPUT_DIR}/lex.yy.c)
add_dependencies(xsql ${BISON_OUTPUT_DIR}/standard.tab.c ${FLEX_OUTPUT_DIR}/lex.yy.c)

include_directories(include ${BISON_OUTPUT_DIR} ${FLEX_OUTPUT_DIR})
