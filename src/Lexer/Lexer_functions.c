#include "../../include/Lexer/Lexer_functions.h"


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

bool Token_setContent(Token* token, char* new_content) {
    if (token == NULL) return false;
    if (new_content == NULL) return false;
    char* content = Token_getContent(token);
    char* new_content_copy = string_copy(new_content);
    if (new_content_copy == NULL) return false;
    token->content = new_content_copy;
    if (content != NULL) free(content);
    return true;
}

bool Token_setTokenType(Token* token, TokenType token_type) {
    if (token == NULL) return false;
    token->token_type = token_type;
    return true;
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

/**
 * @brief Increase the count of the token array
 * 
 * @param token_array The TokenArray object
 * @return A boolean about the result of the operation
 * @retval - true : If the count had been increased
 * @retval - false : If `token_array` == NULL
 * @warning If called without inserting a value at the former index, it will leave an allocated place never valued (so not accessible while freeing it)
 */
bool TokenArray_increaseCount(TokenArray* token_array) {
    if (token_array == NULL) return false;
    INCREMENT_COUNT(token_array);
    return true;
}

/**
 * @brief Increase the capacity of the toke array by multiplying it by 2
 * 
 * @param token_array The TokenArray object
 * @return A boolean about the result of the operation
 * @retval - true : If the token array had been increased
 * @retval - false : If `token_array` == NULL or the reallocation failed
 */
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

const Token FOUR_CHAR_TOKENS[] = {
    {TOKEN_MOTCLE_CHAR, "char"},
    {TOKEN_VIDE, "vide"},
    {TOKEN_ERREUR, NULL}};

const Token SIX_CHAR_TOKENS[] = {
    {TOKEN_MOTCLE_ENTIER, "entier"},
    {TOKEN_MOTCLE_CHAINE, "chaine"},
    {TOKEN_RENVOI, "renvoi"},
    {TOKEN_ERREUR, "erreur"},
    {TOKEN_ERREUR, NULL}
};

const Token SEVEN_CHAR_TOKENS[] = {
    {TOKEN_TANT_QUE, "tantque"},
    {TOKEN_AFFICHE, "affiche"},
    {TOKEN_ERREUR, NULL}
};

const Token SINGULAR_CHAR_OPERATION_TOKENS[] = {   
    {TOKEN_PLUS, "+"},              
    {TOKEN_MOINS, "-"},             
    {TOKEN_FOIS, "*"},              
    {TOKEN_DIVISE, "/"},            
    {TOKEN_RESTE, "%"},
    {TOKEN_AFFECTATION, "="},
    {TOKEN_INFERIEUR, "<"},
    {TOKEN_SUPERIEUR, ">"},
    {TOKEN_ET_BINAIRE, "&"},
    {TOKEN_OU_BINAIRE, "|"},
    {TOKEN_NON, "!"},
    {TOKEN_PARENTHESE_GAUCHE, "("}, 
    {TOKEN_PARENTHESE_DROITE, ")"}, 
    {TOKEN_ACCOLADE_GAUCHE, "{"},   
    {TOKEN_ACCOLADE_DROITE, "}"},   
    {TOKEN_POINT_VIRGULE, ";"},     
    {TOKEN_VIRGULE, ","},
    {TOKEN_ERREUR, NULL}
};

const Token DOUBLE_CHAR_OPERATION_TOKENS[] = {
    {TOKEN_EGAL, "=="},          
    {TOKEN_DIFFERENT, "!="},     
    {TOKEN_INFERIEUR_EGAL, "<="},
    {TOKEN_SUPERIEUR_EGAL, ">="},
    {TOKEN_ET, "&&"},            
    {TOKEN_OU, "||"},            
    {TOKEN_ERREUR, NULL}
};

bool isASingularCharOperationToken(char char_to_compare, Token* token) {
    if (token == NULL) return false;
    int index = 0;
    while (SINGULAR_CHAR_OPERATION_TOKENS[index].content != NULL) {
        if (char_to_compare == SINGULAR_CHAR_OPERATION_TOKENS[index].content[0]) {
            token->token_type = SINGULAR_CHAR_OPERATION_TOKENS[index].token_type;

            char* token_content = Token_getContent(token);
            char* new_token_content = string_copy(SINGULAR_CHAR_OPERATION_TOKENS[index].content);
            if (new_token_content == NULL) return false;
            token->content = new_token_content;
            free(token_content);
            return true;
        }
        else index++;
    }
    return false;
}

bool isADoubleCharOperationToken(char *str, Token* token) {
    if (str == NULL) return false;
    if (token == NULL) return false;
    if (string_size(str) != 1) return false;
    int index = 0;
    while (DOUBLE_CHAR_OPERATION_TOKENS[index].content != NULL) {
        bool token_equal = true;
        for (int index_char = 0; index_char < string_size(DOUBLE_CHAR_OPERATION_TOKENS[index].content); index_char++) {
            if (str[index_char] != DOUBLE_CHAR_OPERATION_TOKENS[index].content[index_char]) {
                token_equal = false;
                break;
            }
        }
        if (token_equal) {
            token->token_type = DOUBLE_CHAR_OPERATION_TOKENS[index].token_type;

            char* token_content = Token_getContent(token);
            char* new_token_content = string_copy(str);
            if (new_token_content == NULL) return false;
            token->content = new_token_content;
            free(token_content);
            return true;
        }
        else index++;
    }
    return false;
}

bool isSi(char* str, Token* token) {
    if (str == NULL) return false;
    if (token == NULL) return false;
    if (string_size(str) != 2) return false;
    if (str[0] == 's' && str[1] == 'i') {
        token->token_type = TOKEN_SI;

        char* token_content = Token_getContent(token);
        char* new_token_content = string_copy(str);
        if (new_token_content == NULL) return false;
        token->content = new_token_content;
        free(token_content);
        return true;
    }
    return false;
}

bool isFin(char* str, Token* token) {
    if (str == NULL) return false;
    if (token == NULL) return false;
    if (string_size(str) != 3) return false;
    if (str[0] == 'f' && str[1] == 'i' && str[2] == 'n') {
        token->token_type = TOKEN_FDF;

        char* token_content = Token_getContent(token);
        char* new_token_content = string_copy(str);
        if (new_token_content == NULL) return false;
        token->content = new_token_content;
        free(token_content);
        return true;
    }
    return false;
}

bool isAFourCharToken(char *str, Token* token) {
    if (str == NULL) return false;
    if (token == NULL) return false;
    if (string_size(str) != 4) return false;
    int index = 0;
    while (FOUR_CHAR_TOKENS[index].content != NULL) {
        bool token_equal = true;
        for (int index_char = 0; index_char < string_size(FOUR_CHAR_TOKENS[index].content); index_char++) {
            if (str[index_char] != FOUR_CHAR_TOKENS[index].content[index_char]) {
                token_equal = false;
                break;
            }
        }
        if (token_equal) {
            token->token_type = FOUR_CHAR_TOKENS[index].token_type;

            char* token_content = Token_getContent(token);
            char* new_token_content = string_copy(str);
            if (new_token_content == NULL) return false;
            token->content = new_token_content;
            free(token_content);
            return true;
        }
        else index++;
    }
    return false;
}

bool isSinon(char* str, Token* token) {
    if (str == NULL) return false;
    if (token == NULL) return false;
    if (string_size(str) != 5) return false;
    if (str[0] == 's' && str[1] == 'i' && str[2] == 'n' && str[3] == 'o' && str[4] == 'n') {
        token->token_type = TOKEN_SINON;

        char* token_content = Token_getContent(token);
        char* new_token_content = string_copy(str);
        if (new_token_content == NULL) return false;
        token->content = new_token_content;
        free(token_content);
        return true;
    }
    return false;
}

bool isASixCharToken(char *str, Token* token) {
    if (str == NULL) return false;
    if (token == NULL) return false;
    if (string_size(str) != 6) return false;
    int index = 0;
    while (SIX_CHAR_TOKENS[index].content != NULL) {
        bool token_equal = true;
        for (int index_char = 0; index_char < string_size(SIX_CHAR_TOKENS[index].content); index_char++) {
            if (str[index_char] != SIX_CHAR_TOKENS[index].content[index_char]) {
                token_equal = false;
                break;
            }
        }
        if (token_equal) {
            token->token_type = SIX_CHAR_TOKENS[index].token_type;

            char* token_content = Token_getContent(token);
            char* new_token_content = string_copy(str);
            if (new_token_content == NULL) return false;
            token->content = new_token_content;
            free(token_content);
            return true;
        }
        else index++;
    }
    return false;
}

bool isASevenCharToken(char *str, Token* token) {
    if (str == NULL) return false;
    if (token == NULL) return false;
    if (string_size(str) != 7) return false;
    int index = 0;
    while (SEVEN_CHAR_TOKENS[index].content != NULL) {
        bool token_equal = true;
        for (int index_char = 0; index_char < string_size(SEVEN_CHAR_TOKENS[index].content); index_char++) {
            if (str[index_char] != SEVEN_CHAR_TOKENS[index].content[index_char]) {
                token_equal = false;
                break;
            }
        }
        if (token_equal) {
            token->token_type = SEVEN_CHAR_TOKENS[index].token_type;

            char* token_content = Token_getContent(token);
            char* new_token_content = string_copy(str);
            if (new_token_content == NULL) return false;
            token->content = new_token_content;
            free(token_content);
            return true;
        }
        else index++;
    }
    return false;
}

bool isKeyword(char* str, Token* token) {
    if (str == NULL) return false;
    if (token == NULL) return false;
    int str_size = string_size(str);
    switch (str_size) {
        case 2: {
            return isSi(str, token);
        }
        case 3: {
            return isFin(str, token);
        }
        case 4: {
            return isAFourCharToken(str, token);
        }
        case 5: {
            return isSinon(str, token);
        }
        case 6: {
            return isASixCharToken(str, token);
        }
        case 7: {
            return isASevenCharToken(str, token);
        }
        default: return false;
    }
}

bool isEntier(char* str, Token* token) {
    if (str == NULL) return false;
    if (token == NULL) return false;

    if (string_size(str) == 0) return false;

    int index = 0;

    if (str[0] == '-' || str[0] == '+') {
        index = 1;
        if (str[1] == '\0') return false; 
    }

    while (str[index] != '\0') {
        if (!isdigit(str[index])) return false;
        index++;
    }
    return (Token_setContent(token, str) && Token_setTokenType(token, TOKEN_ENTIER));
}


Token* getCharToken(char* source_code, int* index, int source_code_size) {
    if (source_code == NULL) return NULL;
    if (*index < 0 || *index >= source_code_size - 2) {
        *index += 1;
        return Token_createToken(TOKEN_ERREUR, "End of file didn't allow the char to close");
    }
    if (source_code[*index + 2] != '\'') return Token_createToken(TOKEN_ERREUR, "Invalid format of char");
    DynamicString* dynamic_string = DynamicString_create();
    *index += 1;
    DynamicString_addChar(dynamic_string, source_code[*index]);
    *index += 2;
    Token* token = Token_createToken(TOKEN_CHAR, DynamicString_getString(dynamic_string));
    DynamicString_free(dynamic_string);
    return token;
}

Token* getStringToken(char* source_code, int* index, int source_code_size) {
    if (source_code == NULL) return NULL;
    if (*index < 0 || *index >= source_code_size - 1) return NULL;
    *index += 1;
    DynamicString* dynamic_string = DynamicString_create();
    while (source_code[*index] != '\"' && source_code[*index] != '\0') {
        DynamicString_addChar(dynamic_string, source_code[*index]);
        *index += 1;
    }
    if (source_code[*index] == '\0') {
        DynamicString_free(dynamic_string);
        return Token_createToken(TOKEN_ERREUR, "String never closed");
    }
    else {
        Token* token = Token_createToken(TOKEN_CHAINE, DynamicString_getString(dynamic_string));
        DynamicString_free(dynamic_string);
        *index += 1;
        return token;
    }
}

// <========================================================================>
// <========================================================================>
// <========================================================================>



// <========================================================================>
// <=========================== Parsing Core ===============================>
// <========================================================================>

TokenArray* Lexer_parseFile(char* source_code) {
    TokenArray* token_array = TokenArray_create(TOKEN_ARRAY_CAPACITY_BASE);
    int source_code_size = string_size(source_code);
    int index = 0;
    DynamicString* dynamic_string = DynamicString_create();
    while (source_code[index] != '\0') {
        char actual_char = source_code[index];
        if (isspace(actual_char)) {
            if (DynamicString_getCount(dynamic_string) > 0) {
                Token* token = Token_createToken(TOKEN_IDENTIFIANT, "");
                if (!isKeyword(DynamicString_getString(dynamic_string), token) && !isEntier(DynamicString_getString(dynamic_string), token)) Token_setContent(token, DynamicString_getString(dynamic_string));
                TokenArray_addToken(token_array, token);
                dynamic_string = DynamicString_recycleString(dynamic_string);
            }
            index++;
        }
        else if (isalpha(actual_char)) {
            DynamicString_addChar(dynamic_string, actual_char);
            index++;
        }
        else if (isdigit(actual_char)) {
            DynamicString_addChar(dynamic_string, actual_char);
            index++;
        }
        else if (actual_char == '\'') {
            Token* charToken = getCharToken(source_code, &index, source_code_size);
            TokenArray_addToken(token_array, charToken);
            if (Token_getTokenType(charToken) == TOKEN_ERREUR) {
                DynamicString_free(dynamic_string);
                return token_array;
            }
        }
        else if (actual_char == '\"') {
            Token* stringToken = getStringToken(source_code, &index, source_code_size);
            TokenArray_addToken(token_array, stringToken);
            if (Token_getTokenType(stringToken) == TOKEN_ERREUR) {
                DynamicString_free(dynamic_string);
                return token_array;
            }
        }
        else {
            if (DynamicString_getCount(dynamic_string) > 0) {
                Token* token = Token_createToken(TOKEN_IDENTIFIANT, "");
                if (!isKeyword(DynamicString_getString(dynamic_string), token) && !isEntier(DynamicString_getString(dynamic_string), token)) Token_setContent(token, DynamicString_getString(dynamic_string));
                TokenArray_addToken(token_array, token);
                dynamic_string = DynamicString_recycleString(dynamic_string);
            }
            else if (index + 1 <= source_code_size) {
                    DynamicString_addChar(dynamic_string, actual_char);
                    DynamicString_addChar(dynamic_string, source_code[index + 1]);

                Token* token = Token_createToken(TOKEN_ERREUR, "");
                if (isADoubleCharOperationToken(DynamicString_getString(dynamic_string), token)) {
                    TokenArray_addToken(token_array, token);
                    index += 2;
                }
                else if (isASingularCharOperationToken(actual_char, token)) {
                    TokenArray_addToken(token_array, token);
                    index++;
                }
                else Token_free(token);
                dynamic_string = DynamicString_recycleString(dynamic_string);
            }
            else {
                TokenArray_addToken(token_array, Token_createToken(TOKEN_ERREUR, "Misplaced special character"));
                DynamicString_free(dynamic_string);
                return token_array;
            }
        }
    }

    if (DynamicString_getCount(dynamic_string) > 0) {
        Token* token = Token_createToken(TOKEN_IDENTIFIANT, "");
        if (!isKeyword(DynamicString_getString(dynamic_string), token) && !isEntier(DynamicString_getString(dynamic_string), token)) Token_setContent(token, DynamicString_getString(dynamic_string));
        TokenArray_addToken(token_array, token);
    }

    DynamicString_free(dynamic_string);
    return token_array;
}

// <========================================================================>
// <========================================================================>
// <========================================================================>
