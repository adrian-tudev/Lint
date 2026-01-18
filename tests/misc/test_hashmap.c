#include "utils/hashmap.h"
#include "utils/value.h"
#include "test.h"
#include <string.h>
#include <stdlib.h>

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