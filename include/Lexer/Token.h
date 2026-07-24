/**
 * @file Token.h
 * @brief Structures of the tokens and token array to parse the source code
 * @author Rosol Simon
 * @date 2026-07-17
 */

#ifndef TOKEN
#define TOKEN

/**
 * @defgroup Lexer
 * @brief All the functions to help the lexer to parse the source code
 * @{
 */

    /**
     * @defgroup Token and TokenArray structures
     * @brief The Token and TokenArray structures in order to tokenize the source code
     * @ingroup Lexer
     * @{
     */
    #define TOKEN_ARRAY_CAPACITY_BASE 256

    typedef enum _TokenType {
        TOKEN_MOTCLE_ENTIER, // entier
        TOKEN_ENTIER,
        TOKEN_MOTCLE_CHAR, // char
        TOKEN_CHAR,
        TOKEN_VIDE, // vide
        TOKEN_MOTCLE_CHAINE, // chaine
        TOKEN_CHAINE,
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
        TOKEN_ERREUR,
        TOKEN_FDF // fin (FDF = Fin De Fichier)
    } TokenType;

    typedef struct _Token {
        TokenType token_type;
        char* content;
        int line;
        int column;
    } Token;

    typedef struct _TokenArray {
        Token** tokens;
        int capacity;
        int count;
    } TokenArray;

    /** @} */

/** @} */

#endif