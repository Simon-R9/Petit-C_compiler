#include "Lexer_functions.h"


// <========================================================================>
// <========================== Helper functions ============================>
// <========================================================================>

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

// <========================================================================>
// <========================================================================>
// <========================================================================>


// <========================================================================>
// <=========================== Token functions ============================>
// <========================================================================>


Token* Token_createToken(TokenType token_type, char* content) {
    if (content == NULL) return NULL;
    Token* token = (Token*)malloc(sizeof(Token));
    if (token == NULL) return NULL;

    token->token_type = token_type;
    char* copy_content = string_copy(content);
    if (copy_content == NULL) return NULL;
    token->content = copy_content;
    return token;
}

void Token_free(Token* token) {
    if (token == NULL) return;
    if (token->content != NULL) free(token->content);
    free(token);
    return;
}

TokenType Token_getTokenType(Token* token) {
    if (token == NULL) return -1;
    return token->token_type;
}

char* Token_getContent(Token* token) {
    if (token == NULL) return NULL;
    return token->content;
}

// <========================================================================>
// <========================================================================>
// <========================================================================>

// <========================================================================>
// <======================== TokenArray functions ==========================>
// <========================================================================>

TokenArray* TokenArray_create(int capacity) {
    if (capacity <= 0) return NULL;

    TokenArray *token_array = (TokenArray*)malloc(sizeof(TokenArray));
    if (token_array == NULL) return NULL;

    Token** tokens = (Token**)malloc(sizeof(Token*) * TOKEN_ARRAY_CAPACITY_BASE);
    if (tokens == NULL) {
        free(token_array);
        return NULL;
    }
    token_array->tokens = tokens;

    token_array->capacity = TOKEN_ARRAY_CAPACITY_BASE;
    token_array->count = 0;

    return token_array;
}

bool TokenArray_free(TokenArray* token_array) {
    if (token_array == NULL) return false;
    Token** tokens = token_array->tokens;
    if (tokens != NULL) {
        int count = TokenArray_getCount(token_array);
        for (int index = 0; index < count; index++) {
            if (tokens[index] != NULL) Token_free(tokens[index]);
        }
        free(tokens);
    }
    free(token_array);
    return true;
}

Token** TokenArray_getTokens(TokenArray* token_array) {
    if (token_array == NULL) return NULL;
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
    int new_capacity = TokenArray_getCapacity(token_array) * 2;
    int new_size_in_bytes = new_capacity * sizeof(Token*);
    Token** new_tokens = realloc(TokenArray_getTokens(token_array), new_size_in_bytes);
    if (new_tokens == NULL) {
        return false;
    }
    token_array->tokens = new_tokens;
    token_array->capacity = new_capacity;
    return true;
}

bool TokenArray_addToken(TokenArray* token_array, Token* token) {
    if (token_array == NULL) return false;
    int capacity = TokenArray_getCapacity(token_array);
    int count = TokenArray_getCount(token_array);
    if (capacity == count) {
        bool increase = TokenArray_increaseCapacity(token_array);
        if (!increase) return false;
    }

    Token** tokens = TokenArray_getTokens(token_array);
    tokens[count] = token;
    if (!TokenArray_increaseCount(token_array)) return false;
    return true;
}

Token* TokenArray_getTokenAtIndex(TokenArray* token_array, int index) {
    if (index >= TokenArray_getCount(token_array) || token_array == NULL) return NULL;
    Token** tokens = TokenArray_getTokens(token_array);
    if (tokens == NULL) return NULL;
    return tokens[index];
}


// <========================================================================>
// <========================================================================>
// <========================================================================>


// <========================================================================>
// <======================= Token parsing functions ========================>
// <========================================================================>

const char* FOUR_CHAR_TOKEN[] = {"char", "vide"};
const char* SIX_CHAR_TOKEN[] = {"entier", "chaine", "renvoi", "erreur"};
const char SINGULAR_CHAR_OPERATION_TOKEN[] = {'=', '+', '-', '*', '/', '%', '<', '>', '(', ')', '{', '}', ';', ',', '"'};
const char* DOUBLE_CHAR_OPERATION_TOKEN[] = {"==", "!=", "<=", ">=", "&&", "||"};

bool isSi(char* str, Token* token) {
    if (str == NULL) return false;
    if (str[0] == 's' && str[1] == 'i' && str[2] == '\0') {
        token->token_type = TOKEN_SI;

        char* token_content = Token_getContent(token);
        char* new_token_content = string_copy(token_content);
        if (new_token_content == NULL) return false;
        token->content = new_token_content;
        free(token_content);
        return true;
    }
    return false;
}

// <========================================================================>
// <========================================================================>
// <========================================================================>
