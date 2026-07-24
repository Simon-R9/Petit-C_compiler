#ifndef PARSER
#define PARSER

#include "ASTNode.h"
#include "../Lexer/Lexer_functions.h"

#define PROGRAM_BASE_CAPACITY 100

typedef struct _Parser {
    TokenArray* tokens;
    int current_index;
} Parser;

Parser* Parser_create(TokenArray* tokens);

#endif