/**
 * @file Parser_functions.c
 * @brief Core of the Parser functions
 * @author Rosol Simon
 * @date 2026-07-24
 */

#include "../../include/Parser/Parser_functions.h"
#include <string.h>

// <========================================================================>
// <=========================== Token Management ===========================>
// <========================================================================>

/**
 * @addtogroup parser_struct
 */

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

Token* Parser_peekAt(Parser* parser, int offset) {
    if (parser == NULL || offset < 0) return NULL;
    int sizeTokenArray = TokenArray_getCount(Parser_getTokenArray(parser));
    if (Parser_getCurrentIndex(parser) + offset >= sizeTokenArray) return NULL;
    return TokenArray_getTokenAtIndex(Parser_getTokenArray(parser), Parser_getCurrentIndex(parser) + offset);
}

Token* Parser_peek(Parser* parser) {
    if (parser == NULL) return NULL;
    return Parser_peekAt(parser, 0);
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

bool Parser_free(Parser* parser) {
    if (parser == NULL) return true;
    free(parser);
    return true;
}

// <========================================================================>
// <========================================================================>
// <========================================================================>


// <========================================================================>
// <========================== AST Constructors ============================>
// <========================================================================>

/**
 * @addtogroup ast_constructors
 * @{
 */

/**
 * @brief Create a new ASTNode object based on the type of the node
 * 
 * @param type Type of the node to create
 * @return The allocated ASTNode object with the type
 * @warning Exit the programm and print an error if allocation fail
 */
static ASTNode* ASTNode_create(NodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Fatal Error: Memory allocation failed for ASTNode\n");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    return node;
}

    /**
     * @addtogroup node_programme
     * @{
     */

    ASTNode* NodeProgramme_create() {
        ASTNode* node = ASTNode_create(NODE_PROGRAMME);
        ASTNode** instructions = (ASTNode**)malloc(sizeof(ASTNode*) * NODE_BASE_CAPACITY);
        if (instructions == NULL) {
            free(node);
            return NULL;
        }
        node->node_programme.instructions = instructions;
        node->node_programme.instructions_capacity = NODE_BASE_CAPACITY;
        node->node_programme.instructions_count = 0;
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

    /** 
     * @}
     */


    /**
     * @addtogroup node_declare_variable
     * @{
     */

    ASTNode* NodeDeclareVariable_create(char* type, char* name, ASTNode* value) {
        if (type == NULL || name == NULL || value == NULL) return NULL;
        ASTNode* node = ASTNode_create(NODE_DECLARE_VARIABLE);
        node->node_declare_variable.type = string_copy(type);
        node->node_declare_variable.name = string_copy(name);
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

    /** 
     * @} 
     */

    
    /**
     * @addtogroup node_assigne_variable
     * @{
     */

    ASTNode* NodeAssigneVariable_create(char* name, ASTNode* value) {
        if (value == NULL || name == NULL) return NULL;
        ASTNode* node = ASTNode_create(NODE_ASSIGNE_VARIABLE);
        node->node_assigne_variable.name = string_copy(name);
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

    /**
     * @}
     */

    /**
     * @addtogroup node_parametres_fonction
     */

    ASTNode* NodeParametresFonction_create() {
        ASTNode* node = ASTNode_create(NODE_PARAMETRES_FONCTION);
        Parametre** parameters = (Parametre**)malloc(sizeof(Parametre*) * NODE_BASE_CAPACITY);
        if (parameters == NULL) {
            free(node);
            return NULL;
        }

        node->node_parametres_fonction.parameters = parameters;
        node->node_parametres_fonction.parameters_capacity = NODE_BASE_CAPACITY;
        node->node_parametres_fonction.parameters_count = 0;
        return node;
    }

    Parametre* Parametre_create(char* type, char* name) {
        if (type == NULL || name == NULL) return NULL;
        Parametre* parameter = (Parametre*)malloc(sizeof(Parametre));
        if (parameter == NULL) return NULL;
        parameter->type = string_copy(type);
        parameter->name = string_copy(name);
        return parameter;
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

    /**
     * @}
     */

    
    /**
     * @addtogroup node_declare_fonction
     * @{
     */

    ASTNode* NodeDeclareFonction_create(char* type, char* name, ASTNode* parameters, ASTNode* function_program) {
        if (type == NULL || name == NULL || parameters == NULL || function_program == NULL) return NULL;
        ASTNode* node = ASTNode_create(NODE_DECLARE_FONCTION);
        node->node_declare_fonction.type = string_copy(type);
        node->node_declare_fonction.name = string_copy(name);
        node->node_declare_fonction.parameters = parameters;
        node->node_declare_fonction.function_program = function_program;
        return node;
    }

    char* NodeDeclareFonction_getType(NodeDeclareFonction* node_declare_fonction) {
        if (node_declare_fonction == NULL) return NULL;
        return node_declare_fonction->type;
    }

    char* NodeDeclareFonction_getName(NodeDeclareFonction* node_declare_fonction) {
        if (node_declare_fonction == NULL) return NULL;
        return node_declare_fonction->name;
    }

    ASTNode* NodeDeclareFonction_getParameters(NodeDeclareFonction* node_declare_fonction) {
        if (node_declare_fonction == NULL) return NULL;
        return node_declare_fonction->parameters;
    }

    ASTNode* NodeDeclareFonction_getFunctionProgram(NodeDeclareFonction* node_declare_fonction) {
        if (node_declare_fonction == NULL) return NULL;
        return node_declare_fonction->function_program;
    }

    /**
     * @}
     */

    
    /**
     * @addtogroup node_si
     * @{
     */

    ASTNode* NodeSi_create(ASTNode* condition, ASTNode* then_program, ASTNode* else_program, bool has_else_program) {
        if (condition == NULL || then_program == NULL) return NULL;
        if (else_program == NULL && has_else_program) return NULL;
        ASTNode* node = ASTNode_create(NODE_SI);
        node->node_si.condition = condition;
        node->node_si.then_program = then_program;
        node->node_si.else_instruction = else_program;
        node->node_si.has_else_instruction = has_else_program;
        return node;
    }

    ASTNode* NodeSi_getCondition(NodeSi* node_si) {
        if (node_si == NULL) return NULL;
        return node_si->condition;
    }

    ASTNode* NodeSi_getThenProgram(NodeSi* node_si) {
        if (node_si == NULL) return NULL;
        return node_si->then_program;
    }

    bool NodeSi_hasElseProgram(NodeSi* node_si) {
        if (node_si == NULL) return false;
        return node_si->has_else_instruction;
    }

    ASTNode* NodeSi_getElseProgram(NodeSi* node_si) {
        if (node_si == NULL) return NULL;
        return node_si->else_instruction;
    }

    /**
     * @}
     */

    /**
     * @addtogroup node_tant_que
     * @{
     */

    ASTNode* NodeTantQue_create(ASTNode* condition, ASTNode* while_program) {
        if (condition == NULL || while_program == NULL) return NULL;
        ASTNode* node = ASTNode_create(NODE_TANT_QUE);
        node->node_tant_que.condition = condition;
        node->node_tant_que.while_program = while_program;
        return node;
    }

    ASTNode* NodeTantQue_getCondition(NodeTantQue* node_tant_que) {
        if (node_tant_que == NULL) return NULL;
        return node_tant_que->condition;
    }

    ASTNode* NodeTantQue_getWhileProgram(NodeTantQue* node_tant_que) {
        if (node_tant_que == NULL) return NULL;
        return node_tant_que->while_program;
    }

    /**
     * @}
     */

    
    /**
     * @addtogroup node_affiche
     * @{
     */

    ASTNode* NodeAffiche_create(ASTNode* value) {
        if (value == NULL) return NULL;
        ASTNode* node = ASTNode_create(NODE_AFFICHE);
        node->node_affiche.value = value;
        return node;
    }

    ASTNode* NodeAffiche_getValue(NodeAffiche* node_affiche) {
        if (node_affiche == NULL) return NULL;
        return node_affiche->value;
    }

    /**
     * @}
     */

    
    /**
     * @addtogroup node_renvoi
     * @{
     */

    ASTNode* NodeRenvoi_create(ASTNode* value) {
        if (value == NULL) return NULL;
        ASTNode* node = ASTNode_create(NODE_RENVOI);
        node->node_renvoi.value = value;
        return node;
    }

    ASTNode* NodeRenvoi_getValue(NodeRenvoi* node_renvoi) {
        if (node_renvoi == NULL) return NULL;
        return node_renvoi->value;
    }

    /**
     * @}
     */


    /**
     * @addtogroup node_valeur
     * @{
     */

    /**
     * @brief Create a NodeValeur without a value initialized 
     * 
     * @param type The type of the value
     * @param identifier_name The name of the identifier assigned to the value. NULL if it's a raw value
     * @return A new NodeValeur with the value field empty
     * @retval - NULL : If `type` == NULL
     * @warning Exit the program if allocation failed because of ASTNode_create call
     */
    ASTNode* NodeValeur_createWithoutValue(char* type) {
        if (type == NULL) return NULL;
        ASTNode* node = ASTNode_create(NODE_VALEUR);
        node->node_valeur.type = string_copy(type);
        node->node_valeur.identifier_name = NULL;
        return node;
    }

    ASTNode* NodeValeur_createInt(int int_value) {
        ASTNode* node = NodeValeur_createWithoutValue("entier");
        if (node == NULL) return NULL;
        node->node_valeur.int_value = int_value;
        return node;
    }

    ASTNode* NodeValeur_createChar(char char_value) {
        ASTNode* node = NodeValeur_createWithoutValue("char");
        if (node == NULL) return NULL;
        node->node_valeur.char_value = char_value;
        return node;
    }

    ASTNode* NodeValeur_createString(char* string_value) {
        if (string_value == NULL) return NULL;
        ASTNode* node = NodeValeur_createWithoutValue("chaine");
        if (node == NULL) return NULL;
        node->node_valeur.string_value = string_copy(string_value);
        return node;
    }

    ASTNode* NodeValeur_createIdentifier(char *name) {
        if (name == NULL) return NULL;
        ASTNode* node = ASTNode_create(NODE_VALEUR);
        node->node_valeur.type = NULL;
        node->node_valeur.identifier_name = string_copy(name);
        return node;
    }

    char* NodeValeur_getType(NodeValeur* node_valeur) {
        if (node_valeur == NULL) return NULL;
        return node_valeur->type;
    }

    bool NodeValeur_isIdentifier(NodeValeur* node_valeur) {
        if (node_valeur == NULL) return false;
        return !(node_valeur->identifier_name == NULL);
    }

    char* NodeValeur_getIdentifierName(NodeValeur* node_valeur) {
        if (node_valeur == NULL) return NULL;
        return node_valeur->identifier_name;
    }

    int NodeValeur_getIntValue(NodeValeur* node_valeur) {
        return node_valeur->int_value;
    }

    char NodeValeur_getCharValue(NodeValeur* node_valeur) {
        return node_valeur->char_value;
    }

    char* NodeValeur_getStringValue(NodeValeur* node_valeur) {
        return node_valeur->string_value;
    }

    /**
     * @}
     */


    /**
     * @addtogroup node_parametres_appel
     * @{
     */

    ASTNode* NodeParametresAppel_create() {
        ASTNode* node = ASTNode_create(NODE_PARAMETRES_APPEL);
        ASTNode** values = (ASTNode**)malloc(sizeof(ASTNode*) * NODE_BASE_CAPACITY);
        if (values == NULL) {
            free(node);
            return NULL;
        }
        node->node_parametres_appel.values = values;
        node->node_parametres_appel.values_capacity = NODE_BASE_CAPACITY;
        node->node_parametres_appel.values_count = 0;
        return node;
    }

    ASTNode** NodeParametresAppel_getValues(NodeParametresAppel* node_parametres_appel) {
        if (node_parametres_appel == NULL) return NULL;
        return node_parametres_appel->values;
    }

    int NodeParametresAppel_getCapacity(NodeParametresAppel* node_parametres_appel) {
        if (node_parametres_appel == NULL) return -1;
        return node_parametres_appel->values_capacity;
    }

    int NodeParametresAppel_getCount(NodeParametresAppel* node_parametres_appel) {
        if (node_parametres_appel == NULL) return -1;
        return node_parametres_appel->values_count;
    }

    bool NodeParametresAppel_increaseCapacity(NodeParametresAppel* node_parametres_appel) {
        if (node_parametres_appel == NULL) return false;
        int new_capacity = NodeParametresAppel_getCapacity(node_parametres_appel) * 2;
        ASTNode** values = NodeParametresAppel_getValues(node_parametres_appel);
        if (values == NULL) return false;
        ASTNode** new_values = (ASTNode**)realloc(values, new_capacity);
        if (new_values == NULL) return false;
        node_parametres_appel->values = new_values;
        node_parametres_appel->values_capacity = new_capacity;
        return true;
    }

    bool NodeParametresAppel_increaseCount(NodeParametresAppel* node_parametres_appel) {
        if (node_parametres_appel == NULL) return false;
        node_parametres_appel->values_count++;
        return true;
    }

    bool NodeParametresAppel_addValue(NodeParametresAppel* node_parametres_appel, ASTNode* value) {
        if (node_parametres_appel == NULL || value == NULL) return false;
        ASTNode** values = NodeParametresAppel_getValues(node_parametres_appel);
        int capacity = NodeParametresAppel_getCapacity(node_parametres_appel);
        int count = NodeParametresAppel_getCount(node_parametres_appel);
        if (capacity == count) {
            if (!NodeParametresAppel_increaseCapacity(node_parametres_appel)) {
                fprintf(stderr, "Fatal error: reallocation failed.\n");
                exit(EXIT_FAILURE);
            }
        }
        values[count] = value;
        return NodeParametresAppel_increaseCount(node_parametres_appel);
    }

    /**
     * @}
     */

    
    /**
     * @addtogroup node_appel_fonction
     * @{
     */

    ASTNode* NodeAppelFonction_create(char* name, ASTNode* parameters) {
        if (name == NULL || parameters == NULL) return NULL;
        ASTNode* node = ASTNode_create(NODE_APPEL_FONCTION);
        node->node_appel_fonction.name = string_copy(name);
        node->node_appel_fonction.parameters = parameters;
        return node;
    }

    char* NodeAppelFonction_getName(NodeAppelFonction* node_appel_fonction) {
        if (node_appel_fonction == NULL) return NULL;
        return node_appel_fonction->name;
    }

    ASTNode* NodeAppelFonction_getParameters(NodeAppelFonction* node_appel_fonction) {
        if (node_appel_fonction == NULL) return NULL;
        return node_appel_fonction->parameters;
    }

    /**
     * @}
     */

    
    /**
     * @addtogroup node_expressions_binaires
     * @{
     */

    ASTNode* NodeExpressionsBinaires_create(char* expression_operator, ASTNode* left, ASTNode* right) {
        if (expression_operator == NULL || left == NULL || right == NULL) return NULL;
        ASTNode* node = ASTNode_create(NODE_EXPRESSIONS_BINAIRES);
        node->node_expressions_binaires.expression_operator = string_copy(expression_operator);
        node->node_expressions_binaires.left = left;
        node->node_expressions_binaires.right = right;
        return node;
    }

    char* NodeExpressionsBinaires_getExpressionOperator(NodeExpressionsBinaires* node_expressions_binaires) {
        if (node_expressions_binaires == NULL) return NULL;
        return node_expressions_binaires->expression_operator;
    }

    ASTNode* NodeExpressionsBinaires_getLeft(NodeExpressionsBinaires* node_expressions_binaires) {
        if (node_expressions_binaires == NULL) return NULL;
        return node_expressions_binaires->left;
    }

    ASTNode* NodeExpressionsBinaires_getRight(NodeExpressionsBinaires* node_expressions_binaires) {
        if (node_expressions_binaires == NULL) return NULL;
        return node_expressions_binaires->right;
    }

    /**
     * @}
     */

    // NodeExpressionsUnaires

    ASTNode* NodeExpressionsUnaires_create(char* expression_operator, ASTNode* condition) {
        if (expression_operator == NULL || condition == NULL) return NULL;
        ASTNode* node = ASTNode_create(NODE_EXPRESSIONS_UNAIRES);
        node->node_expressions_unaires.expression_operator = string_copy(expression_operator);
        node->node_expressions_unaires.condition = condition;
        return node;
    }

    char* NodeExpressionsUnaires_getExpressionOperator(NodeExpressionsUnaires* node_expressions_unaires) {
        if (node_expressions_unaires == NULL) return NULL;
        return node_expressions_unaires->expression_operator;
    }

    ASTNode* NodeExpressionsUnaires_getCondition(NodeExpressionsUnaires* node_expressions_unaires) {
        if (node_expressions_unaires == NULL) return NULL;
        return node_expressions_unaires->condition;
    }

    /**
     * @}
     */

/**
 * @}
 */

// <========================================================================>
// <========================================================================>
// <========================================================================>


// <========================================================================>
// <====================== Recursive Descent Parser ========================>
// <========================================================================>

ASTNode* Parser_parseCondition(Parser* parser);

ASTNode* Parser_parseDeclareVariable(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    Token* type = Parser_peek(parser);
    if (!Token_isAKeywordType(type)) {
        fprintf(stderr, "Syntax error: Expected type declaration, got %s. Line %d, Column %d\n", Token_getContent(type), Token_getLine(type), Token_getColumn(type));
        exit(EXIT_FAILURE);
    } else {
        Parser_advance(parser);
    }

    Token* identifiant = Parser_consume(parser, TOKEN_IDENTIFIANT);
    Token* affectation = Parser_consume(parser, TOKEN_AFFECTATION);
    (void)affectation;
    ASTNode* valeur = Parser_parseCondition(parser);
    Token* point_virgule = Parser_consume(parser, TOKEN_POINT_VIRGULE);
    (void)point_virgule;

    ASTNode* nodeDeclareVariable = NodeDeclareVariable_create(Token_getContent(type), Token_getContent(identifiant), valeur);
    if (nodeDeclareVariable == NULL) {
        fprintf(stderr, "Parsing error: The NodeDeclareVariable hasn't been well created\n");
        exit(EXIT_FAILURE);
    }
    return nodeDeclareVariable;
}

ASTNode* Parser_parseAppelFonction(Parser* parser);

ASTNode* Parser_parseValeur(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    Token* token = Parser_peek(parser);
    switch (Token_getTokenType(token)) {
        case TOKEN_ENTIER: {
            Parser_advance(parser);
            return NodeValeur_createInt(atoi(Token_getContent(token)));
            break;
        }
        case TOKEN_CHAR: {
            Parser_advance(parser);
            return NodeValeur_createChar(Token_getContent(token)[0]);
            break;
        }
        case TOKEN_CHAINE: {
            Parser_advance(parser);
            return NodeValeur_createString(Token_getContent(token));
            break;
        }
        case TOKEN_IDENTIFIANT: {
            Token* next_token = Parser_peekAt(parser, 1);
            if (Token_getTokenType(next_token) == TOKEN_PARENTHESE_GAUCHE) {
                return Parser_parseAppelFonction(parser);

            }
            Parser_advance(parser);
            return NodeValeur_createIdentifier(Token_getContent(token));
            break;
        }
        default: {
            fprintf(stderr, "Syntax error: Waited for value declaration, got %s. Line %d, Column %d\n", Token_getContent(token), Token_getLine(token), Token_getColumn(token));
            exit(EXIT_FAILURE);
        }
    }
}

ASTNode* Parser_parseAssignationVariable(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    Token* identifiant = Parser_consume(parser, TOKEN_IDENTIFIANT);
    Token* affectation = Parser_consume(parser, TOKEN_AFFECTATION);
    (void)affectation;
    ASTNode* valeur = Parser_parseCondition(parser);
    Token* point_virgule = Parser_consume(parser, TOKEN_POINT_VIRGULE);
    (void)point_virgule;

    ASTNode* nodeAssigneVariable = NodeAssigneVariable_create(Token_getContent(identifiant), valeur);
    if (nodeAssigneVariable == NULL) {
        fprintf(stderr, "Parsing error: The NodeAssigneVariable hasn't been well created\n");
        exit(EXIT_FAILURE);
    }
    return nodeAssigneVariable;
}

ASTNode* Parser_parseParametresFonction(Parser* parser);
ASTNode* Parser_parseProgramme(Parser* parser);
ASTNode* Parser_parseInstruction(Parser* parser);

ASTNode* Parser_parseDeclareFonction(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    Token* type = Parser_peek(parser);
    if (!Token_isAKeywordType(type)) {
        fprintf(stderr, "Syntax error: Expected type declaration, got %s. Line %d, Column %d\n", Token_getContent(type), Token_getLine(type), Token_getColumn(type));
        exit(EXIT_FAILURE);
    } else {
        Parser_advance(parser);
    }

    Token* identifiant = Parser_consume(parser, TOKEN_IDENTIFIANT);
    Token* parenthese_gauche = Parser_consume(parser, TOKEN_PARENTHESE_GAUCHE);
    (void)parenthese_gauche;
    ASTNode* parametres = Parser_parseParametresFonction(parser);
    Token* parenthese_droite = Parser_consume(parser, TOKEN_PARENTHESE_DROITE);
    (void)parenthese_droite;
    Token* accolade_gauche = Parser_consume(parser, TOKEN_ACCOLADE_GAUCHE);
    (void)accolade_gauche;
    ASTNode* programme = NodeProgramme_create();
    if (programme == NULL) {
        fprintf(stderr, "Parsing error: The NodeProgramme hasn't been well created\n");
        exit(EXIT_FAILURE);
    }
    while (Token_getTokenType(Parser_peek(parser)) != TOKEN_ACCOLADE_DROITE) {
        NodeProgramme_addInstruction(&programme->node_programme, Parser_parseInstruction(parser));
    }
    Token* accolade_droite = Parser_consume(parser, TOKEN_ACCOLADE_DROITE);
    (void)accolade_droite;

    ASTNode* nodeDeclareFonction = NodeDeclareFonction_create(Token_getContent(type), Token_getContent(identifiant), parametres, programme);
    if (nodeDeclareFonction == NULL) {
        fprintf(stderr, "Parsing error: The NodeDeclareFonction hasn't been well created\n");
        exit(EXIT_FAILURE);
    }
    return nodeDeclareFonction;
}

ASTNode* Parser_parseParametresFonction(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    ASTNode* parametresFonction = NodeParametresFonction_create();
    if (parametresFonction == NULL) return NULL;

    bool first_parameter = true;
    while (Token_getTokenType(Parser_peek(parser)) != TOKEN_PARENTHESE_DROITE) {
        if (first_parameter) {
            Token* type = Parser_peek(parser);
            if (!Token_isAKeywordType(type)) {
                fprintf(stderr, "Syntax error: Expected type declaration, got %s. Line %d, Column %d\n", Token_getContent(type), Token_getLine(type), Token_getColumn(type));
                exit(EXIT_FAILURE);
            } else {
                Parser_advance(parser);
            }

            Token* name = Parser_consume(parser, TOKEN_IDENTIFIANT);

            Parametre* parametre = Parametre_create(Token_getContent(type), Token_getContent(name));
            NodeParametresFonction_addParameter(&parametresFonction->node_parametres_fonction, parametre);

            first_parameter = false;
        } else {
            Token* virgule = Parser_consume(parser, TOKEN_VIRGULE);
            (void)virgule;
            Token* type = Parser_peek(parser);
            if (!Token_isAKeywordType(type)) {
                fprintf(stderr, "Syntax error: Expected type declaration, got %s. Line %d, Column %d\n", Token_getContent(type), Token_getLine(type), Token_getColumn(type));
                exit(EXIT_FAILURE);
            } else {
                Parser_advance(parser);
            }

            Token* name = Parser_consume(parser, TOKEN_IDENTIFIANT);

            Parametre* parametre = Parametre_create(Token_getContent(type), Token_getContent(name));
            NodeParametresFonction_addParameter(&parametresFonction->node_parametres_fonction, parametre);
        }
    }
    return parametresFonction;
}

ASTNode* Parser_parseSinon(Parser* parser);

ASTNode* Parser_parseSi(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    Token* keyword_si = Parser_consume(parser, TOKEN_SI);
    (void)keyword_si;
    Token* parenthese_gauche = Parser_consume(parser, TOKEN_PARENTHESE_GAUCHE);
    (void)parenthese_gauche;
    ASTNode* condition = Parser_parseCondition(parser);
    Token* parenthese_droite = Parser_consume(parser, TOKEN_PARENTHESE_DROITE);
    (void)parenthese_droite;
    Token* accolade_gauche = Parser_consume(parser, TOKEN_ACCOLADE_GAUCHE);
    (void)accolade_gauche;
    
    ASTNode* instructions = NodeProgramme_create();
    while (Token_getTokenType(Parser_peek(parser)) != TOKEN_ACCOLADE_DROITE) {
        NodeProgramme_addInstruction(&instructions->node_programme, Parser_parseInstruction(parser));
    }
    Token* accolade_droite = Parser_consume(parser, TOKEN_ACCOLADE_DROITE);
    (void)accolade_droite;

    ASTNode* sinon = NULL;
    bool has_else_condition = false;
    if (Token_getTokenType(Parser_peek(parser)) == TOKEN_SINON) {
        sinon = Parser_parseSinon(parser);
        has_else_condition = true;
    }

    ASTNode* si = NodeSi_create(condition, instructions, sinon, has_else_condition);
    if (si == NULL) {
        fprintf(stderr, "Parsing error: The NodeSi hasn't been well created\n");
        exit(EXIT_FAILURE);
    }
    return si;
}

ASTNode* Parser_parseSinon(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    Token* sinon = Parser_consume(parser, TOKEN_SINON);
    (void)sinon;
    Token* accolade_gauche = Parser_consume(parser, TOKEN_ACCOLADE_GAUCHE);
    (void)accolade_gauche;

    ASTNode* instructions = NodeProgramme_create();
    if (instructions == NULL) {
        fprintf(stderr, "Parsing error: The NodeProgramme hasn't been well created\n");
        exit(EXIT_FAILURE);
    }
    while (Token_getTokenType(Parser_peek(parser)) != TOKEN_ACCOLADE_DROITE) {
        NodeProgramme_addInstruction(&instructions->node_programme, Parser_parseInstruction(parser));
    }
    Token* accolade_droite = Parser_consume(parser, TOKEN_ACCOLADE_DROITE);
    (void)accolade_droite;
    return instructions;
}

ASTNode* Parser_parsePrimaire(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    Token* token = Parser_peek(parser);
    if (Token_getTokenType(token) == TOKEN_PARENTHESE_GAUCHE) {
        Parser_advance(parser);
        ASTNode* condition = Parser_parseCondition(parser);
        token = Parser_consume(parser, TOKEN_PARENTHESE_DROITE);
        (void)token;
        return condition;
    } else {
        return Parser_parseValeur(parser);
    }
}

ASTNode* Parser_parseUnaire(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    Token* token = Parser_peek(parser);
    if (Token_getTokenType(token) == TOKEN_NON || Token_getTokenType(token) == TOKEN_MOINS) {
        char* expression_operator = Token_getContent(token);
        Parser_advance(parser);
        ASTNode* exp_unaire = NodeExpressionsUnaires_create(expression_operator, Parser_parseUnaire(parser));
        if (exp_unaire == NULL) {
            fprintf(stderr, "Parsing error: The NodeExpressionsUnaires hasn't been well created\n");
            exit(EXIT_FAILURE);
        }
        return exp_unaire;
    } else {
        return Parser_parsePrimaire(parser);
    }
}

ASTNode* Parser_parseMultiplication(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    ASTNode* gauche = Parser_parseUnaire(parser);

    while (Parser_peek(parser) != NULL) {
        Token* operator = Parser_peek(parser);
        TokenType operator_type = Token_getTokenType(operator);

        if (operator_type == TOKEN_FOIS || operator_type == TOKEN_DIVISE || operator_type == TOKEN_RESTE) {
            Parser_advance(parser);
            ASTNode* droite = Parser_parseUnaire(parser);

            gauche = NodeExpressionsBinaires_create(Token_getContent(operator), gauche, droite);
            if (gauche == NULL) {
                fprintf(stderr, "Parsing error: Failed to create binary node\n");
                exit(EXIT_FAILURE);
            }
        } else {
            break;
        }
    }

    return gauche;
}

ASTNode* Parser_parseAddition(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    ASTNode* gauche = Parser_parseMultiplication(parser);

    while (Parser_peek(parser) != NULL) {
        Token* operator = Parser_peek(parser);
        TokenType operator_type = Token_getTokenType(operator);

        if (operator_type == TOKEN_PLUS || operator_type == TOKEN_MOINS) {
            Parser_advance(parser);
            ASTNode* droite = Parser_parseMultiplication(parser);

            gauche = NodeExpressionsBinaires_create(Token_getContent(operator), gauche, droite);
            if (gauche == NULL) {
                fprintf(stderr, "Parsing error: Failed to create binary node\n");
                exit(EXIT_FAILURE);
            }
        } else {
            break;
        }
    }

    return gauche;
}

ASTNode* Parser_parseComparaison(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    ASTNode* gauche = Parser_parseAddition(parser);

    while (Parser_peek(parser) != NULL) {
        Token* operator = Parser_peek(parser);
        TokenType operator_type = Token_getTokenType(operator);

        if (operator_type == TOKEN_INFERIEUR || operator_type == TOKEN_INFERIEUR_EGAL || operator_type == TOKEN_SUPERIEUR || operator_type == TOKEN_SUPERIEUR_EGAL) {
            Parser_advance(parser);
            ASTNode* droite = Parser_parseAddition(parser);

            gauche = NodeExpressionsBinaires_create(Token_getContent(operator), gauche, droite);
            if (gauche == NULL) {
                fprintf(stderr, "Parsing error: Failed to create binary node\n");
                exit(EXIT_FAILURE);
            }
        } else {
            break;
        }
    }

    return gauche;
}

ASTNode* Parser_parseEgalite(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    ASTNode* gauche = Parser_parseComparaison(parser);

    while (Parser_peek(parser) != NULL) {
        Token* operator = Parser_peek(parser);
        TokenType operator_type = Token_getTokenType(operator);

        if (operator_type == TOKEN_EGAL || operator_type == TOKEN_DIFFERENT) {
            Parser_advance(parser);
            ASTNode* droite = Parser_parseComparaison(parser);

            gauche = NodeExpressionsBinaires_create(Token_getContent(operator), gauche, droite);
            if (gauche == NULL) {
                fprintf(stderr, "Parsing error: Failed to create binary node\n");
                exit(EXIT_FAILURE);
            }
        } else {
            break;
        }
    }

    return gauche;
}

ASTNode* Parser_parseEtBinaire(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    ASTNode* gauche = Parser_parseEgalite(parser);

    while (Parser_peek(parser) != NULL) {
        Token* operator = Parser_peek(parser);
        TokenType operator_type = Token_getTokenType(operator);

        if (operator_type == TOKEN_ET_BINAIRE) {
            Parser_advance(parser);
            ASTNode* droite = Parser_parseEgalite(parser);

            gauche = NodeExpressionsBinaires_create(Token_getContent(operator), gauche, droite);
            if (gauche == NULL) {
                fprintf(stderr, "Parsing error: Failed to create binary node\n");
                exit(EXIT_FAILURE);
            }
        } else {
            break;
        }
    }

    return gauche;
}

ASTNode* Parser_parseOuBinaire(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    ASTNode* gauche = Parser_parseEtBinaire(parser);

    while (Parser_peek(parser) != NULL) {
        Token* operator = Parser_peek(parser);
        TokenType operator_type = Token_getTokenType(operator);

        if (operator_type == TOKEN_OU_BINAIRE) {
            Parser_advance(parser);
            ASTNode* droite = Parser_parseEtBinaire(parser);

            gauche = NodeExpressionsBinaires_create(Token_getContent(operator), gauche, droite);
            if (gauche == NULL) {
                fprintf(stderr, "Parsing error: Failed to create binary node\n");
                exit(EXIT_FAILURE);
            }
        } else {
            break;
        }
    }

    return gauche;
}

ASTNode* Parser_parseEtLogique(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    ASTNode* gauche = Parser_parseOuBinaire(parser);

    while (Parser_peek(parser) != NULL) {
        Token* operator = Parser_peek(parser);
        TokenType operator_type = Token_getTokenType(operator);

        if (operator_type == TOKEN_ET) {
            Parser_advance(parser);
            ASTNode* droite = Parser_parseOuBinaire(parser);

            gauche = NodeExpressionsBinaires_create(Token_getContent(operator), gauche, droite);
            if (gauche == NULL) {
                fprintf(stderr, "Parsing error: Failed to create binary node\n");
                exit(EXIT_FAILURE);
            }
        } else {
            break;
        }
    }

    return gauche;
}

ASTNode* Parser_parseOuLogique(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    ASTNode* gauche = Parser_parseEtLogique(parser);

    while (Parser_peek(parser) != NULL) {
        Token* operator = Parser_peek(parser);
        TokenType operator_type = Token_getTokenType(operator);

        if (operator_type == TOKEN_OU) {
            Parser_advance(parser);
            ASTNode* droite = Parser_parseEtLogique(parser);

            gauche = NodeExpressionsBinaires_create(Token_getContent(operator), gauche, droite);
            if (gauche == NULL) {
                fprintf(stderr, "Parsing error: Failed to create binary node\n");
                exit(EXIT_FAILURE);
            }
        } else {
            break;
        }
    }

    return gauche;
}

ASTNode* Parser_parseCondition(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    return Parser_parseOuLogique(parser);
}

ASTNode* Parser_parseTantQue(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    Token* tant_que = Parser_consume(parser, TOKEN_TANT_QUE);
    (void)tant_que;
    Token* parenthese_gauche = Parser_consume(parser, TOKEN_PARENTHESE_GAUCHE);
    (void)parenthese_gauche;
    ASTNode* condition = Parser_parseCondition(parser);
    Token* parenthese_droite = Parser_consume(parser, TOKEN_PARENTHESE_DROITE);
    (void)parenthese_droite;
    Token* accolade_gauche = Parser_consume(parser, TOKEN_ACCOLADE_GAUCHE);
    (void)accolade_gauche;
    ASTNode* programme = NodeProgramme_create();
    if (programme == NULL) {
        fprintf(stderr, "Parsing error: The NodeProgramme hasn't been well created\n");
        exit(EXIT_FAILURE);
    }
    while (Token_getTokenType(Parser_peek(parser)) != TOKEN_ACCOLADE_DROITE) {
        NodeProgramme_addInstruction(&programme->node_programme, Parser_parseInstruction(parser));
    }
    Token* accolade_droite = Parser_consume(parser, TOKEN_ACCOLADE_DROITE);
    (void)accolade_droite;
    ASTNode* tantque = NodeTantQue_create(condition, programme);
    if (tantque == NULL) {
        fprintf(stderr, "Parsing error: The NodeTantQue hasn't been well created\n");
        exit(EXIT_FAILURE);
    }
    return tantque;
}

ASTNode* Parser_parseAffiche(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    Token* affiche_keyword = Parser_consume(parser, TOKEN_AFFICHE);
    (void)affiche_keyword;
    Token* parenthese_gauche = Parser_consume(parser, TOKEN_PARENTHESE_GAUCHE);
    (void)parenthese_gauche;
    ASTNode* valeur = Parser_parseCondition(parser);
    Token* parenthese_droite = Parser_consume(parser, TOKEN_PARENTHESE_DROITE);
    (void)parenthese_droite;
    Token* point_virgule = Parser_consume(parser, TOKEN_POINT_VIRGULE);
    (void)point_virgule;
    ASTNode* affiche = NodeAffiche_create(valeur);
    if (affiche == NULL) {
        fprintf(stderr, "Parsing error: The NodeAffiche hasn't been well created\n");
        exit(EXIT_FAILURE);
    }
    return affiche;
}

ASTNode* Parser_parseRenvoi(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    Token* renvoi_keyword = Parser_consume(parser, TOKEN_RENVOI);
    (void)renvoi_keyword;
    Token* parenthese_gauche = Parser_consume(parser, TOKEN_PARENTHESE_GAUCHE);
    (void)parenthese_gauche;
    ASTNode* valeur = Parser_parseCondition(parser);
    Token* parenthese_droite = Parser_consume(parser, TOKEN_PARENTHESE_DROITE);
    (void)parenthese_droite;
    Token* point_virgule = Parser_consume(parser, TOKEN_POINT_VIRGULE);
    (void)point_virgule;
    ASTNode* renvoi = NodeRenvoi_create(valeur);
    if (renvoi == NULL) {
        fprintf(stderr, "Parsing error: The NodeRenvoi hasn't been well created\n");
        exit(EXIT_FAILURE);
    }
    return renvoi;
}

ASTNode* Parser_parseParametresAppel(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    ASTNode* parametresAppel = NodeParametresAppel_create();
    if (parametresAppel == NULL) return NULL;

    bool first_parameter = true;
    while (Token_getTokenType(Parser_peek(parser)) != TOKEN_PARENTHESE_DROITE) {
        if (first_parameter) {
            ASTNode* valeur = Parser_parseCondition(parser);
            NodeParametresAppel_addValue(&parametresAppel->node_parametres_appel, valeur);
            first_parameter = false;
        } else {
            Token* virgule = Parser_consume(parser, TOKEN_VIRGULE);
            (void)virgule;
            ASTNode* valeur = Parser_parseCondition(parser);
            NodeParametresAppel_addValue(&parametresAppel->node_parametres_appel, valeur);
        }
    }
    return parametresAppel;
}

ASTNode* Parser_parseAppelFonction(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    Token* identifiant = Parser_consume(parser, TOKEN_IDENTIFIANT);
    Token* parenthese_gauche = Parser_consume(parser, TOKEN_PARENTHESE_GAUCHE);
    (void)parenthese_gauche;
    ASTNode* parametres = Parser_parseParametresAppel(parser);
    Token* parenthese_droite = Parser_consume(parser, TOKEN_PARENTHESE_DROITE);
    (void)parenthese_droite;
    ASTNode* appel = NodeAppelFonction_create(Token_getContent(identifiant), parametres);
    if (appel == NULL) {
        fprintf(stderr, "Parsing error: The NodeAppelFonction hasn't been well created\n");
        exit(EXIT_FAILURE);
    }
    return appel;
}

ASTNode* Parser_parseInstruction(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }

    Token* current = Parser_peek(parser);

    switch (Token_getTokenType(current)) {
        case TOKEN_IDENTIFIANT: {
            Token* next = Parser_peekAt(parser, 1);
            if (next != NULL && Token_getTokenType(next) == TOKEN_PARENTHESE_GAUCHE) {
                ASTNode* call_node = Parser_parseAppelFonction(parser);
                Token* point_virgule = Parser_consume(parser, TOKEN_POINT_VIRGULE);
                (void)point_virgule;
                return call_node;
            }
            return Parser_parseAssignationVariable(parser);
        }

        case TOKEN_SI: {
            return Parser_parseSi(parser);
        }

        case TOKEN_TANT_QUE: {
            return Parser_parseTantQue(parser);
        }

        case TOKEN_AFFICHE: {
            return Parser_parseAffiche(parser);
        }

        case TOKEN_RENVOI: {
            return Parser_parseRenvoi(parser);
        }

        default:
            if (Token_isAKeywordType(current)) {
                Token* afterIdent = Parser_peekAt(parser, 2);
                if (afterIdent != NULL && Token_getTokenType(afterIdent) == TOKEN_PARENTHESE_GAUCHE) {
                    return Parser_parseDeclareFonction(parser);
                }
                return Parser_parseDeclareVariable(parser);
            }
            fprintf(stderr, "Syntax error: unexpected token '%s' at line %d, column %d\n", Token_getContent(current), Token_getLine(current), Token_getColumn(current));
            exit(EXIT_FAILURE);
    }
}

ASTNode* Parser_parseProgramme(Parser* parser) {
    if (parser == NULL) {
        fprintf(stderr, "Parsing error: The parser is null\n");
        exit(EXIT_FAILURE);
    }
    
    ASTNode* programme = NodeProgramme_create();
    if (programme == NULL) return NULL;

    while (Parser_peek(parser) != NULL && Token_getTokenType(Parser_peek(parser)) != TOKEN_FDF) {
        NodeProgramme_addInstruction(&programme->node_programme, Parser_parseInstruction(parser));
    }
    return programme;
}

// <========================================================================>
// <========================================================================>
// <========================================================================>


// <========================================================================>
// <========================== Debug & Free Zone ===========================>
// <========================================================================>

/**
 * @addtogroup debug_free_zone
 * @{
 */

bool ASTNode_free(ASTNode* node) {
    if (node == NULL) return true;
    bool boolean = true;

    switch (node->type) {
        case NODE_PROGRAMME: {
            for (int i = 0; i < node->node_programme.instructions_count; i++) {
                boolean = boolean && ASTNode_free(node->node_programme.instructions[i]);
            }
            free(node->node_programme.instructions);
            break;
        }
        case NODE_DECLARE_VARIABLE: {
            free(node->node_declare_variable.name);
            free(node->node_declare_variable.type);
            boolean = ASTNode_free(node->node_declare_variable.value);
            break;
        }
        case NODE_ASSIGNE_VARIABLE: {
            free(node->node_assigne_variable.name);
            boolean = ASTNode_free(node->node_assigne_variable.value);
            break;
        }
        case NODE_PARAMETRES_FONCTION: {
            for (int i = 0; i < node->node_parametres_fonction.parameters_count; i++) {
                Parametre* parameter = node->node_parametres_fonction.parameters[i];
                free(parameter->name);
                free(parameter->type);
                free(parameter);
            }
            free(node->node_parametres_fonction.parameters);
            break;
        }
        case NODE_DECLARE_FONCTION: {
            free(node->node_declare_fonction.type);
            free(node->node_declare_fonction.name);
            boolean = ASTNode_free(node->node_declare_fonction.parameters) && ASTNode_free(node->node_declare_fonction.function_program);
            break;
        }
        case NODE_SI: {
            boolean = ASTNode_free(node->node_si.condition) && ASTNode_free(node->node_si.then_program) && ASTNode_free(node->node_si.else_instruction);
            break;
        }
        case NODE_TANT_QUE: {
            boolean = ASTNode_free(node->node_tant_que.condition) && ASTNode_free(node->node_tant_que.while_program);
            break;
        }
        case NODE_AFFICHE: {
            boolean = ASTNode_free(node->node_affiche.value);
            break;
        }
        case NODE_RENVOI: {
            boolean = ASTNode_free(node->node_renvoi.value);
            break;
        }
        case NODE_VALEUR: {
            if (node->node_valeur.type != NULL && strcmp(node->node_valeur.type, "chaine") == 0) free(node->node_valeur.string_value);
            free(node->node_valeur.type);
            free(node->node_valeur.identifier_name);
            break;
        }
        case NODE_PARAMETRES_APPEL: {
            for (int i = 0; i < node->node_parametres_appel.values_count; i++) {
                boolean = boolean && ASTNode_free(node->node_parametres_appel.values[i]);
            }
            free(node->node_parametres_appel.values);
            break;
        }
        case NODE_APPEL_FONCTION: {
            free(node->node_appel_fonction.name);
            boolean = ASTNode_free(node->node_appel_fonction.parameters);
            break;
        }
        case NODE_EXPRESSIONS_BINAIRES: {
            free(node->node_expressions_binaires.expression_operator);
            boolean = ASTNode_free(node->node_expressions_binaires.left) && ASTNode_free(node->node_expressions_binaires.right);
            break;
        }
        case NODE_EXPRESSIONS_UNAIRES: {
            free(node->node_expressions_unaires.expression_operator);
            boolean = ASTNode_free(node->node_expressions_unaires.condition);
            break;
        }
    }
    free(node);
    return boolean;
}

/**
 * @}
 */


static void print_indent(FILE *out, int depth) {
    for (int i = 0; i < depth; i++) {
        fprintf(out, "  ");
    }
}

void print_ast(const ASTNode *node, FILE *out, int depth) {
    if (node == NULL) {
        print_indent(out, depth);
        fprintf(out, "(null)\n");
        return;
    }

    print_indent(out, depth);

    switch (node->type) {
        case NODE_PROGRAMME: {
            const NodeProgramme *prog = &node->node_programme;
            fprintf(out, "Programme (count: %d)\n", prog->instructions_count);
            for (int i = 0; i < prog->instructions_count; i++) {
                print_ast(prog->instructions[i], out, depth + 1);
            }
            break;
        }

        case NODE_DECLARE_VARIABLE: {
            const NodeDeclareVariable *var = &node->node_declare_variable;
            fprintf(out, "DeclareVariable: %s %s\n", var->type ? var->type : "?", var->name ? var->name : "?");
            if (var->value) {
                print_ast(var->value, out, depth + 1);
            }
            break;
        }

        case NODE_ASSIGNE_VARIABLE: {
            const NodeAssigneVariable *assign = &node->node_assigne_variable;
            fprintf(out, "AssignVariable: %s\n", assign->name ? assign->name : "?");
            if (assign->value) {
                print_ast(assign->value, out, depth + 1);
            }
            break;
        }

        case NODE_PARAMETRES_FONCTION: {
            const NodeParametresFonction *params = &node->node_parametres_fonction;
            fprintf(out, "ParametresFonction (count: %d)\n", params->parameters_count);
            for (int i = 0; i < params->parameters_count; i++) {
                print_indent(out, depth + 1);
                fprintf(out, "Param: %s %s\n", 
                        params->parameters[i]->type ? params->parameters[i]->type : "?", 
                        params->parameters[i]->name ? params->parameters[i]->name : "?");
            }
            break;
        }

        case NODE_DECLARE_FONCTION: {
            const NodeDeclareFonction *fn = &node->node_declare_fonction;
            fprintf(out, "DeclareFonction: %s %s()\n", fn->type ? fn->type : "?", fn->name ? fn->name : "?");
            if (fn->parameters) {
                print_indent(out, depth + 1);
                fprintf(out, "[Parameters]\n");
                print_ast(fn->parameters, out, depth + 2);
            }
            if (fn->function_program) {
                print_indent(out, depth + 1);
                fprintf(out, "[Body]\n");
                print_ast(fn->function_program, out, depth + 2);
            }
            break;
        }

        case NODE_SI: {
            const NodeSi *si = &node->node_si;
            fprintf(out, "Si (If)\n");
            print_indent(out, depth + 1);
            fprintf(out, "[Condition]\n");
            print_ast(si->condition, out, depth + 2);

            print_indent(out, depth + 1);
            fprintf(out, "[Then]\n");
            print_ast(si->then_program, out, depth + 2);

            if (si->has_else_instruction && si->else_instruction) {
                print_indent(out, depth + 1);
                fprintf(out, "[Else]\n");
                print_ast(si->else_instruction, out, depth + 2);
            }
            break;
        }

        case NODE_TANT_QUE: {
            const NodeTantQue *tq = &node->node_tant_que;
            fprintf(out, "TantQue (While)\n");
            print_indent(out, depth + 1);
            fprintf(out, "[Condition]\n");
            print_ast(tq->condition, out, depth + 2);

            print_indent(out, depth + 1);
            fprintf(out, "[Body]\n");
            print_ast(tq->while_program, out, depth + 2);
            break;
        }

        case NODE_AFFICHE: {
            fprintf(out, "Affiche\n");
            print_ast(node->node_affiche.value, out, depth + 1);
            break;
        }

        case NODE_RENVOI: {
            fprintf(out, "Renvoi (Return)\n");
            print_ast(node->node_renvoi.value, out, depth + 1);
            break;
        }

        case NODE_VALEUR: {
            const NodeValeur *val = &node->node_valeur;
            if (val->type && strcmp(val->type, "entier") == 0) {
                fprintf(out, "Valeur (entier): %d\n", val->int_value);
            } else if (val->type && strcmp(val->type, "char") == 0) {
                fprintf(out, "Valeur (char): '%c'\n", val->char_value);
            } else if (val->type && strcmp(val->type, "chaine") == 0) {
                fprintf(out, "Valeur (chaine): \"%s\"\n", val->string_value ? val->string_value : "");
            } else if (val->identifier_name) {
                fprintf(out, "Valeur (id): %s\n", val->identifier_name);
            } else {
                fprintf(out, "Valeur (unknown/raw)\n");
            }
            break;
        }

        case NODE_PARAMETRES_APPEL: {
            const NodeParametresAppel *args = &node->node_parametres_appel;
            fprintf(out, "ParametresAppel (count: %d)\n", args->values_count);
            for (int i = 0; i < args->values_count; i++) {
                print_ast(args->values[i], out, depth + 1);
            }
            break;
        }

        case NODE_APPEL_FONCTION: {
            const NodeAppelFonction *call = &node->node_appel_fonction;
            fprintf(out, "AppelFonction: %s()\n", call->name ? call->name : "?");
            if (call->parameters) {
                print_ast(call->parameters, out, depth + 1);
            }
            break;
        }

        case NODE_EXPRESSIONS_BINAIRES: {
            const NodeExpressionsBinaires *bin = &node->node_expressions_binaires;
            fprintf(out, "BinaryOp: %s\n", bin->expression_operator ? bin->expression_operator : "?");
            print_ast(bin->left, out, depth + 1);
            print_ast(bin->right, out, depth + 1);
            break;
        }

        case NODE_EXPRESSIONS_UNAIRES: {
            const NodeExpressionsUnaires *un = &node->node_expressions_unaires;
            fprintf(out, "UnaryOp: %s\n", un->expression_operator ? un->expression_operator : "?");
            print_ast(un->condition, out, depth + 1);
            break;
        }

        default:
            fprintf(out, "Unknown node type: %d\n", node->type);
            break;
    }
}

// <========================================================================>
// <========================================================================>
// <========================================================================>
