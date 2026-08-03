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
 * @defgroup token Token
 * @brief All the functions to manipulate Token structure
 * @ingroup lexer
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
 * @defgroup token_array TokenArray
 * @brief All the functions to manipulate the TokenArray structure
 * @ingroup lexer
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

/** 
 * @} 
 */


// <========================================================================>
// <========================================================================>
// <========================================================================>


// <========================================================================>
// <======================= Token parsing functions ========================>
// <========================================================================>

/**
 * @defgroup token_parsing Token Parsing
 * @brief All the function to compare characters and transform them into tokens
 * @ingroup lexer
 * @{
 */

/**
 * @brief Determine whether the identifier is a singular char operator or not
 * 
 * @param char_to_compare The char identifier to test
 * @param token The Token susceptible to change
 * @retval - false : If the char isn't satisfying the condition, `token` == NULL or the copy of the char in the token failed
 * @warning Modify the token in consequence. No need to use another regExp function after `true` return
 */
bool isASingularCharOperationToken(char char_to_compare, Token* token);

/**
 * @brief Determine whether the identifier is a double char operator or not
 * 
 * @param str The string to test
 * @param token The Token susceptible to change
 * @retval - false : If the string isn't satisfying the condition, `str` == NULL, `token` == NULL or the copy of `str` failed
 * @warning Modify the token in consequence. No need to use another regExp function after `true` return
 */
bool isADoubleCharOperationToken(char *str, Token* token);

/**
 * @brief Determine if the identifier is the keyword 'si' or not
 * 
 * @param str The string to compare
 * @param token The token susceptible to change
 * @retval false : If `str` != "si", or `str` == NULL or the copy of `str` failed
 * @warning Modify the token in consequence. No need to use another regExp function after `true` return
 */
bool isSi(char* str, Token* token);

/**
 * @brief Determine if the identifier is the keyword 'fin' or not
 * 
 * @param str The string to compare
 * @param token The token susceptible to change
 * @retval false : If `str` != "fin", or `str` == NULL or the copy of `str` failed
 * @warning Modify the token in consequence. No need to use another regExp function after `true` return
 */
bool isFin(char* str, Token* token);

/**
 * @brief Determine whether the identifier is a four char keyword or not 
 * 
 * @param str The string to test
 * @param token The Token susceptible to change
 * @retval - false : If the string isn't satisfying the condition, `str` == NULL, `token` == NULL or the copy of `str` failed
 * @warning Modify the token in consequence. No need to use another regExp function after `true` return
 */
bool isAFourCharToken(char *str, Token* token);

/**
 * @brief Determine if the identifier is the keyword 'sinon' or not
 * 
 * @param str The string to compare
 * @param token The token susceptible to change
 * @retval false : If `str` != "sinon", or `str` == NULL or the copy of `str` failed
 * @warning Modify the token in consequence. No need to use another regExp function after `true` return
 */
bool isSinon(char* str, Token* token);

/**
 * @brief Determine whether the identifier is a six char keyword or not
 * 
 * @param str The string to test
 * @param token The Token susceptible to change
 * @retval - false : If the string isn't satisfying the condition, `str` == NULL, `token` == NULL or the copy of `str` failed
 * @warning Modify the token in consequence. No need to use another regExp function after `true` return
 */
bool isASixCharToken(char *str, Token* token);

/**
 * @brief Determine whether the identifier is a seven char keyword or not
 * 
 * @param str The string to test
 * @param token The Token susceptible to change
 * @retval - false : If the string isn't satisfying the condition, `str` == NULL, `token` == NULL or the copy of `str` failed
 * @warning Modify the token in consequence. No need to use another regExp function after `true` return
 */
bool isASevenCharToken(char *str, Token* token);

/**
 * @brief Determine whether the identifier is a keyword or not
 * 
 * @param str The string to test
 * @param token The Token susceptible to change
 * @retval - false : If the string isn't satisfying the condition, `str` == NULL or `token` == NULL
 * @warning Modify the token in consequence
 */
bool isKeyword(char* str, Token* token);

/**
 * @brief Determine if the identifier is the keyword 'entier'
 * 
 * @param str The string to compare
 * @param token The token susceptible to change
 * @retval false : If `str` != "entier", or `str` == NULL or the copy of `str` failed
 * @warning Modify the token in consequence. No need to use another regExp function after `true` return
 */
bool isEntier(char* str, Token* token);

/**
 * @brief Return a token with the char in the source code at the given index
 * 
 * @param source_code The raw source code in a string
 * @param index The index in the source code where to start
 * @param source_code_size The length of the source code string
 * @param current_line The current line in the parsing
 * @param current_column The current column in the parsing
 * @return The token with the char as content property
 * @retval - NULL : If `source_code` == NULL
 * @retval - Token(TOKEN_ERREUR, error_message) : If there was an error
 */
Token* getCharToken(char* source_code, int* index, int source_code_size, int current_line, int current_column);

/**
 * @brief Return a token with the string in the source code at the given index
 * 
 * @param source_code The raw source code in a string
 * @param index The index in the source code where to start
 * @param source_code_size The length of the source code string
 * @param current_line The current line in the parsing
 * @param current_column The current column in the parsing
 * @return The token with the string as content property
 * @retval - NULL : If `source_code` == NULL, `index` < 0 or  `index` >= `source_code_size` - 1
 * @retval - Token(TOKEN_ERREUR, error_message) : If there was an error
 */
Token* getStringToken(char* source_code, int* index, int source_code_size, int current_line, int current_column);

/** @} */

// <========================================================================>
// <========================================================================>
// <========================================================================>

// <========================================================================>
// <=========================== Parsing Core ===============================>
// <========================================================================>

/**
 * @defgroup parsing_core Parsing Core
 * @brief The function that transform a source code into a TokenArray
 * @ingroup lexer
 * @{
 */

/**
 * @brief Read a source code and create a TokenArray dedicated 
 * 
 * @param source_code The string containing the raw source code
 * @return The TokenArray after the parsing
 * @retval - NULL : If `source_code` == NULL
 */
TokenArray* Lexer_parseFile(char* source_code);

/** @} */

// <========================================================================>
// <========================================================================>
// <========================================================================>

#endif
