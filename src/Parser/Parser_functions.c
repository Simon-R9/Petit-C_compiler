/**
 * @file Parser_functions.c
 * @brief Core of the Parser functions
 * @author Rosol Simon
 * @date 2026-07-24
 */

#include "../../include/Parser/Parser_functions.h"

// <========================================================================>
// <=========================== Token Management ===========================>
// <========================================================================>

Parser* Parser_create(TokenArray* tokens) {
    if (tokens == NULL) return NULL;
    Parser* parser = (Parser*)malloc(sizeof(Parser));
    parser->tokens = tokens;
    parser->current_index = 0;
    return parser;
}

TokenArray* Parser_getTokenArray(Parser* parser) {
    if (parser == NULL) return NULL;
    return parser->tokens;
}

int Parser_getCurrentIndex(Parser* parser) {
    if (parser == NULL) return -1;
    return parser->current_index;
}

Token* Parser_peek(Parser* parser) {
    if (parser == NULL) return NULL;
    return TokenArray_getTokenAtIndex(Parser_getTokenArray(parser), Parser_getCurrentIndex(parser));
}

bool Parser_increment(Parser* parser) {
    if (parser == NULL) return false;
    parser->current_index++;
    return true;
}

bool Parser_advance(Parser *parser) {
    if (parser == NULL) return false;
    if (Parser_getCurrentIndex(parser) < TokenArray_getCount(Parser_getTokenArray(parser))) {
        if (Parser_increment(parser)) return true;
    }
    return false;
}

Token* Parser_consume(Parser* parser, TokenType expected_token_type) {
    if (parser == NULL) return NULL;
    Token* current_token = Parser_peek(parser);
    if (current_token == NULL) printf("Syntax Error : Unexpected end of file.\n");

    if (Token_getTokenType(current_token) == expected_token_type) {
        Parser_advance(parser);
        return current_token;
    }
    else {
        printf("Syntax Error at line %d, column %d: Expected token type %d, but got %d ('%s')\n", Token_getLine(current_token), Token_getColumn(current_token), expected_token_type, Token_getTokenType(current_token), Token_getContent(current_token));
    }
    exit(EXIT_FAILURE);
}

// <========================================================================>
// <========================================================================>
// <========================================================================>


// <========================================================================>
// <========================== AST Constructors ============================>
// <========================================================================>


// NodeProgramme

ASTNode* NodeProgramme_create() {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;
    ASTNode** instructions = (ASTNode**)malloc(sizeof(ASTNode*) * NODE_BASE_CAPACITY);
    if (instructions == NULL) {
        free(node);
        return NULL;
    }
    node->node_programme.instructions = instructions;
    node->node_programme.instructions_capacity = NODE_BASE_CAPACITY;
    node->node_programme.instructions_count = 0;

    node->type = NODE_PROGRAMME;
    return node;
}

ASTNode** NodeProgramme_getInstructions(NodeProgramme* node_programme) {
    if (node_programme == NULL) return NULL;
    return node_programme->instructions;
}

int NodeProgramme_getInstructionsCapacity(NodeProgramme* node_programme) {
    if (node_programme == NULL) return -1;
    return node_programme->instructions_capacity;
}

int NodeProgramme_getInstructionsCount(NodeProgramme* node_programme) {
    if (node_programme == NULL) return -1;
    return node_programme->instructions_count;
}

bool NodeProgramme_increaseCapacity(NodeProgramme* node_programme) {
    if (node_programme == NULL) return false;
    int new_capacity = NodeProgramme_getInstructionsCapacity(node_programme) * 2;
    ASTNode** instructions = NodeProgramme_getInstructions(node_programme);
    if (instructions == NULL) return false;
    ASTNode** new_instructions = (ASTNode**)realloc(instructions, new_capacity);
    if (new_instructions == NULL) return false;
    node_programme->instructions = new_instructions;
    node_programme->instructions_capacity = new_capacity;
    return true;
}

bool NodeProgramme_increaseCount(NodeProgramme* node_programme) {
    if (node_programme == NULL) return false;
    node_programme->instructions_count++;
    return true;
}

bool NodeProgramme_addInstruction(NodeProgramme* node_programme, ASTNode* instruction) {
    if (node_programme == NULL) return false;
    if (instruction == NULL) return false;
    int instructions_count = NodeProgramme_getInstructionsCount(node_programme);
    if (instructions_count == NodeProgramme_getInstructionsCapacity(node_programme)) {
        if (!NodeProgramme_increaseCapacity(node_programme)) {
            fprintf(stderr, "Fatal Error: Reallocation of node programm failed");
            exit(EXIT_FAILURE);
        }
    }
    ASTNode** instructions = NodeProgramme_getInstructions(node_programme);
    instructions[instructions_count] = instruction;

    return NodeProgramme_increaseCount(node_programme);
}


