/**
 * @file Parser_functions.h
 * @brief Declaration of useful functions for the Parser to retrieve TokenArray from the Lexer and create a dedicated AST
 * @author Rosol Simon
 * @date 2026-07-24
 */

#ifndef PARSER
#define PARSER

#include "ASTNode.h"
#include "../Lexer/Lexer_functions.h"

#define NODE_BASE_CAPACITY 100

typedef struct _Parser {
    TokenArray* tokens;
    int current_index;
} Parser;

// <========================================================================>
// <=========================== Token Management ===========================>
// <========================================================================>

Parser* Parser_create(TokenArray* tokens);

TokenArray* Parser_getTokenArray(Parser* parser);

int Parser_getCurrentIndex(Parser* parser);

Token* Parser_peek(Parser* parser);

bool Parser_increment(Parser* parser);

bool Parser_advance(Parser *parser);

Token* Parser_consume(Parser* parser, TokenType expected_token_type);

// <========================================================================>
// <========================================================================>
// <========================================================================>


// <========================================================================>
// <========================== AST Constructors ============================>
// <========================================================================>


// NodeProgramme

ASTNode* NodeProgramme_create();

ASTNode** NodeProgramme_getInstructions(NodeProgramme* node_programme);

int NodeProgramme_getInstructionsCapacity(NodeProgramme* node_programme);

int NodeProgramme_getInstructionsCount(NodeProgramme* node_programme);

bool NodeProgramme_increaseCapacity(NodeProgramme* node_programme);

bool NodeProgramme_increaseCount(NodeProgramme* node_programme);

bool NodeProgramme_addInstruction(NodeProgramme* node_programme, ASTNode* instruction);


// NodeDeclareVariable

ASTNode* NodeDeclareVariable_create(char* type, char* name, ASTNode* value);

char* NodeDeclareVariable_getType(NodeDeclareVariable* node_declare_variable);

char* NodeDeclareVariable_getName(NodeDeclareVariable* node_declare_variable);

ASTNode* NodeDeclareVariable_getValue(NodeDeclareVariable* node_declare_variable);

// <========================================================================>
// <========================================================================>
// <========================================================================>


// <========================================================================>
// <====================== Recursive Descent Parser ========================>
// <========================================================================>



// <========================================================================>
// <========================================================================>
// <========================================================================>


// <========================================================================>
// <========================== Debug & Free Zone ===========================>
// <========================================================================>



// <========================================================================>
// <========================================================================>
// <========================================================================>

#endif