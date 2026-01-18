#include "utils/value.h"
#include "test.h"
#include <string.h>

bool test_value_copy(void) {
    bool passed = true;

    // Test Int
    Value* v_int = new_int_value(42);
    Value* c_int = copy_value(v_int);
    passed &= test_assert(c_int != NULL, "Copied int should not be NULL");
    passed &= test_assert(c_int->type == VAL_INT, "Copied int should have VAL_INT type");
    passed &= test_assert(c_int->as.integer == 42, "Copied int should have value 42");
    free_value(v_int);
    free_value(c_int);

    // Test Bool
    Value* v_bool = new_bool_value(true);
    Value* c_bool = copy_value(v_bool);
    passed &= test_assert(c_bool != NULL, "Copied bool should not be NULL");
    passed &= test_assert(c_bool->type == VAL_BOOL, "Copied bool should have VAL_BOOL type");
    passed &= test_assert(c_bool->as.boolean == true, "Copied bool should have value true");
    free_value(v_bool);
    free_value(c_bool);

    // Test String
    Value* v_str = new_string_value("hello");
    Value* c_str = copy_value(v_str);
    passed &= test_assert(c_str != NULL, "Copied string should not be NULL");
    passed &= test_assert(c_str->type == VAL_STRING, "Copied string should have VAL_STRING type");
    passed &= test_assert(strcmp(c_str->as.string, "hello") == 0, "Copied string should have value 'hello'");
    passed &= test_assert(c_str->as.string != v_str->as.string, "Copied string should be a new allocation");
    free_value(v_str);
    free_value(c_str);

    // Test NULL
    passed &= test_assert(copy_value(NULL) == NULL, "Copy of NULL should be NULL");

    return passed;
}
