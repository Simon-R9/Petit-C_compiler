/**
 * @file string_functions.c
 * @brief Basic functions to manipulate strings
 * @author Rosol Simon
 * @date 2026-07-17
 */

#include "../../include/helper/string_functions.h"

// <========================================================================>
// <========================== Helper functions ============================>
// <========================================================================>

/**
 * @addtogroup string_functions
 */

int string_size(char* str) {
    if (str == NULL) return -1;
    int count = 0;
    while (str[count] != '\0') {
        count++;
    }
    return count;
}

char* string_copy(char* str) {
    if (str == NULL) return NULL;
    int content_size = string_size(str);
    char* new_content = (char*)malloc(sizeof(char)*(content_size + 1));

    if (new_content == NULL) return NULL;

    for (int index = 0; index < content_size; index++) {
        new_content[index] = str[index];
    }
    new_content[content_size] = '\0';
    return new_content;
}

/** @} */

// <========================================================================>
// <========================================================================>
// <========================================================================>