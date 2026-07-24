/**
 * @file DynamicString.h
 * @brief Declaration of the functions about a dynamic string that can grow if needed
 * @author Rosol Simon
 * @date 2026-07-19
 */

#ifndef DYNAMIC_STRING
#define DYNAMIC_STRING

#include <stdlib.h>
#include <stdbool.h>

#define DYNAMIC_STRING_CAPACITY_BASE 32

typedef struct _DynamicString {
    char* string;
    int capacity;
    int count;
} DynamicString;

// <========================================================================>
// <======================== DynamicString functions =======================>
// <========================================================================>

/**
 * @defgroup DynamicString
 * @brief Dynamic strings to handle the size unknown identifiers and string for the Lexer
 * @{
 */

DynamicString* DynamicString_create();

bool DynamicString_free(DynamicString* dynamic_string);

char* DynamicString_getString(DynamicString* dynamic_string);

int DynamicString_getCapacity(DynamicString* dynamic_string);

int DynamicString_getCount(DynamicString* dynamic_string);

bool DynamicString_addChar(DynamicString* dynamic_string, char new_char);

char DynamicString_getCharAtIndex(DynamicString* dynamic_string, int index);

DynamicString* DynamicString_recycleString(DynamicString* dynamic_string);

/** @}*/
// <========================================================================>
// <========================================================================>
// <========================================================================>

#endif
