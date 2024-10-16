#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * substring - common function
 * @str: string char pointer
 * @start: start index
 * @length: result length
 * 
 * substring function - If length is -1, the length up to the end is automatically calculated 
 */
char* substring(const char* str, int start, int length) {
    if (start < 0 || start >= strlen(str) || length < -1) {
        return NULL;
    }
    
    if (length == -1) {
        length = strlen(str) - start;
    }

    int actual_length = strlen(str) - start < length ? strlen(str) - start : length;
    if (actual_length <= 0) {
        return NULL;
    }

    char* result = (char*)malloc(actual_length + 1);

    strncpy(result, str + start, actual_length);
    result[actual_length] = '\0';

    return result;
}