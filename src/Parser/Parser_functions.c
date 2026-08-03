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
    ASTNode* NodeValeur_createWithoutValue(char* type, char* identifier_name) {
        if (type == NULL) return NULL;
        ASTNode* node = ASTNode_create(NODE_VALEUR);
        node->node_valeur.type = string_copy(type);
        node->node_valeur.identifier_name = string_copy(identifier_name);
        return node;
    }

    ASTNode* NodeValeur_createInt(char* type, char* identifier_name, int int_value) {
        if (type == NULL) return NULL;
        ASTNode* node = NodeValeur_createWithoutValue(type, identifier_name);
        if (node == NULL) return NULL;
        node->node_valeur.int_value = int_value;
        return node;
    }

    ASTNode* NodeValeur_createChar(char* type, char* identifier_name, char char_value) {
        if (type == NULL) return NULL;
        ASTNode* node = NodeValeur_createWithoutValue(type, identifier_name);
        if (node == NULL) return NULL;
        node->node_valeur.char_value = char_value;
        return node;
    }

    ASTNode* NodeValeur_createString(char* type, char* identifier_name, char* string_value) {
        if (type == NULL || string_value == NULL) return NULL;
        ASTNode* node = NodeValeur_createWithoutValue(type, identifier_name);
        if (node == NULL) return NULL;
        node->node_valeur.string_value = string_value;
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

    // NodeParametresAppel

    ASTNode* NodeParametresAppel_create(){
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

    // NodeAppelFonction

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

    // NodeExpressionsBinaires

    ASTNode* NodeExpressionBinaire_create(char* expression_operator, ASTNode* left, ASTNode* right) {
        if (expression_operator == NULL || left == NULL || right == NULL) return NULL;
        ASTNode* node = ASTNode_create(NODE_EXPRESSIONS_BINAIRES);
        node->node_expressions_binaires.expression_operator = string_copy(expression_operator);
        node->node_expressions_binaires.left = left;
        node->node_expressions_binaires.right = right;
        return node;
    }

    char* NodeExpressionBinaire_getExpressionOperator(NodeExpressionsBinaires* node_expression_binaires) {
        if (node_expression_binaires == NULL) return NULL;
        return node_expression_binaires->expression_operator;
    }

    ASTNode* NodeExpressionsBinaires_getLeft(NodeExpressionsBinaires* node_expressions_binaires) {
        if (node_expressions_binaires == NULL) return NULL;
        return node_expressions_binaires->left;
    }

    ASTNode* NodeExpressionsBinaires_getRight(NodeExpressionsBinaires* node_expression_binaires) {
        if (node_expression_binaires == NULL) return NULL;
        return node_expression_binaires->right;
    }

    // NodeExpressionsUnaires

    ASTNode* NodeExpressionsUnaires_create(char* expression_operator, ASTNode* condition) {
        if (expression_operator == NULL || condition == NULL) return NULL;
        ASTNode* node = ASTNode_create(NODE_EXPRESSIONS_UNAIRES);
        node->node_expressions_unaires.expression_operator = string_copy(expression_operator);
        node->node_expressions_unaires.condition = condition;
        return node;
    }

    char* NodeExpressionsUnaire_getExpressionOperator(NodeExpressionsUnaires* node_expressions_unaires) {
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

// <========================================================================>
// <========================================================================>
// <========================================================================>
