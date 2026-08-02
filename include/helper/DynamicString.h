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

/**
 * @brief Create a Dynamic String
 * 
 * @return The pointer to the newly created Dynamic String
 * @retval - NULL : If the allocation of the struct or the string failed
 */
DynamicString* DynamicString_create();

/**
 * @brief Free the Dynamic String
 * 
 * @param dynamic_string The Dynamic String to free
 * @return A boolean about the success or not of the operation
 * @retval - false : If `dynamic_string` == NULL
 */
bool DynamicString_free(DynamicString* dynamic_string);

/**
 * @brief Retrieve the string inside the Dynamic String
 * 
 * @param dynamic_string The Dynamic String
 * @return The string in the struct
 * @retval - NULL : If `dynamic_string` == NULL
 */
char* DynamicString_getString(DynamicString* dynamic_string);

/**
 * @brief Retrieve the actual capacity of the Dynamic String
 * 
 * @param dynamic_string The Dynamic String
 * @return The capacity of the struct
 * @retval - -1 : If `dynamic_string` == NULL
 */
int DynamicString_getCapacity(DynamicString* dynamic_string);

/**
 * @brief Retrieve the count in the string of the Dynamic String
 * 
 * @param dynamic_string The Dynamic String
 * @return The position in the string of the Dynamic String
 * @retval - -1 : If `dynamic_string` == NULL
 */
int DynamicString_getCount(DynamicString* dynamic_string);

/**
 * @brief Add a character to the Dynamic String
 * 
 * @param dynamic_string The Dynamic String in which to add the char
 * @param new_char The char to add
 * @return A boolean about the success or not of the operation
 * @retval - false : If `dynamic_string` == NULL, or the reallocation failed
 */
bool DynamicString_addChar(DynamicString* dynamic_string, char new_char);

/**
 * @brief Get the char at the given index
 * 
 * @param dynamic_string The Dynamic String
 * @param index The index of the char to get
 * @return The char at the given index
 * @retval - '\0' : If `dynamic_string` == NULL, the index is out of bond or the string in the struct is NULL
 */
char DynamicString_getCharAtIndex(DynamicString* dynamic_string, int index);

/**
 * @brief Free a Dynamic String and replace it by a new one
 * 
 * @param dynamic_string The Dynamic String to replace
 * @return The new Dynamic String
 * @warning The return value depends on a call of DynamicString_create function
 */
DynamicString* DynamicString_recycleString(DynamicString* dynamic_string);

/** @}*/
// <========================================================================>
// <========================================================================>
// <========================================================================>

#endif
