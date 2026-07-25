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
 * @defgroup ParserStruct
 * @brief Functions and struct definition to handle the TokenArray and the position in it
 * @ingroup Parser
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

/** @} */

// <========================================================================>
// <========================================================================>
// <========================================================================>


// <========================================================================>
// <========================== AST Constructors ============================>
// <========================================================================>

/**
 * @defgroup AST Constructors
 * @brief All the functions about the nodes of the AST
 * @ingroup Parser
 * @{
 */


    /**
     * @defgroup NodeProgramme
     * @ingroup AST Constructors
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
     * @defgroup NodeDeclareVariable
     * @ingroup AST Constructors
     * @brief The Node assigned to declare variables
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
     * @brief Retrive the value inside the object
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



// <========================================================================>
// <========================================================================>
// <========================================================================>

#endif