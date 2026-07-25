/**
 * @file ASTNode.h
 * @brief Structures of the nodes to construct the AST after token reading
 * @author Rosol Simon
 * @date 2026-07-24
 */

#ifndef ASTNODE
#define ASTNODE

#include <stdbool.h>

typedef struct _ASTNode ASTNode;

typedef struct _NodeProgramme {
    ASTNode** instructions;
    int instructions_capacity;
    int instructions_count;
} NodeProgramme;

typedef struct _NodeDeclareVariable {
    char* type;
    char* name;
    ASTNode* value;
} NodeDeclareVariable;

typedef struct _NodeAssigneVariable {
    char* name;
    ASTNode* value;
} NodeAssigneVariable;

typedef struct _Parametre {
    char* type;
    char* name;
} Parametre;

typedef struct _NodeParametresFonction {
    Parametre** parameters;
    int parameters_capacity;
    int parameters_count;
} NodeParametresFonction;

typedef struct _NodeDeclareFonction {
    char* type;
    char* name;
    ASTNode* parameters;
    ASTNode* function_program;
} NodeDeclareFonction;

typedef struct _NodeSi {
    ASTNode* condition;
    ASTNode* then_program;
    ASTNode* else_instruction;
    bool has_else_instruction;
} NodeSi;

typedef struct _NodeTantQue {
    ASTNode* condition;
    ASTNode* while_program;
} NodeTantQue;

typedef struct _NodeAffiche {
    ASTNode* value;
} NodeAffiche;

typedef struct _NodeRenvoi {
    ASTNode* value;
} NodeRenvoi;

typedef struct _NodeValeur {
    char* type;
    char* identifier_name;
    union {
        int int_value;
        char char_value;
        char* string_value;
    };
} NodeValeur;

typedef struct _NodeParametresAppel {
    ASTNode** values;
    int values_capacity;
    int values_count;
} NodeParametresAppel;

typedef struct _NodeAppelFonction {
    char* name;
    ASTNode* parameters;
} NodeAppelFonction;

typedef struct _NodeExpressionsBinaires {
    char* expression_operator;
    ASTNode* left;
    ASTNode* right;
} NodeExpressionsBinaires;

typedef struct _NodeExpressionsUnaires {
    char* expression_operator;
    ASTNode* condition;
} NodeExpressionsUnaires;

typedef enum _NodeType {
    NODE_PROGRAMME,
    NODE_DECLARE_VARIABLE,
    NODE_ASSIGNE_VARIABLE,
    NODE_PARAMETRES_FONCTION,
    NODE_DECLARE_FONCTION,
    NODE_SI,
    NODE_TANT_QUE,
    NODE_AFFICHE,
    NODE_RENVOI,
    NODE_VALEUR,
    NODE_PARAMETRES_APPEL,
    NODE_APPEL_FONCTION,
    NODE_EXPRESSIONS_BINAIRES,
    NODE_EXPRESSIONS_UNAIRES,
} NodeType;

struct _ASTNode {
    NodeType type;
    union {
        NodeProgramme node_programme;
        NodeDeclareVariable node_declare_variable;
        NodeAssigneVariable node_assigne_variable;
        NodeParametresFonction node_parametres_fonction;
        NodeDeclareFonction node_declare_fonction;
        NodeSi node_si;
        NodeTantQue node_tant_que;
        NodeAffiche node_affiche;
        NodeRenvoi node_renvoi;
        NodeValeur node_valeur;
        NodeParametresAppel node_parametres_appel;
        NodeAppelFonction node_appel_fonction;
        NodeExpressionsBinaires node_expressions_binaires;
        NodeExpressionsUnaires node_expressions_unaires;
    };
};

#endif