/**
 * @file Lexer_functions.h
 * @brief Declaration of all the essential functions to parse the source code
 * @author Rosol Simon
 * @date 2026-07-18
 */

#ifndef LEXER
#define LEXER

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#include "Token.h"
#include "../helper/string_functions.h"
#include "../helper/DynamicString.h"


// <========================================================================>
// <=========================== Token functions ============================>
// <========================================================================>

/**
 * @defgroup Token
 * @brief All the functions to manipulate Token structure
 * @ingroup Lexer
 * @{
 */

/**
 * @brief Create a new Token object
 * 
 * @param token_type Type of the new token
 * @param content Content of the new token
 * @param line Line of the new token in the code
 * @param column Column of the new token in the code
 * @return A new Token object
 * @retval NULL : If `content` == NULL or the allocation (the Token struct or the copy of `content`) failed
 * @warning `content` isn't freed by this function
 */
Token* Token_createToken(TokenType token_type, char* content, int line, int column);

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

/**
 * @brief Retrieve the line of the Token
 * 
 * @return The line of the Token in the code
 * @retval -1 : If `token` == NULL
 */
int Token_getLine(Token* token);

/**
 * @brief Retrieve the column of the Token
 * 
 * @return The Column of the Token in the code
 * @retval -1 : If `token` == NULL
 */
int Token_getColumn(Token* token);

/** @} */

// <========================================================================>
// <========================================================================>
// <========================================================================>


// <========================================================================>
// <======================== TokenArray functions ==========================>
// <========================================================================>

/**
 * @defgroup TokenArray
 * @brief All the functions to manipulate the TokenArray structure
 * @ingroup Lexer
 * @{
 */

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

/**@} */


// <========================================================================>
// <========================================================================>
// <========================================================================>


// <========================================================================>
// <======================= Token parsing functions ========================>
// <========================================================================>

/**
 * @defgroup Token Parsing
 * @brief All the function to compare characters and transform them into tokens
 * @ingroup Lexer
 */

bool isASingularCharOperationToken(char char_to_compare, Token* token);

bool isADoubleCharOperationToken(char *str, Token* token);

/**
 * @brief Determine if `str` == "si" and modify the content of `token` if so
 * 
 * @param str The string to compare
 * @param token The token to modify if true
 * @return A boolean about if `str` == "si"
 * @retval false : If `str` != "si", or `str` == NULL or the copy of `str` failed
 * @warning Modify the token in consequence. No need to use another regExp function after `true` return
 */
bool isSi(char* str, Token* token);

bool isFin(char* str, Token* token);

bool isAFourCharToken(char *str, Token* token);

bool isSinon(char* str, Token* token);

bool isASixCharToken(char *str, Token* token);

bool isASevenCharToken(char *str, Token* token);

bool isKeyword(char* str, Token* token);

bool isEntier(char* str, Token* token);

/**@} */

// <========================================================================>
// <========================================================================>
// <========================================================================>

// <========================================================================>
// <=========================== Parsing Core ===============================>
// <========================================================================>

/**
 * @defgroup Parsing Core
 * @brief The function that transform a source code into a TokenArray
 * @ingroup Lexer
 * @{
 */

TokenArray* Lexer_parseFile(char* source_code);

/** @} */

// <========================================================================>
// <========================================================================>
// <========================================================================>

#endif
