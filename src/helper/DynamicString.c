#include "../../include/helper/DynamicString.h"

// <========================================================================>
// <======================== DynamicString functions =======================>
// <========================================================================>

DynamicString* DynamicString_create() {
    DynamicString* dynamic_string = (DynamicString*)malloc(sizeof(DynamicString));
    if (dynamic_string == NULL) return NULL;

    char* string = (char*)malloc((DYNAMIC_STRING_CAPACITY_BASE + 1) * sizeof(char));
    if (string == NULL) {
        free(dynamic_string);
        return NULL;
    }
    string[0] = '\0';
    dynamic_string->string = string;
    dynamic_string->capacity = DYNAMIC_STRING_CAPACITY_BASE;
    dynamic_string->count = 0;
    return dynamic_string;
}

bool DynamicString_free(DynamicString* dynamic_string) {
    if (dynamic_string == NULL) return false;
    char* string = dynamic_string->string;
    if (string != NULL) free(string);
    free(dynamic_string);
    return true; 
}

char* DynamicString_getString(DynamicString* dynamic_string) {
    if (dynamic_string == NULL) return NULL;
    return dynamic_string->string;
}

int DynamicString_getCapacity(DynamicString* dynamic_string) {
    if (dynamic_string == NULL) return -1;
    return dynamic_string->capacity;
}

int DynamicString_getCount(DynamicString* dynamic_string) {
    if (dynamic_string == NULL) return -1;
    return dynamic_string->count;
}

bool DynamicString_addChar(DynamicString* dynamic_string, char new_char) {
    if (dynamic_string == NULL) return false;
    int capacity = DynamicString_getCapacity(dynamic_string);
    int count = DynamicString_getCount(dynamic_string);
    char* string = DynamicString_getString(dynamic_string);
    if (capacity == (count + 1)) {
        int new_capacity = 2 * capacity + 1;
        char* new_string = (char*)realloc(string, new_capacity * sizeof(char));
        if (new_string == NULL) return false;
        dynamic_string->string = new_string;
        new_string[count + 1] = '\0';
        new_string[count] = new_char;
        dynamic_string->capacity = 2 * capacity;
        return true;
    }
    string[count + 1] = '\0';
    string[count] = new_char;
    dynamic_string->count++;
    return true;
}

char DynamicString_getCharAtIndex(DynamicString* dynamic_string, int index) {
    if (dynamic_string == NULL) return '\0';
    if (index < 0 || index >= dynamic_string->count) return '\0';
    char* string = dynamic_string->string;
    if (string == NULL) return '\0';
    return string[index];
}

DynamicString* DynamicString_recycleString(DynamicString* dynamic_string) {
    DynamicString* new_dynamic_string = DynamicString_create();
    DynamicString_free(dynamic_string);
    return new_dynamic_string;
}

// <========================================================================>
// <========================================================================>
// <========================================================================>
