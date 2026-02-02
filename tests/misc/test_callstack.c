#include "utils/callstack.h"
#include "ast/grammar.h"
#include "test.h"
#include <string.h>

bool test_callstack_init_and_free(void) {
    CallStack cs;
    callstack_init(&cs);

    if (!test_assert(cs.data != NULL, "Data should be allocated")) {
        callstack_free(&cs);
        return false;
    }
    if (!test_assert(cs.size == 0, "Size should be 0")) {
        callstack_free(&cs);
        return false;
    }
    if (!test_assert(cs.capacity > 0, "Capacity should be > 0")) {
        callstack_free(&cs);
        return false;
    }

    callstack_free(&cs);

    if (!test_assert(cs.data == NULL, "Data should be NULL after free")) {
        return false;
    }
    if (!test_assert(cs.size == 0, "Size should be 0 after free")) {
        return false;
    }

    return true;
}

bool test_callstack_push_pop(void) {
    CallStack cs;
    callstack_init(&cs);

    Function* fn1 = function_new("func1");
    Function* fn2 = function_new("func2");
    Function* fn3 = function_new("func3");

    if (!test_assert(callstack_push(&cs, fn1) == 1, "Push fn1 should succeed")) {
        callstack_free(&cs);
        return false;
    }
    if (!test_assert(callstack_push(&cs, fn2) == 1, "Push fn2 should succeed")) {
        callstack_free(&cs);
        return false;
    }
    if (!test_assert(callstack_push(&cs, fn3) == 1, "Push fn3 should succeed")) {
        callstack_free(&cs);
        return false;
    }

    if (!test_assert(callstack_size(&cs) == 3, "Size should be 3")) {
        callstack_free(&cs);
        return false;
    }

    Function* popped = callstack_pop(&cs);
    if (!test_assert(popped == fn3, "Pop should return fn3")) {
        callstack_free(&cs);
        return false;
    }
    if (!test_assert(strcmp(popped->identifier, "func3") == 0, "Popped function identifier should be func3")) {
        callstack_free(&cs);
        return false;
    }

    popped = callstack_pop(&cs);
    if (!test_assert(popped == fn2, "Pop should return fn2")) {
        callstack_free(&cs);
        return false;
    }

    popped = callstack_pop(&cs);
    if (!test_assert(popped == fn1, "Pop should return fn1")) {
        callstack_free(&cs);
        return false;
    }

    if (!test_assert(callstack_size(&cs) == 0, "Size should be 0 after all pops")) {
        callstack_free(&cs);
        return false;
    }

    callstack_free(&cs);
    return true;
}

bool test_callstack_peek(void) {
    CallStack cs;
    callstack_init(&cs);

    if (!test_assert(callstack_peek(&cs) == NULL, "Peek on empty stack should return NULL")) {
        callstack_free(&cs);
        return false;
    }

    Function* fn1 = function_new("peek_test");
    callstack_push(&cs, fn1);

    Function* peeked = callstack_peek(&cs);
    if (!test_assert(peeked == fn1, "Peek should return fn1")) {
        callstack_free(&cs);
        return false;
    }
    if (!test_assert(callstack_size(&cs) == 1, "Size should still be 1 after peek")) {
        callstack_free(&cs);
        return false;
    }

    callstack_free(&cs);
    return true;
}

bool test_callstack_is_empty(void) {
    CallStack cs;
    callstack_init(&cs);

    if (!test_assert(callstack_is_empty(&cs) == 1, "Stack should be empty after init")) {
        callstack_free(&cs);
        return false;
    }

    Function* fn = function_new("empty_test");
    callstack_push(&cs, fn);

    if (!test_assert(callstack_is_empty(&cs) == 0, "Stack should not be empty after push")) {
        callstack_free(&cs);
        return false;
    }

    callstack_pop(&cs);

    if (!test_assert(callstack_is_empty(&cs) == 1, "Stack should be empty after pop")) {
        callstack_free(&cs);
        return false;
    }

    callstack_free(&cs);
    return true;
}

bool test_callstack_pop_empty(void) {
    CallStack cs;
    callstack_init(&cs);

    Function* result = callstack_pop(&cs);
    if (!test_assert(result == NULL, "Pop on empty stack should return NULL")) {
        callstack_free(&cs);
        return false;
    }

    callstack_free(&cs);
    return true;
}

bool test_callstack_grow(void) {
    CallStack cs;
    callstack_init(&cs);

    // Push more than initial capacity (8) to trigger resize
    for (int i = 0; i < 20; i++) {
        char name[32];
        snprintf(name, sizeof(name), "func_%d", i);
        Function* fn = function_new(name);
        if (!test_assert(callstack_push(&cs, fn) == 1, "Push should succeed during growth")) {
            callstack_free(&cs);
            return false;
        }
    }

    if (!test_assert(callstack_size(&cs) == 20, "Size should be 20")) {
        callstack_free(&cs);
        return false;
    }
    if (!test_assert(cs.capacity >= 20, "Capacity should have grown to at least 20")) {
        callstack_free(&cs);
        return false;
    }

    // Verify LIFO order
    for (int i = 19; i >= 0; i--) {
        char expected_name[32];
        snprintf(expected_name, sizeof(expected_name), "func_%d", i);
        Function* popped = callstack_pop(&cs);
        if (!test_assert(strcmp(popped->identifier, expected_name) == 0, "Popped function name should match")) {
            callstack_free(&cs);
            return false;
        }
    }

    callstack_free(&cs);
    return true;
}
