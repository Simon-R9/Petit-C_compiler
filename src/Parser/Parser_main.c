/**
 * @file Parser_main.c
 * @brief To execute to create the Lexer + Parser
 */

#include "../../include/Parser/Parser_functions.h"

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

    Parser* parser = Parser_create(token_array);

    ASTNode* root = Parser_parseProgramme(parser);

    FILE *f = fopen("tests/Parser/ast_debug.txt", "w");
    if (f) {
        print_ast(root, f, 0);
        fclose(f);
    }

    ASTNode_free(root);
    Parser_free(parser);
    TokenArray_free(token_array);
    free(source_code);
    return 0;
}