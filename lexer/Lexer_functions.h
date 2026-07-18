#include "Token.h"

#define INCREMENT_COUNT(token_array) (token_array->count = (token_array->count + 1))

/**
 * @brief Create a TokenArray object
 * 
 * @param capacity Capacity of the TokenArray object
 * @return A TokenArray object with an empty array of size `capacity` and `count` starting at 0
 * @retval - TokenArray : If the allocation did well
 * @retval - NULL : If TokenType array or TokenArray allocation failed, or `capacity` <= 0
 * @warning Need to free it with `TokenArray_free` function
 */
TokenArray* TokenArray_create(int capacity);

/**
 * @brief Free a TokenArray object
 * 
 * @param token_array The TokenArray
 * @return A boolean about the result
 * @retval - true : If well freed
 * @retval - false : If token_array == NULL
 */
bool TokenArray_free(TokenArray* token_array);

/**
 * @brief Get the token array inside the TokenArray object
 * 
 * @param token_array The TokenArray object
 * @return The token array 
 * @retval TOKEN_ERREUR : If token_array == NULL
 */
TokenType* TokenArray_getTokens(TokenArray* token_array);

/**
 * @brief Get the capacity of the token array
 * 
 * @param token_array The TokenArray object
 * @return The capacity
 * @retval -1 : If token_array == NULL
 */
int TokenArray_getCapacity(TokenArray* token_array);

/**
 * @brief Get the count of the token array
 * 
 * @param token_array The TokenArray object
 * @return The count 
 * @retval -1 : If token_array == NULL
 */
int TokenArray_getCount(TokenArray* token_array);

/**
 * @brief Increase the count of the token array
 * 
 * @param token_array The TokenArray object
 * @return A boolean about the result of the operation
 * @retval - true : If the count had been increased
 * @retval - false : If token_array == NULL
 */
bool TokenArray_increaseCount(TokenArray* token_array);

/**
 * @brief Increase the capacity of the toke array by 256 TokenType
 * 
 * @param token_array The TokenArray object
 * @return A boolean about the result of the operation
 * @retval - true : If the token array had been increased
 * @retval - false : If token_array == NULL or the reallocation failed
 */
bool TokenArray_increaseCapacity(TokenArray* token_array);

/**
 * @brief Add a token at the end of the token array
 * 
 * @param token_array The TokenArray object
 * @param token The new TypeToken to add
 * @return A boolean about the result of the operation
 * @retval - true : If the token is well added
 * @retval - false : If token_array == NULL or the capacity increase failed
 */
bool TokenArray_addToken(TokenArray* token_array, TokenType token);

/**
 * @brief Get the token at the index given
 * 
 * @param token_array The TokenArray object
 * @param index The index at which the token to get is
 * @return The token to retrieve at index
 * @retval TOKEN_ERREUR : If `index` >= token_array->count or token_array == NULL
 */
TokenType TokenArray_getTokenAtIndex(TokenArray* token_array, int index);