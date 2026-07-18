#ifndef TOKEN
#define TOKEN

#define TOKEN_ARRAY_CAPACITY_BASE 256

typedef enum _TokenType {
    TOKEN_ENTIER,
    TOKEN_CHAR,
    TOKEN_VIDE,
    TOKEN_CHAINE,
    TOKEN_IDENTIFIANT,
    TOKEN_SI,
    TOKEN_SINON,
    TOKEN_TANT_QUE,
    TOKEN_RENVOI,
    TOKEN_AFFECTATION,
    TOKEN_PLUS,
    TOKEN_MOINS,
    TOKEN_FOIS,
    TOKEN_DIVISE,
    TOKEN_EGAL,
    TOKEN_DIFFERENT,
    TOKEN_INFERIEUR,
    TOKEN_INFERIEUR_EGAL,
    TOKEN_SUPERIEUR,
    TOKEN_SUPERIEUR_EGAL,
    TOKEN_ET,
    TOKEN_OU,
    TOKEN_PARENTHESE_GAUCHE,
    TOKEN_PARENTHESE_DROITE,
    TOKEN_ACCOLADE_GAUCHE,
    TOKEN_ACCOLADE_DROITE,
    TOKEN_POINT_VIRGULE,
    TOKEN_VIRGULE,
    TOKEN_ERREUR,
    TOKEN_EOF
} TokenType;

typedef struct _Token {
    TokenType token_type;
    char* content;
} Token;

typedef struct _TokenArray {
    Token* tokens;
    int capacity;
    int count;
} TokenArray;

#endif