/**
 * @file Parser_functions.h
 * @brief Declaration of useful functions for the Parser to retrieve TokenArray from the Lexer and create a dedicated AST
 * @author Rosol Simon
 * @date 2026-07-24
 */

#ifndef PARSER
#define PARSER

#include <string.h>

#include "ASTNode.h"
#include "../Lexer/Lexer_functions.h"

#define NODE_BASE_CAPACITY 100

// <========================================================================>
// <=========================== Token Management ===========================>
// <========================================================================>

/**
 * @defgroup parser_struct ParserStruct
 * @brief Functions and struct definition to handle the TokenArray and the position in it
 * @ingroup parser
 * @{
 */

typedef struct _Parser {
    TokenArray* tokens;
    int current_index;
} Parser;

/**
 * @brief Create a new Parser object
 * 
 * @param tokens The TokenArray retrieved from the source_code
 * @return The Parser object created
 * @retval NULL : If `tokens` == NULL
 */
Parser* Parser_create(TokenArray* tokens);

/**
 * @brief Retrieve the TokenArray from the Parser object
 * 
 * @param parser The Parser object
 * @return The TokenArray in it
 * @retval NULL : If `parser` == NULL
 */
TokenArray* Parser_getTokenArray(Parser* parser);

/**
 * @brief Retrieve the position in the TokenArray
 * 
 * @param parser The Parser object
 * @return The position in the parsing
 * @retval -1 : If `parser` == NULL
 */
int Parser_getCurrentIndex(Parser* parser);

/**
 * @brief Allow to peek the Token at an `offset` distance from the current position
 * 
 * @param parser The Parser object
 * @param offset The offset to add to the current position
 * @return The Token seen
 * @retval NULL : If `parser` == NULL, `offset` < 0 or when trying to access further the token array
 */
Token* Parser_peekAt(Parser* parser, int offset);

/**
 * @brief Allow to peek the Token at the current position
 * 
 * @param parser The Parser object
 * @return The Token seen
 * @retval NULL : If `parser` == NULL
 */
Token* Parser_peek(Parser* parser);

/**
 * @brief Increment the cursor of the position of 1
 * 
 * @param parser The Parser object
 * @return A boolean about the success or not of the operation
 * @retval false : If `parser` == NULL
 */
bool Parser_increment(Parser* parser);

/**
 * @brief Advance with security in the TokenArray
 * 
 * @param parser The Parser object
 * @return A boolean about the success or not of the operation
 * @retval false : If `parser` == NULL or the parser is already at the end of the TokenArray
 */
bool Parser_advance(Parser *parser);

/**
 * @brief Retrieve the token at position if expected and safely advance in the Parser
 * 
 * @param parser The Parser object
 * @param expected_token_type The expected type of the Token at position
 * @return The Token at position
 * @retval NULL : If `parser` == NULL
 * @warning Exit the program if the type of the token isn't the one expected, and display an error message
 */
Token* Parser_consume(Parser* parser, TokenType expected_token_type);

/**
 * @brief Free the parser struct
 * 
 * @param parser The Parser object
 * @return A boolean about the state of the operation
 */
bool Parser_free(Parser* parser);

/** @} */

// <========================================================================>
// <========================================================================>
// <========================================================================>


// <========================================================================>
// <========================== AST Constructors ============================>
// <========================================================================>

