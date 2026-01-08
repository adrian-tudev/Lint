#include "value.h"
#include <stdlib.h>
#include <string.h>

Value* new_bool_value(bool value) {
    Value* val = malloc(sizeof(Value));
    if (val == NULL) return NULL;
    val->type = VAL_BOOL;
    val->as.boolean = value;
    return val;
}

Value* new_int_value(int64_t value) {
    Value* val = malloc(sizeof(Value));
    if (val == NULL) return NULL;
    val->type = VAL_INT;
    val->as.integer = value;
    return val;
}

Value* new_string_value(const char* chars) {
    Value* val = malloc(sizeof(Value));
    if (val == NULL) return NULL;
    val->type = VAL_STRING;
    size_t len = strlen(chars);
    val->as.string = malloc(len + 1);
    if (val->as.string == NULL) {
        free(val);
        return NULL;
    }
    memcpy(val->as.string, chars, len);
    val->as.string[len] = '\0';
    return val;
}

void free_value(Value* value) {
    if (value == NULL) return;
    if (value->type == VAL_STRING) {
        free(value->as.string);
    }
    free(value);
}
