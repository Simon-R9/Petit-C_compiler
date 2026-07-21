#ifndef TOKEN
#define TOKEN

#define TOKEN_ARRAY_CAPACITY_BASE 256
#define DYNAMIC_STRING_CAPACITY_BASE 32

typedef enum _TokenType {
    TOKEN_ENTIER, // entier
    TOKEN_CHAR, // char
    TOKEN_VIDE, // vide
    TOKEN_CHAINE, // chaine
    TOKEN_IDENTIFIANT, 
    TOKEN_SI, // si
    TOKEN_SINON, // sinon
    TOKEN_TANT_QUE, // tantque
    TOKEN_RENVOI, // renvoi
    TOKEN_AFFICHE, // affiche
    TOKEN_AFFECTATION, // =
    TOKEN_PLUS, // +
    TOKEN_MOINS, // -
    TOKEN_FOIS, // *
    TOKEN_DIVISE, // /
    TOKEN_RESTE, // %
    TOKEN_EGAL, // ==
    TOKEN_DIFFERENT, // !=
    TOKEN_INFERIEUR, // <
    TOKEN_INFERIEUR_EGAL, // <=
    TOKEN_SUPERIEUR, // >
    TOKEN_SUPERIEUR_EGAL, // >=
    TOKEN_ET_BINAIRE, // &
    TOKEN_OU_BINAIRE, // |
    TOKEN_ET, // &&
    TOKEN_OU, // ||
    TOKEN_NON, // !
    TOKEN_PARENTHESE_GAUCHE, // (
    TOKEN_PARENTHESE_DROITE, // )
    TOKEN_ACCOLADE_GAUCHE, // {
    TOKEN_ACCOLADE_DROITE, // }
    TOKEN_POINT_VIRGULE, // ;
    TOKEN_VIRGULE, // ,
    TOKEN_ANTI_SLASH, // '\'
    TOKEN_ERREUR, // erreur
    TOKEN_FDF // fin (FDF = Fin De Fichier)
} TokenType;

typedef struct _Token {
    TokenType token_type;
    char* content;
} Token;

typedef struct _TokenArray {
    Token** tokens;
    int capacity;
    int count;
} TokenArray;

typedef struct _DynamicString {
    char* string;
    int capacity;
    int count;
} DynamicString;

#endif