// NodeDeclareVariable

ASTNode* NodeDeclareVariable_create(char* type, char* name, ASTNode* value) {
    if (type == NULL || name == NULL || value == NULL) return NULL;
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;
    node->node_declare_variable.type = type;
    node->node_declare_variable.name = name;
    node->node_declare_variable.value = value;
    return node;
}

char* NodeDeclareVariable_getType(NodeDeclareVariable* node_declare_variable) {
    if (node_declare_variable == NULL) return NULL;
    return node_declare_variable->type;
}

char* NodeDeclareVariable_getName(NodeDeclareVariable* node_declare_variable) {
    if (node_declare_variable == NULL) return NULL;
    return node_declare_variable->name;
}

ASTNode* NodeDeclareVariable_getValue(NodeDeclareVariable* node_declare_variable) {
    if (node_declare_variable == NULL) return NULL;
    return node_declare_variable->value;
}

// NodeAssigneVariable

ASTNode* NodeAssigneVariable_create(char* name, ASTNode* value) {
    if (value == NULL || name == NULL) return NULL;
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;
    node->node_assigne_variable.name = name;
    node->node_assigne_variable.value = value;
    return node;
}

char* NodeAssigneVariable_getName(NodeAssigneVariable* node_assigne_variable) {
    if (node_assigne_variable == NULL) return NULL;
    return node_assigne_variable->name;
}

ASTNode* NodeAssigneVariable_getValue(NodeAssigneVariable* node_assigne_variable) {
    if (node_assigne_variable == NULL) return NULL;
    return node_assigne_variable->value;
}

// NodeParametresFonction

ASTNode* NodeParametresFonction_create() {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;
    Parametre** parameters = (Parametre**)malloc(sizeof(Parametre) * NODE_BASE_CAPACITY);
    if (parameters == NULL) {
        free(node);
        return NULL;
    }

    node->node_parametres_fonction.parameters = parameters;
    node->node_parametres_fonction.parameters_capacity = NODE_BASE_CAPACITY;
    node->node_parametres_fonction.parameters_count = 0;
    return node;
}

Parametre** NodeParametresFonction_getParameters(NodeParametresFonction* node_parametres_fonction) {
    if (node_parametres_fonction == NULL) return NULL;
    return node_parametres_fonction->parameters;
}

int NodeParametresFonction_getCapacity(NodeParametresFonction* node_parametres_fonction) {
    if (node_parametres_fonction == NULL) return -1;
    return node_parametres_fonction->parameters_capacity;
}

int NodeParametresFonction_getCount(NodeParametresFonction* node_parametres_fonction) {
    if (node_parametres_fonction == NULL) return -1;
    return node_parametres_fonction->parameters_count;
}

bool NodeParametresFonction_increaseCapacity(NodeParametresFonction* node_parametres_fonction) {
    if (node_parametres_fonction == NULL) return false;
    int new_capacity = NodeParametresFonction_getCapacity(node_parametres_fonction) * 2;
    Parametre** parameters = NodeParametresFonction_getParameters(node_parametres_fonction);
    if (parameters == NULL) return false;
    Parametre** new_parameters = (Parametre**)realloc(parameters, new_capacity);
    if (new_parameters == NULL) return false;
    node_parametres_fonction->parameters = new_parameters;
    node_parametres_fonction->parameters_capacity = new_capacity;
    return true;
}

bool NodeParametresFonction_increaseCount(NodeParametresFonction* node_parametres_fonction) {
    if (node_parametres_fonction == NULL) return false;
    node_parametres_fonction->parameters_count++;
    return true;
}

bool NodeParametresFonction_addParameter(NodeParametresFonction* node_parametres_fonction, Parametre* parametre) {
    if (node_parametres_fonction == NULL || parametre == NULL) return false;
    Parametre** parameters = NodeParametresFonction_getParameters(node_parametres_fonction);
    if (parameters == NULL) return false;
    int parameters_capacity = NodeParametresFonction_getCapacity(node_parametres_fonction);
    int parameters_count = NodeParametresFonction_getCount(node_parametres_fonction);
    if (parameters_capacity == parameters_count) {
        if (!NodeParametresFonction_increaseCapacity(node_parametres_fonction)) {
            fprintf(stderr, "Fatal Error: reallocation failed");
            exit(EXIT_FAILURE);
        }
    }
    parameters[parameters_count] = parametre;
    return NodeParametresFonction_increaseCount(node_parametres_fonction);
}

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
