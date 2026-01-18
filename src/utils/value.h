#ifndef VALUE_H_
#define VALUE_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    VAL_BOOL,
    VAL_INT,
    VAL_STRING,
} ValueType;

typedef struct Value {
    ValueType type;
    union {
        bool boolean;
        int64_t integer;
        char* string; // owned by the value
    } as;
} Value;


Value* new_bool_value(bool value);
Value* new_int_value(int64_t value);
Value* new_string_value(const char* chars);
Value* copy_value(const Value* value);
void free_value(Value* value);

#endif // VALUE_H_
