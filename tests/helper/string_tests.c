#include <assert.h>

#include "../../include/helper/string_functions.h"

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

    return 0;
}