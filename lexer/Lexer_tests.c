#include <assert.h>

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

    // DynamicString functions tests

    DynamicString* dynamic_string_test = DynamicString_create();
    assert(DynamicString_getString(dynamic_string_test)[0] == '\0');
    assert(DynamicString_getCapacity(dynamic_string_test) == DYNAMIC_STRING_CAPACITY_BASE);
    assert(DynamicString_getCount(dynamic_string_test) == 0);

    assert(DynamicString_addChar(dynamic_string_test, 'a'));
    assert(DynamicString_getString(dynamic_string_test)[0] == 'a');
    assert(DynamicString_getCharAtIndex(dynamic_string_test, 0) == 'a');
    assert(DynamicString_getCount(dynamic_string_test) == 1);

    for (int index = 0; index < DYNAMIC_STRING_CAPACITY_BASE; index++) {
        assert(DynamicString_addChar(dynamic_string_test, 'a'));
    }
    assert(DynamicString_getCapacity(dynamic_string_test) == 2 * DYNAMIC_STRING_CAPACITY_BASE);
    assert(DynamicString_getCount(dynamic_string_test) == DYNAMIC_STRING_CAPACITY_BASE);


    // Token functions tests

    Token* token_test = Token_createToken(TOKEN_VIDE, "test");
    assert(token_test != NULL);
    assert(Token_getTokenType(token_test) == TOKEN_VIDE);
    assert(Token_getContent(token_test) != NULL);
    assert(Token_getContent(token_test)[0] == 't');

    // TokenArray functions tests

    TokenArray* token_array_test = TokenArray_create(TOKEN_ARRAY_CAPACITY_BASE);
    assert(TokenArray_getTokens(token_array_test) != NULL);
    assert(TokenArray_getCapacity(token_array_test) == TOKEN_ARRAY_CAPACITY_BASE);
    assert(TokenArray_getCount(token_array_test) == 0);

    assert(TokenArray_addToken(token_array_test, token_test));
    assert(TokenArray_getCount(token_array_test) == 2);

    assert(TokenArray_getTokenAtIndex(token_array_test, 1) == token_test);

    // Token parsing functions tests

    assert(!isSi("sinon", token_test));
    assert(!isSi("so", token_test));
    assert(isSi("si", token_test));
    assert(Token_getTokenType(token_test) == TOKEN_SI);
    assert(Token_getContent(token_test)[0] == 's' && Token_getContent(token_test)[1] == 'i');

    assert(!isFin("finition", token_test));
    assert(!isFin("fan", token_test));
    assert(isFin("fin", token_test));
    assert(Token_getTokenType(token_test) == TOKEN_FDF);
    assert(Token_getContent(token_test)[0] == 'f' && Token_getContent(token_test)[1] == 'i' && Token_getContent(token_test)[2] == 'n');

    assert(!isAFourCharToken("sinon", token_test));
    assert(!isAFourCharToken("test", token_test));
    assert(isAFourCharToken("vide", token_test));
    assert(Token_getTokenType(token_test) == TOKEN_VIDE);
    char* token_test_content = Token_getContent(token_test);
    assert(token_test_content[0] == 'v' && token_test_content[1] == 'i' && token_test_content[2] == 'd' && token_test_content[3] == 'e');

    assert(isAFourCharToken("char", token_test));
    assert(Token_getTokenType(token_test) == TOKEN_CHAR);
    token_test_content = Token_getContent(token_test);
    assert(token_test_content[0] == 'c' && token_test_content[1] == 'h' && token_test_content[2] == 'a' && token_test_content[3] == 'r');

    assert(!isSinon("langue", token_test));
    assert(!isSinon("sinan", token_test));
    assert(isSinon("sinon", token_test));
    assert(Token_getTokenType(token_test) == TOKEN_SINON);
    token_test_content = Token_getContent(token_test);
    assert(token_test_content[0] == 's' && token_test_content[1] == 'i' && token_test_content[2] == 'n' && token_test_content[3] == 'o' && token_test_content[4] == 'n');


    assert(!isASixCharToken("sinon", token_test));
    assert(!isASixCharToken("langue", token_test));
    assert(isASixCharToken("entier", token_test));
    assert(Token_getTokenType(token_test) == TOKEN_ENTIER);
    token_test_content = Token_getContent(token_test);
    assert(token_test_content[0] == 'e' && token_test_content[1] == 'n' && token_test_content[2] == 't' && token_test_content[3] == 'i' && token_test_content[4] == 'e' && token_test_content[5] == 'r');

    assert(!isASevenCharToken("sinon", token_test));
    assert(!isASevenCharToken("tantqua", token_test));
    assert(isASevenCharToken("tantque", token_test));
    assert(Token_getTokenType(token_test) == TOKEN_TANT_QUE);
    token_test_content = Token_getContent(token_test);
    assert(token_test_content[0] == 't' && token_test_content[1] == 'a' && token_test_content[2] == 'n' && token_test_content[3] == 't' && token_test_content[4] == 'q' && token_test_content[5] == 'u' && token_test_content[6] == 'e');

    // Free area

    DynamicString_free(dynamic_string_test);
    TokenArray_free(token_array_test);

    return 0;
}