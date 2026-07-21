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

const Token FOUR_CHAR_TOKENS[] = {
    {TOKEN_CHAR, "char"},
    {TOKEN_VIDE, "vide"},
    {TOKEN_ERREUR, NULL}};

const Token SIX_CHAR_TOKENS[] = {
    {TOKEN_ENTIER, "entier"},
    {TOKEN_CHAINE, "chaine"},
    {TOKEN_RENVOI, "renvoi"},
    {TOKEN_ERREUR, "erreur"},
    {TOKEN_ERREUR, NULL}
};

const Token SINGULAR_CHAR_OPERATION_TOKENS[] = {
    {TOKEN_AFFECTATION, "="},       
    {TOKEN_PLUS, "+"},              
    {TOKEN_MOINS, "-"},             
    {TOKEN_FOIS, "*"},              
    {TOKEN_DIVISE, "/"},            
    {TOKEN_RESTE, "%"},             
    {TOKEN_INFERIEUR, "<"},         
    {TOKEN_SUPERIEUR, ">"},         
    {TOKEN_PARENTHESE_GAUCHE, "("}, 
    {TOKEN_PARENTHESE_DROITE, ")"}, 
    {TOKEN_ACCOLADE_GAUCHE, "{"},   
    {TOKEN_ACCOLADE_DROITE, "}"},   
    {TOKEN_POINT_VIRGULE, ";"},     
    {TOKEN_VIRGULE, ","},           
    {TOKEN_GUILLEMET, "\""},        
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


bool isASingularCharOperationToken(char *str, Token* token) {
    if (str == NULL) return false;
    if (string_size(str) != 1) return false;
    int index = 0;
    while (SINGULAR_CHAR_OPERATION_TOKENS[index].content != NULL) {
        bool token_equal = true;
        for (int index_char = 0; index_char < string_size(SINGULAR_CHAR_OPERATION_TOKENS[index].content); index_char++) {
            if (str[index_char] != SINGULAR_CHAR_OPERATION_TOKENS[index].content[index_char]) {
                token_equal = false;
                break;
            }
        }
        if (token_equal) {
            token->token_type = SINGULAR_CHAR_OPERATION_TOKENS[index].token_type;

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

bool isADoubleCharOperationToken(char *str, Token* token) {
    if (str == NULL) return false;
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

bool isTantque(char* str, Token* token) {
    if (str == NULL) return false;
    if (string_size(str) != 7) return false;
    if (str[0] == 't' && str[1] == 'a' && str[2] == 'n' && str[3] == 't' && str[4] == 'q' && str[5] == 'u' && str[6] == 'e') {
        token->token_type = TOKEN_TANT_QUE;

        char* token_content = Token_getContent(token);
        char* new_token_content = string_copy(str);
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



// <========================================================================>
// <=========================== Parsing Core ===============================>
// <========================================================================>

TokenArray* Lexer_parseFile(char* source_code) {
    TokenArray* token_array = TokenArray_create(TOKEN_ARRAY_CAPACITY_BASE);
    TokenArray_addToken(token_array, Token_createToken(TOKEN_ENTIER, "5"));
    TokenArray_addToken(token_array, Token_createToken(TOKEN_FDF, "fin"));
    return token_array;
}

// <========================================================================>
// <========================================================================>
// <========================================================================>
