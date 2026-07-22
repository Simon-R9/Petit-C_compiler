#ifndef HELPER
#define HELPER

#include <stdlib.h>

// <========================================================================>
// <========================== Helper functions ============================>
// <========================================================================>

/**
 * @brief Calculate the size of a string
 * 
 * @return The size of the string
 * @retval -1 : If `str` == NULL
 */
int string_size(char* str);

/**
 * @brief Copy a string
 * 
 * @return A pointer to the new string
 * @retval NULL : If `str` == NULL or the allocation failed
 * @warning The function don't free the original string
 * @warning The new string needs to be freed after use
 */
char* string_copy(char* str);

// <========================================================================>
// <========================================================================>
// <========================================================================>

#endif
