#include "Lexer_functions.h"

char* read_file(const char* file_path) {
    if (file_path == NULL) return NULL;

    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("Error : Impossible to open %s.\n", file_path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);

    rewind(file);

    char* buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL) {
        printf("Error : Memory allocation failed.\n");
        fclose(file);
        return NULL;
    }

    size_t elements_read = fread(buffer, 1, file_size, file);
    if (elements_read != (size_t)file_size) {
        printf("Error : Problem encountered during file reading.\n");
        free(buffer);
        return NULL;
    }

    buffer[file_size] = '\0';
    fclose(file);
    return buffer;
}

char* token_name(TokenType token_type) {
    switch (token_type) {
        case TOKEN_ENTIER:           return "TOKEN_ENTIER";
        case TOKEN_CHAR:             return "TOKEN_CHAR";
        case TOKEN_VIDE:             return "TOKEN_VIDE";
        case TOKEN_CHAINE:           return "TOKEN_CHAINE";
        case TOKEN_IDENTIFIANT:      return "TOKEN_IDENTIFIANT";
        case TOKEN_SI:               return "TOKEN_SI";
        case TOKEN_SINON:            return "TOKEN_SINON";
        case TOKEN_TANT_QUE:         return "TOKEN_TANT_QUE";
        case TOKEN_RENVOI:           return "TOKEN_RENVOI";
        case TOKEN_AFFECTATION:      return "TOKEN_AFFECTATION";
        case TOKEN_PLUS:             return "TOKEN_PLUS";
        case TOKEN_MOINS:            return "TOKEN_MOINS";
        case TOKEN_FOIS:             return "TOKEN_FOIS";
        case TOKEN_DIVISE:           return "TOKEN_DIVISE";
        case TOKEN_RESTE:            return "TOKEN_RESTE";
        case TOKEN_EGAL:             return "TOKEN_EGAL";
        case TOKEN_DIFFERENT:        return "TOKEN_DIFFERENT";
        case TOKEN_INFERIEUR:        return "TOKEN_INFERIEUR";
        case TOKEN_INFERIEUR_EGAL:   return "TOKEN_INFERIEUR_EGAL";
        case TOKEN_SUPERIEUR:        return "TOKEN_SUPERIEUR";
        case TOKEN_SUPERIEUR_EGAL:   return "TOKEN_SUPERIEUR_EGAL";
        case TOKEN_ET_BINAIRE:       return "TOKEN_ET_BINAIRE";
        case TOKEN_ET:               return "TOKEN_ET";
        case TOKEN_OU_BINAIRE:       return "TOKEN_OU_BINAIRE";
        case TOKEN_OU:               return "TOKEN_OU";
        case TOKEN_NON:              return "TOKEN_NON";
        case TOKEN_PARENTHESE_GAUCHE:return "TOKEN_PARENTHESE_GAUCHE";
        case TOKEN_PARENTHESE_DROITE:return "TOKEN_PARENTHESE_DROITE";
        case TOKEN_ACCOLADE_GAUCHE:  return "TOKEN_ACCOLADE_GAUCHE";
        case TOKEN_ACCOLADE_DROITE:  return "TOKEN_ACCOLADE_DROITE";
        case TOKEN_POINT_VIRGULE:    return "TOKEN_POINT_VIRGULE";
        case TOKEN_VIRGULE:          return "TOKEN_VIRGULE";
        case TOKEN_GUILLEMET:        return "TOKEN_GUILLEMET";
        case TOKEN_ERREUR:           return "TOKEN_ERREUR";
        case TOKEN_FDF:              return "TOKEN_FDF";
        default:                     return "TOKEN_INCONNU";
    }
}

void save_tokens(const char* exit_file, TokenArray* token_array) {
    FILE* file = fopen(exit_file, "w");
    if (file == NULL) {
        printf("Error : Impossible to open %s.\n", exit_file);
        return;
    }

    for (int i = 0; i < token_array->count; i++) {
        Token* t = token_array->tokens[i];
        fprintf(file, " [%s, %s] ", token_name(t->token_type), t->content);
    }
    fclose(file);
    return;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage : %s <file_path.petitC>\n", argv[0]);
        return EXIT_FAILURE;
    }
    const char* file_path = argv[1];
    char* source_code = read_file(file_path);
    if (source_code == NULL) {
        return EXIT_FAILURE;
    }

    TokenArray* token_array = Lexer_parseFile(source_code);

    save_tokens("lexer/Token_result_parsing.txt", token_array);

    TokenArray_free(token_array);
    free(source_code);
    return 0;
}
