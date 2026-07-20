#include "assert.h"

#include "Lexer_functions.h"

int main(void) {

    // Helper functions tests

    char* string_test = "Test";
    char* copy_string_test = string_copy(string_test);
    assert(string_size(string_test) == 4);
    assert(string_size(string_test) == string_size(copy_string_test));
    int index = 0;
    while (string_test[index] != '\0') {
        assert(string_test[index] == copy_string_test[index]);
        index++;
    }
    free(copy_string_test);


    // Token functions tests


    // TokenArray functions tests

    TokenArray* token_array_test = TokenArray_create(TOKEN_ARRAY_CAPACITY_BASE);
    assert(TokenArray_getTokens(token_array_test) != NULL);
    assert(TokenArray_getCapacity(token_array_test) == TOKEN_ARRAY_CAPACITY_BASE);
    assert(TokenArray_getCount(token_array_test) == 0);

    TokenArray_free(token_array_test);

    return 0;
}