/**
 * @defgroup ast_constructors AST Constructors
 * @brief All the functions about the nodes of the AST
 * @ingroup parser
 * @{
 */


    /**
     * @defgroup node_programme NodeProgramme
     * @ingroup ast_constructors
     * @brief The Node containing a list of instructions, reusable
     * @{
     */

    /**
     * @brief Create a NodeProgramme object
     * 
     * @return A new NodeProgramme object with an empty array
     * @retval NULL : If allocation of instructions array failed
     * @warning Can exit the program because of ASTNode_create call if allocation fail
     */
    ASTNode* NodeProgramme_create();

    /**
     * @brief Retrieve the instructions array from the node_programme
     * 
     * @param node_programme The NodeProgramme object
     * @return The instructions array in the node_programme
     * @retval NULL : If `node_programme` == NULL
     */
    ASTNode** NodeProgramme_getInstructions(NodeProgramme* node_programme);

    /**
     * @brief Retrieve the capacity of the instructions array
     * 
     * @param node_programme The NodeProgramme object
     * @return The capacity of the instructions array
     * @retval -1 : If `node_programme` == NULL
     */
    int NodeProgramme_getInstructionsCapacity(NodeProgramme* node_programme);

    /**
     * @brief Retrieve the number of entries in the instructions array
     * 
     * @param node_programme The NodeProgramme object
     * @return The count property of the object
     * @retval -1 : If `node_programme` == NULL
     */
    int NodeProgramme_getInstructionsCount(NodeProgramme* node_programme);

    /**
     * @brief Increase dynamically the capacity of the instructions array
     * 
     * @param node_programme The NodeProgramme object
     * @return A boolean about the success or not of the operation
     * @retval false : If `node_programme` == NULL, the reallocation failed or the actual instructions array is NULL
     */
    bool NodeProgramme_increaseCapacity(NodeProgramme* node_programme);

    /**
     * @brief Increase the count property of the object
     * 
     * @param node_programme The NodeProgramme object
     * @return A boolean about the success or not of the operation
     * @retval false : If `node_programme` == NULL
     */
    bool NodeProgramme_increaseCount(NodeProgramme* node_programme);

    /**
     * @brief Add a new instruction to the instructions array
     * 
     * @param node_programm The NodeProgramm object
     * @param instruction The new instruction
     * @return A boolean about the success or not of the operation
     * @retval false : If `node_programme` == NULL, `instruction` == NULL or the operation of increasing count failed
     * @warning Exit the program if the increase capacity operation failed and display an error message
     */
    bool NodeProgramme_addInstruction(NodeProgramme* node_programme, ASTNode* instruction);

    /** @} */


    /**
     * @defgroup node_declare_variable NodeDeclareVariable
     * @ingroup ast_constructors
     * @brief The node designed to declare variables
     * @{
     */

    /**
     * @brief Create a NodeDeclareVariable object
     * 
     * @param type The type of the variable
     * @param name The name of the variable
     * @param value The value to store in the variable
     * @return A new NodeDeclareVariable object
     * @retval NULL : If `type` == NULL, `name` == NULL or `value` == NULL
     * @warning Exit the program if allocation failed because of ASTNode_create call
     */
    ASTNode* NodeDeclareVariable_create(char* type, char* name, ASTNode* value);

    /**
     * @brief Retrieve the type of the variable
     * 
     * @param node_declare_variable The NodeDeclareVariable object
     * @return The type of the variable
     * @retval NULL : If `node_declare_variable` == NULL
     */
    char* NodeDeclareVariable_getType(NodeDeclareVariable* node_declare_variable);

    /**
     * @brief Retrieve the name of the variable
     * 
     * @param node_declare_variable The NodeDeclareVariable object
     * @return The name of the variable
     * @retval NULL : If `node_declare_variable` == NULL
     */
    char* NodeDeclareVariable_getName(NodeDeclareVariable* node_declare_variable);

    /**
     * @brief Retrieve the value inside the object
     * 
     * @param node_declare_variable The NodeDeclareVariable object
     * @return The value stored in the variable
     * @retval NULL : If `node_declare_variable` == NULL
     */
    ASTNode* NodeDeclareVariable_getValue(NodeDeclareVariable* node_declare_variable);

    /**
     * @}
     */

    /**
     * @defgroup node_assigne_variable NodeAssigneVariable
     * @ingroup ast_constructors
     * @brief The node designed to assign variables
     * @{
     */

     /**
      * @brief Create a NodeAssigneVariable object
      * 
      * @param name Name of the variable
      * @param value Value of the variable
      * @return The newly created object
      * @retval - NULL : If `name` == NULL or `value` == NULL
      * @warning Exit the program if allocation failed because of ASTNode_create call
      */
    ASTNode* NodeAssigneVariable_create(char* name, ASTNode* value);

    /**
     * @brief Retrieve the name inside the NodeAssigneVariable object
     * 
     * @param node_assigne_variable The NodeAssigneVariable object
     * @return The name of the variable
     * @retval - NULL : If `node_assigne_variable` == NULL
     */
    char* NodeAssigneVariable_getName(NodeAssigneVariable* node_assigne_variable);

    /**
     * @brief Retrieve the value inside the NodeAssigneVariable object
     * 
     * @param node_assigne_variable The NodeAssigneVariable object
     * @return The value of the variable
     * @retval - NULL : If `node_assigne_variable` == NULL
     */
    ASTNode* NodeAssigneVariable_getValue(NodeAssigneVariable* node_assigne_variable);

    /**
     * @}
     */

    /**
     * @defgroup node_parametres_fonction NodeParametreFonction
     * @ingroup ast_constructors
     * @brief The node designed to store the parameters of a function declaration
     * @{
     */

    /**
     * @brief Create a new NodeParametresFonction object
     * 
     * @return The newly created NodeParametresFonction object
     * @retval - NULL : If the parametres allocation failed
     * @warning Exit the program if allocation failed because of ASTNode_create call
     */
    ASTNode* NodeParametresFonction_create();

    /**
     * @brief Create a new Parametre object
     * 
     * @param type The type of the parameter
     * @param name The name of the parameter in the function
     * @return A new Parameter object
     * @retval - NULL : If `type` == NULL, `name` == NULL or the allocation failed
     */
    Parametre* Parametre_create(char* type, char* name);

    /**
     * @brief Return the Parametre array inside the NodeParametresFonction
     * 
     * @param node_parameteres_fonction The NodeParametresFonction object
     * @return The Parametre array
     * @retval - NULL : If `node_parametres_fonction` == NULL
     */
    Parametre** NodeParametresFonction_getParameters(NodeParametresFonction* node_parametres_fonction);

    /**
     * @brief Return the capacity of the Parametre array
     * 
     * @param node_parametres_fonction The NodeParametresFonction object
     * @return The capacity property inside the given object
     * @retval - NULL : If `node_parametres_fonction` == NULL
     */
    int NodeParametresFonction_getCapacity(NodeParametresFonction* node_parametres_fonction);

    /**
     * @brief Return the count of the Parametre array
     * 
     * @param node_parametres_fonction The NodeParametresFonction object
     * @return The count property inside the given object
     * @retval - NULL : If `node_parametres_fonction` == NULL
     */
    int NodeParametresFonction_getCount(NodeParametresFonction* node_parametres_fonction);

    /**
     * @brief Increase the capacity of the Parametre array
     * 
     * @param node_parametres_fonction The NodeParametresFonction object
     * @return A boolean about the success or not of the operation
     * @retval - false : If `node_parametres_fonction` == NULL, the Parametre array is NULL or the reallocation failed
     */
    bool NodeParametresFonction_increaseCapacity(NodeParametresFonction* node_parametres_fonction);

    /**
     * @brief Increase the count of the Parametre array
     * 
     * @param node_parametres_fonction The NodeParametresFonction object
     * @return A boolean about the success or not of the operation
     * @retval - false : If `node_parametres_fonction` == NULL
     */
    bool NodeParametresFonction_increaseCount(NodeParametresFonction* node_parametres_fonction);

    /**
     * @brief Add a new parameter to the Parametre array
     * 
     * @param node_parametres_fonction The NodeParametresFonction object
     * @param parametre The Parametre to add
     * @return A boolean about the success or not of the operation
     * @retval - false : If `node_parametres_fonction` == NULL, `parametre` == NULL or the Parametre array is NULL
     * @warning Exit and print an error message if the increase of capacity failed
     */
    bool NodeParametresFonction_addParameter(NodeParametresFonction* node_parametres_fonction, Parametre* parametre);

    /**
     * @}
     */

    /**
     * @defgroup node_declare_fonction NodeDeclareFonction
     * @ingroup ast_constructors
     * @brief The node designed to declare a function
     * @{
     */

    /**
     * @brief Create a new NodeDeclareFonction object
     * 
     * @param type The type of the function return
     * @param name The identifier of the function
     * @param parameters The Parametre array
     * @param function_program The program the function is supposed to execute
     * @return The newly created NodeDeclareFonction object
     * @retval - NULL : If `type` == NULL, `name` == NULL, `parameters` == NULL or `function_program` == NULL
     * @warning Exit the program if allocation failed because of ASTNode_create call
     */
    ASTNode* NodeDeclareFonction_create(char* type, char* name, ASTNode* parameters, ASTNode* function_program);

    /**
     * @brief Retrieve the type of the function return
     * 
     * @param node_declare_fonction The NodeDeclareFonction object
     * @return The type property inside the given object
     * @retval - NULL : If `node_declare_fonction` == NULL
     */
    char* NodeDeclareFonction_getType(NodeDeclareFonction* node_declare_fonction);

    /**
     * @brief Retrieve the name of the function
     * 
     * @param node_declare_fonction The NodeDeclareFonction object
     * @return The name property inside the given object
     * @retval - NULL : If `node_declare_fonction` == NULL
     */
    char* NodeDeclareFonction_getName(NodeDeclareFonction* node_declare_fonction);

    /**
     * @brief Retrieve the Parametre array of the function
     * 
     * @param node_declare_fonction The NodeDeclareFonction object
     * @return The Parametre array inside the given object
     * @retval - NULL : If `node_declare_fonction` == NULL
     */
    ASTNode* NodeDeclareFonction_getParameters(NodeDeclareFonction* node_declare_fonction);

    /**
     * @brief Retrieve the program of the function
     * 
     * @param node_declare_fonction The NodeDeclareFonction object
     * @return The program inside the given object
     * @retval - NULL : If `node_declare_fonction` == NULL
     */
    ASTNode* NodeDeclareFonction_getFunctionProgram(NodeDeclareFonction* node_declare_fonction);

    /**
     * @}
     */

    /**
     * @defgroup node_si NodeSi
     * @ingroup ast_constructors
     * @brief The node designed to store a 'si ... sinon ...' structure
     * @{
     */

    /**
     * @brief Create a new NodeSi object
     * 
     * @param condition The condition for the 'si ...' to execute
     * @param then_program The program in case of validation of the condition
     * @param else_instruction The program in case of invalidation of the condition
     * @param has_else_instruction Boolean to debug the `else_instruction` == NULL case
     * @return The newly created NodeSi object
     * @retval - NULL : If `condition` == NULL, `then_programm` == NULL or (else_instruction == NULL && has_else_instruction)
     * @warning Exit the program if allocation failed because of ASTNode_create call
     */
    ASTNode* NodeSi_create(ASTNode* condition, ASTNode* then_program, ASTNode* else_program, bool has_else_program);

    /**
     * @brief Retrieve the condition in the NodeSi
     * 
     * @param node_si The NodeSi object
     * @return The condition object inside the NodeSi
     * @retval - NULL : If `node_si` == NULL
     */
    ASTNode* NodeSi_getCondition(NodeSi* node_si);

    /**
     * @brief Retrieve the then_program in the NodeSi
     * 
     * @param node_si The NodeSi object
     * @return The then_program object inside the NodeSi
     * @retval - NULL : If `node_si` == NULL
     */
    ASTNode* NodeSi_getThenProgram(NodeSi* node_si);

    /**
     * @brief Return if a NodeSi has an else_program
     * 
     * @param node_si The NodeSi object
     * @return A boolean about the existence of an else_program
     * @retval - false : If `node_si` == NULL
     */
    bool NodeSi_hasElseProgram(NodeSi* node_si);

    /**
     * @brief Retrieve the else_program in the NodeSi
     * 
     * @param node_si The NodeSi object
     * @return The else_program object inside the NodeSi
     * @retval - NULL : If `node_si` == NULL
     */
    ASTNode* NodeSi_getElseProgram(NodeSi* node_si);

    /**
     * @}
     */

    /**
     * @defgroup node_tant_que NodeTantQue
     * @ingroup ast_constructors
     * @brief The node designed to store a 'tantque ...' structure
     * @{
     */

    /**
     * @brief Create a new NodeTantQue object
     * 
     * @param condition The condition to evaluate
     * @param while_program The program to execute
     * @return The newly created NodeTantQue object
     * @retval - NULL : If `condition` == NULL or `while_program` == NULL
     * @warning Exit the program if allocation failed because of ASTNode_create call
     */
    ASTNode* NodeTantQue_create(ASTNode* condition, ASTNode* while_program);

    /**
     * @brief Retrieve the condition in the NodeTantQue object
     * 
     * @param node_tant_que The NodeTantQue object
     * @return The condition inside the given object
     * @retval - NULL : If `node_tant_que` == NULL
     */
    ASTNode* NodeTantQue_getCondition(NodeTantQue* node_tant_que);

    /**
     * @brief Retrieve the while_program in the NodeTantQue object
     * 
     * @param node_tant_que The NodeTantQue object
     * @retval - NULL : If `node_tant_que` == NULL
     */
    ASTNode* NodeTantQue_getWhileProgram(NodeTantQue* node_tant_que);

    /**
     * @}
     */

    /**
     * @defgroup node_affiche NodeAffiche
     * @ingroup ast_constructors
     * @brief The node designed to store what's supposed to be displayed
     * @{
     */

    /**
     * @brief Create a new NodeAffiche object
     * 
     * @param value The value to display
     * @return The newly created NodeAffiche object
     * @retval - NULL : If `value` == NULL
     * @warning Exit the program if allocation failed because of ASTNode_create call
     */
    ASTNode* NodeAffiche_create(ASTNode* value);

    /**
     * @brief Retrieve the value to display
     * 
     * @param node_affiche The NodeAffiche object
     * @return The value inside the NodeAffiche object
     * @retval - NULL : If `node_affiche` == NULL
     */
    ASTNode* NodeAffiche_getValue(NodeAffiche* node_affiche);

    /**
     * @}
     */

    /**
     * @defgroup node_renvoi NodeRenvoi
     * @ingroup ast_constructors
     * @brief The node designed to handle the return of a function
     * @{
     */

    /**
     * @brief Create a new NodeRenvoi object
     * @param value The value to return
     * @return The newly created NodeRenvoi object
     * @retval - NULL : If `value` == NULL
     * @warning Exit the program if allocation failed because of ASTNode_create call
     */
    ASTNode* NodeRenvoi_create(ASTNode* value);

    /**
     * @brief Retrieve the value to return
     * @param node_renvoi The NodeRenvoi object
     * @return The value inside the NodeRenvoi object
     * @retval - NULL : If `node_renvoi` == NULL
     */
    ASTNode* NodeRenvoi_getValue(NodeRenvoi* node_renvoi);

    /**
     * @}
     */

    /**
     * @defgroup node_valeur NodeValeur
     * @ingroup ast_constructors
     * @brief The node designed to store a value
     * @{
     */

    /**
     * @brief Create a new NodeValeur object with a int inside
     * 
     * @param int_value The value
     * @return The newly created NodeValeur object
     * @retval - NULL : If the allocation of the raw node failed
     */
    ASTNode* NodeValeur_createInt(int int_value);

    /**
     * @brief Create a new NodeValeur object with a char inside
     * 
     * @param char_value The value
     * @return The newly created NodeValeur object
     * @retval - NULL : If the allocation of the raw node failed
     */
    ASTNode* NodeValeur_createChar(char char_value);

    /**
     * @brief Create a new NodeValeur object with a string inside
     * 
     * @param string_value The value
     * @return The newly created NodeValeur object
     * @retval - NULL : If `string_value` == NULL or the allocation of the raw node failed
     */
    ASTNode* NodeValeur_createString(char* string_value);

    /**
     * @brief Create a new NodeValeur object with an identifier inside
     * 
     * @param name The name of the identifier
     * @return The newly created NodeValeur object
     * @retval - NULL : If `name` == NULL
     */
    ASTNode* NodeValeur_createIdentifier(char *name);

    /**
     * @brief Retrieve the type of the value
     * 
     * @param node_valeur The NodeValeur object
     * @return The type of the value stored in the given object
     * @retval - NULL : If `node_valeur` == NULL
     */
    char* NodeValeur_getType(NodeValeur* node_valeur);

    /**
     * @brief Tell if a NodeValeur is a raw value or linked to a variable
     * 
     * @param node_valeur The NodeValeur object
     * @return A boolean about whether the NodeValeur is a variable or not
     * @retval - false : If `node_valeur` == NULL
     */
    bool NodeValeur_isIdentifier(NodeValeur* node_valeur);

    /**
     * @brief Retrieve the name of the identifier inside the NodeValeur object
     * 
     * @param node_valeur The NodeValeur object
     * @return The name of the variable
     * @retval - NULL : If `node_valeur` == NULL
     */
    char* NodeValeur_getIdentifierName(NodeValeur* node_valeur);

    /**
     * @brief Retrieve the value of an int NodeValeur
     * 
     * @param node_valeur The NodeValeur object
     * @return The value inside the given object
     */
    int NodeValeur_getIntValue(NodeValeur* node_valeur);

    /**
     * @brief Retrieve the value of a char NodeValeur
     * 
     * @param node_valeur The NodeValeur object
     * @return The value inside the given object
     */
    char NodeValeur_getCharValue(NodeValeur* node_valeur);

    /**
     * @brief Retrieve the value of a string NodeValeur
     * 
     * @param node_valeur The NodeValeur object
     * @return The value inside the given object
     */
    char* NodeValeur_getStringValue(NodeValeur* node_valeur);

    /**
     * @}
     */

    /**
     * @defgroup node_parametres_appel NodeParametresAppel
     * @ingroup ast_constructors
     * @brief The node designed to store the parameters inside a parameters call
     * @{
     */

    /**
     * @brief Create a new NodeParametresAppel object
     * 
     * @return The newly created NodeParametresAppel object
     * @retval - NULL : If the values array allocation failed
     * @warning Exit the program if allocation failed because of ASTNode_create call
     */
    ASTNode* NodeParametresAppel_create();

    /**
     * @brief Retrieve the values array in the function call
     * 
     * @param node_parametres_appel The NodeParametresAppel object
     * @return The values array inside the given object
     * @retval - NULL : If `node_parametres_appel` == NULL
     */
    ASTNode** NodeParametresAppel_getValues(NodeParametresAppel* node_parametres_appel);

    /**
     * @brief Retrieve the capacity of the values array
     * 
     * @param node_parametres_appel The NodeParametresAppel object
     * @return The capacity property of the given object
     * @retval - -1 : If `node_parametres_appel` == NULL
     */
    int NodeParametresAppel_getCapacity(NodeParametresAppel* node_parametres_appel);

    /**
     * @brief Retrieve the number of entries in the values array
     * 
     * @param node_parametres_appel The NodeParametresAppel object
     * @return The count property of the given object
     * @retval - -1 : If `node_parametres_appel` == NULL
     */
    int NodeParametresAppel_getCount(NodeParametresAppel* node_parametres_appel);

    /**
     * @brief Increase the capacity of the values array
     * 
     * @param node_parametres_appel The NodeParametresAppel object
     * @return A boolean about the success or not of the operation
     * @retval - false : If `node_parametres_appel` == NULL, the values array is NULL or the reallocation failed
     */
    bool NodeParametresAppel_increaseCapacity(NodeParametresAppel* node_parametres_appel);

    /**
     * @brief Increase the count of the values array
     * 
     * @param node_parametres_appel The NodeParametresAppel object
     * @return A boolean about the success or not of the operation
     * @retval - false : If `node_parametres_apple` == NULL
     */
    bool NodeParametresAppel_increaseCount(NodeParametresAppel* node_parametres_appel);

    /**
     * @brief Add a new value to the values array
     * 
     * @param node_parametres_appel The NodeParametresAppel object
     * @param value The new value to append
     * @return A boolean about the success of the operation or not
     * @retval - false : If `node_parametres_appel` == NULL, `value` == NULL or the addition failed
     * @warning Exit and print an error message if the increase of capacity failed
     */
    bool NodeParametresAppel_addValue(NodeParametresAppel* node_parametres_appel, ASTNode* value);

    /**
     * @}
     */

    /**
     * @defgroup node_appel_fonction NodeAppelFonction
     * @ingroup ast_constructors
     * @brief The node designed to handle function calls
     * @{
     */

    /**
     * @brief Create a new NodeAppelFonction object
     * 
     * @param name The name of the function called
     * @param parameters The parameters in the function call
     * @return The newly created NodeAppelFonction object
     * @retval - NULL : If `name` == NULL or `parameters` == NULL
     * @warning Exit the program if allocation failed because of ASTNode_create call
     */
    ASTNode* NodeAppelFonction_create(char* name, ASTNode* parameters);

    /**
     * @brief Retrieve the name of the function called
     * 
     * @param node_appele_fonction The NodeAppelFonction object
     * @return The name of the function inside the given object
     * @retval - NULL : If `node_appel_fonction` == NULL
     */
    char* NodeAppelFonction_getName(NodeAppelFonction* node_appel_fonction);

    /**
     * @brief Retrieve the parameters array inside the node
     * 
     * @param node_appel_fonction The NodeAppelFonction object
     * @return The parameters array inside the given object
     * @retval - NULL : If `node_appel_fonction` == NULL
     */
    ASTNode* NodeAppelFonction_getParameters(NodeAppelFonction* node_appel_fonction);

    /**
     * @}
     */

    /**
     * @defgroup node_expressions_binaires NodeExpressionsBinaires
     * @ingroup ast_constructors
     * @brief The node designed to store binary operations
     * @{
     */

    /**
     * @brief Create a new NodeExpressionsBinaires object
     * 
     * @param expression_operator The operator between the two conditions
     * @param left The left condition
     * @param right The right condition
     * @return The newly created NodeExpressionsBinaires object
     * @retval - NULL : If `expression_operator` == NULL, `left` == NULL or `right` == NULL
     * @warning Exit the program if allocation failed because of ASTNode_create call
     */
    ASTNode* NodeExpressionsBinaires_create(char* expression_operator, ASTNode* left, ASTNode* right);

    /**
     * @brief Retrieve the expression operator of the binary operation
     * 
     * @param node_expressions_binaires The NodeExpressionsBinaires object
     * @return The string representing the operator
     * @retval - NULL : If `node_expressions_binaires` == NULL
     */
    char* NodeExpressionsBinaires_getExpressionOperator(NodeExpressionsBinaires* node_expressions_binaires);

    /**
     * @brief Retrieve the left part of the binary operation
     * 
     * @param node_expressions_binaires The NodeExpressionsBinaires object
     * @return The left property of the given object
     * @retval - NULL : If `node_expressions_binaires` == NULL
     */
    ASTNode* NodeExpressionsBinaires_getLeft(NodeExpressionsBinaires* node_expressions_binaires);

    /**
     * @brief Retrieve the right part of the binary operation
     * 
     * @param node_expressions_binaires The NodeExpressionsBinaires object
     * @return The right property of the given object
     * @retval - NULL : If `node_expressions_binaires` == NULL
     */
    ASTNode* NodeExpressionsBinaires_getRight(NodeExpressionsBinaires* node_expressions_binaires);

    /**
     * @}
     */

    /**
     * @defgroup node_expressions_unaires NodeExpressionsUnaires
     * @ingroup ast_constructors
     * @brief The node designed to store unary operations
     * @{
     */

    /**
     * @brief Create a new NodeExpressionsUnaires object
     * 
     * @param expression_operator The operator on the condition
     * @param condition The condition on what act the operator
     * @return The newly created NodeExpressionUnaires object
     * @retval - NULL : If `expression_operator` == NULL or `condition` == NULL
     * @warning Exit the program if allocation failed because of ASTNode_create call
     */
    ASTNode* NodeExpressionsUnaires_create(char* expression_operator, ASTNode* condition);

    /**
     * @brief Retrieve the operator of the unary operation
     * 
     * @param node_expressions_unaires The NodeExpressionsUnaires object
     * @return The string representing the operator
     * @retval - NULL : If `node_expressions_unaires` == NULL
     */
    char* NodeExpressionsUnaires_getExpressionOperator(NodeExpressionsUnaires* node_expressions_unaires);

    /**
     * @brief Retrieve the condition of the unary operation
     * 
     * @param node_expressions_unaires The NodeExpressionsUnaires object
     * @return The condition object inside the given object
     * @retval - NULL : If `node_expressions_unaires`
     */
    ASTNode* NodeExpressionsUnaires_getCondition(NodeExpressionsUnaires* node_expressions_unaires);

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

ASTNode* Parser_parseProgramme(Parser* parser);

// <========================================================================>
// <========================================================================>
// <========================================================================>


// <========================================================================>
// <========================== Debug & Free Zone ===========================>
// <========================================================================>

/**
 * @defgroup debug_free_zone Debug & Free Zone
 * @ingroup parser
 * @brief The functions needed in order to correctly free all the ASTNodes from a tree
 * @{
 */

/**
 * @brief Free an ASTNode. Recursive function
 * 
 * @param node The ASTNode to free
 * @return A boolean about the success or not of the operation
 * @retval - false : If one operation failed during the process
 * @warning If a process fail during the operation, it could impact the rest. Check your memory gestion in that case
 */
bool ASTNode_free(ASTNode* node);

void print_ast(const ASTNode *node, FILE *out, int depth);

/**
 * @}
 */


// <========================================================================>
// <========================================================================>
// <========================================================================>

#endif