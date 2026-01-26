#include "utils/hashmap.h"
#include "utils/value.h"
#include "test.h"
#include <string.h>
#include <stdlib.h>

#include "ast/grammar.h"
#include "utils/vector.h"

bool test_hm_copy(void) {
    HashMap* map = hm_create();
    
    Value* v_int = new_int_value(42);
    Value* v_str = new_string_value("hello");
    Value* v_bool = new_bool_value(true);

    hm_set(map, "key1", v_int);
    hm_set(map, "key2", v_str);
    hm_set(map, "key3", v_bool);

    HashMap* copy = hm_copy(map);
    if (!test_assert(copy != NULL, "Copy should not be NULL")) {
        hm_free(map);
        return false;
    }

    Value* c1 = hm_get(copy, "key1");
    if (!test_assert(c1 != NULL && c1->type == VAL_INT && c1->as.integer == 42, "key1 check")) {
        hm_free(map);
        hm_free(copy);
        return false;
    }
    
    Value* c2 = hm_get(copy, "key2");
    if (!test_assert(c2 != NULL && c2->type == VAL_STRING && strcmp(c2->as.string, "hello") == 0, "key2 check")) {
        hm_free(map);
        hm_free(copy);
        return false;
    }
    
    Value* c3 = hm_get(copy, "key3");
    if (!test_assert(c3 != NULL && c3->type == VAL_BOOL && c3->as.boolean == true, "key3 check")) {
        hm_free(map);
        hm_free(copy);
        return false;
    }

    hm_delete(map, "key1");
    if (!test_assert(hm_get(map, "key1") == NULL, "key1 deleted from original")) {
        hm_free(map);
        hm_free(copy);
        return false;
    }
    
    Value* c1_still = hm_get(copy, "key1");
    if (!test_assert(c1_still != NULL && c1_still->type == VAL_INT && c1_still->as.integer == 42, "key1 remains in copy")) {
        hm_free(map);
        hm_free(copy);
        return false;
    }

    hm_free(map);
    hm_free(copy);
    return true;
}

bool test_hm_get_keys(void) {
    HashMap* map = hm_create();
    hm_set(map, "a", new_int_value(1));
    hm_set(map, "b", new_int_value(2));
    hm_set(map, "c", new_int_value(3));
    
    size_t count = 0;
    char** keys = hm_get_keys(map, &count);
    
    if (!test_assert(count == 3, "Count should be 3")) {
        hm_free(map);
        if (keys) free(keys);
        return false;
    }
    if (!test_assert(keys != NULL, "Keys array should not be NULL")) {
        hm_free(map);
        return false;
    }
    
    bool found_a = false, found_b = false, found_c = false;
    for(size_t i=0; i<count; i++) {
        if(strcmp(keys[i], "a") == 0) found_a = true;
        else if(strcmp(keys[i], "b") == 0) found_b = true;
        else if(strcmp(keys[i], "c") == 0) found_c = true;
    }
    
    if (!test_assert(found_a && found_b && found_c, "All keys found")) {
        free(keys);
        hm_free(map);
        return false;
    }
    
    free(keys);
    hm_free(map);
    return true;
}

bool test_hm_copy_with_functions(void) {
    HashMap* map = hm_create();
    
    Function* fn = function_new("my_func");
    function_add_param(fn, "a");
    function_add_param(fn, "b");
    function_add_stmt(fn, stmt_return(expr_binary(OP_ADD, expr_identifier("a"), expr_identifier("b"))));
    
    Value* v_fn = new_function_value(fn);
    hm_set(map, "my_func", v_fn);

    HashMap* copy = hm_copy(map);
    if (!test_assert(copy != NULL, "Function map copy should not be NULL")) {
        hm_free(map);
        return false;
    }

    Value* c_fn_val = hm_get(copy, "my_func");
    if (!test_assert(c_fn_val != NULL && c_fn_val->type == VAL_FUNCTION, "Copied function value check")) {
        hm_free(map);
        hm_free(copy);
        return false;
    }
    
    Function* c_fn = c_fn_val->as.function;
    if (!test_assert(strcmp(c_fn->identifier, "my_func") == 0, "Function identifier check")) {
        hm_free(map);
        hm_free(copy);
        return false;
    }

    if (!test_assert(c_fn->params.size == 2, "Function param count check")) {
        hm_free(map);
        hm_free(copy);
        return false;
    }
    
    if (!test_assert(strcmp((char*)vec_get(&c_fn->params, 0), "a") == 0, "Function param 1 check")) {
        hm_free(map);
        hm_free(copy);
        return false;
    }

    if (!test_assert(strcmp((char*)vec_get(&c_fn->params, 1), "b") == 0, "Function param 2 check")) {
        hm_free(map);
        hm_free(copy);
        return false;
    }
    
    if (!test_assert(c_fn->body->statements.size == 1, "Function body statement count check")) {
        hm_free(map);
        hm_free(copy);
        return false;
    }

    hm_free(map);
    hm_free(copy);
    return true;
}
