/**
 * @file DynamicString_test.c
 * @brief Test file about DynamicString struct
 * @author Rosol Simon
 * @date 2026-06-21
 */

#include <assert.h>

#include "../../include/helper/DynamicString.h"

int main(void) {

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

    DynamicString_free(dynamic_string_test);

    return 0;
}