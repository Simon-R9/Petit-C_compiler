#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "Token.h"

#define INCREMENT_COUNT(token_array) (token_array->count = (token_array->count + 1))


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


// <========================================================================>
// <=========================== Token functions ============================>
// <========================================================================>

/**
 * @brief Create a new Token object
 * 
 * @param token_type Type of the new token
 * @param content Content of the new token
 * @return A new Token object
 * @retval NULL : If `content` == NULL or the allocation (the Token struct or the copy of `content`) failed
 * @warning `content` isn't freed by this function
 */
Token* Token_createToken(TokenType token_type, char* content);

/**
 * @brief Free a Token object
 */
void Token_free(Token* token);

/**
 * @brief Retrieve the TokenType of the Token
 * 
 * @return The TokenType of the Token
 * @retval -1 : If `token` == NULL
 */
TokenType Token_getTokenType(Token* token);

/**
 * @brief Retrieve the content of the Token
 * 
 * @return The pointer to the content
 * @retval NULL : If `token` == NULL
 */
char* Token_getContent(Token* token);

// <========================================================================>
// <========================================================================>
// <========================================================================>


// <========================================================================>
// <======================== TokenArray functions ==========================>
// <========================================================================>


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
 * @retval - false : If `token_array` == NULL
 */
bool TokenArray_free(TokenArray* token_array);

/**
 * @brief Get the token array inside the TokenArray object
 * 
 * @param token_array The TokenArray object
 * @return The token array 
 * @retval NULL : If `token_array` == NULL
 */
Token** TokenArray_getTokens(TokenArray* token_array);

/**
 * @brief Get the capacity of the token array
 * 
 * @param token_array The TokenArray object
 * @return The capacity
 * @retval -1 : If `token_array` == NULL
 */
int TokenArray_getCapacity(TokenArray* token_array);

/**
 * @brief Get the count of the token array
 * 
 * @param token_array The TokenArray object
 * @return The count 
 * @retval -1 : If `token_array` == NULL
 */
int TokenArray_getCount(TokenArray* token_array);

/**
 * @brief Increase the count of the token array
 * 
 * @param token_array The TokenArray object
 * @return A boolean about the result of the operation
 * @retval - true : If the count had been increased
 * @retval - false : If `token_array` == NULL
 * @warning If called without inserting a value at the former index, it will leave an allocated place never valued (so not accessible while freeing it)
 */
bool TokenArray_increaseCount(TokenArray* token_array);

/**
 * @brief Increase the capacity of the toke array by multiplying it by 2
 * 
 * @param token_array The TokenArray object
 * @return A boolean about the result of the operation
 * @retval - true : If the token array had been increased
 * @retval - false : If `token_array` == NULL or the reallocation failed
 */
bool TokenArray_increaseCapacity(TokenArray* token_array);

/**
 * @brief Add a token at the end of the token array
 * 
 * @param token_array The TokenArray object
 * @param token The new Token to add
 * @return A boolean about the result of the operation
 * @retval - true : If the token is well added
 * @retval - false : If `token_array` == NULL or the capacity increase failed
 */
bool TokenArray_addToken(TokenArray* token_array, Token* token);

/**
 * @brief Get the token at the index given
 * 
 * @param token_array The TokenArray object
 * @param index The index at which the token to get is
 * @return The Token to retrieve at index
 * @retval NULL : If `index` >= token_array->count or `` == NULL
 */
Token* TokenArray_getTokenAtIndex(TokenArray* token_array, int index);


// <========================================================================>
// <========================================================================>
// <========================================================================>


// <========================================================================>
// <======================= Token parsing functions ========================>
// <========================================================================>

bool isASingularCharOperationToken(char *str, Token* token);

bool isADoubleCharOperationToken(char *str, Token* token);

/**
 * @brief Determine if `str` == "si" and modify the content of `token` if so
 * 
 * @param str The string to compare
 * @param token The token to modify if true
 * @return A boolean about if `str` == "si"
 * @retval false : If `str` != "si", or `str` == NULL or the copy of `str` failed
 * @retval true : If `str` == "si"
 * @warning Modify the token in consequence. No need to use another regExp function after `true` return
 */
bool isSi(char* str, Token* token);

bool isFin(char* str, Token* token);

bool isAFourCharToken(char *str, Token* token);

bool isSinon(char* str, Token* token);

bool isASixCharToken(char *str, Token* token);

bool isTantque(char* str, Token* token);

// <========================================================================>
// <========================================================================>
// <========================================================================>

// <========================================================================>
// <=========================== Parsing Core ===============================>
// <========================================================================>

TokenArray* Lexer_parseFile(char* source_code);

// <========================================================================>
// <========================================================================>
// <========================================================================>
