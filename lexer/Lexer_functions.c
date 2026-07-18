#include <stdlib.h>
#include <stdbool.h>

#include "Lexer_functions.h"

TokenArray* TokenArray_create(int capacity) {
    if (capacity <= 0) return NULL;

    TokenArray *token_array = (TokenArray*)malloc(sizeof(TokenArray));
    if (token_array == NULL) return NULL;

    TokenType* tokens = (TokenType*)malloc(sizeof(TokenType) * TOKEN_ARRAY_CAPACITY_BASE);
    if (tokens == NULL) {
        free(token_array);
        return NULL;
    }

    token_array->capacity = TOKEN_ARRAY_CAPACITY_BASE;
    token_array->count = 0;

    return token_array;
}

bool TokenArray_free(TokenArray* token_array) {
    if (token_array == NULL) return false;
    TokenType* tokens = token_array->tokens;
    if (tokens != NULL) free(tokens);
    free(token_array);
    return true;
}

TokenType* TokenArray_getTokens(TokenArray* token_array) {
    if (token_array == NULL) return TOKEN_ERREUR;
    return token_array->tokens;
}

int TokenArray_getCapacity(TokenArray* token_array) {
    if (token_array == NULL) return -1;
    return token_array->capacity;
}

int TokenArray_getCount(TokenArray* token_array) {
    if (token_array == NULL) return -1;
    return token_array->count;
}

bool TokenArray_increaseCount(TokenArray* token_array) {
    if (token_array == NULL) return false;
    INCREMENT_COUNT(token_array);
    return true;
}

bool TokenArray_increaseCapacity(TokenArray* token_array) {
    if (token_array == NULL) return false;
    TokenType* new_tokens = realloc(TokenArray_getTokens(token_array), TOKEN_ARRAY_CAPACITY_BASE);
    if (new_tokens == NULL) {
        return false;
    }
    token_array->tokens = new_tokens;
    return true;
}

bool TokenArray_addToken(TokenArray* token_array, TokenType token) {
    if (token_array == NULL) return false;
    TokenType* tokens = TokenArray_getTokens(token_array);
    int capacity = TokenArray_getCapacity(token_array);
    int count = TokenArray_getCount(token_array);
    if (capacity == count) {
        bool increase = TokenArray_increaseCapacity(token_array);
        if (increase) return false;
    }
    tokens[count] = token;
    return true;
}

TokenType TokenArray_getTokenAtIndex(TokenArray* token_array, int index) {
    if (index >= TokenArray_getCount(token_array) || token_array == NULL) return TOKEN_ERREUR;
    TokenType* tokens = TokenArray_getTokens(token_array);
    if (tokens == NULL) return -1;
    return tokens[index];
}


