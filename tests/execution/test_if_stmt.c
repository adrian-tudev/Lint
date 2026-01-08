#include "test.h"
#include "ast/grammar.h"
#include "execution/interpreter.h"

//
// Helpers
//

// Creates a statement that will fail execution: `if (0) {}`
// "Condition in if statement must be boolean expression" -> returns false.
static Statement* create_fail_stmt(void) {
    Block* empty = block_new();
    // 0 is a number, not a boolean, so this fails type check in execute_if_stmt
    return stmt_if(expr_number(0), empty, NULL);
}

// Creates a statement that executes successfully: `if (true) {}`
static Statement* create_ok_stmt(void) {
    Block* empty = block_new();
    return stmt_if(expr_bool(true), empty, NULL);
}

//
// Tests
//

bool test_if_true_execution(void) {
    bool passed = true;

    // 1. if (true) { ok } else { fail } -> Should PASS
    // This confirms else-branch is NOT taken.
    {
        Block* then_blk = block_new();
        block_add(then_blk, create_ok_stmt());

        Block* else_blk = block_new();
        block_add(else_blk, create_fail_stmt());

        Statement* stmt = stmt_if(expr_bool(true), then_blk, else_blk);
        Program* p = program_new();
        program_add_statement(p, stmt);

        bool result = execute(p);
        program_free(p);
        passed &= test_assert(result == true, "if(true) should skip else-block (failing stmt in else)");
    }

    // 2. if (true) { fail } else { ok } -> Should FAIL
    // This confirms then-branch IS taken.
    {
        Block* then_blk = block_new();
        block_add(then_blk, create_fail_stmt());

        Block* else_blk = block_new();
        block_add(else_blk, create_ok_stmt());

        Statement* stmt = stmt_if(expr_bool(true), then_blk, else_blk);
        Program* p = program_new();
        program_add_statement(p, stmt);

        // Turn off error logging for this expected failure? 
        bool result = execute(p);
        program_free(p);
        passed &= test_assert(result == false, "if(true) should execute then-block (failing stmt in then)");
    }

    return passed;
}

bool test_if_false_execution(void) {
    bool passed = true;

    // 1. if (false) { fail } else { ok } -> Should PASS
    // This confirms then-branch is NOT taken.
    {
        Block* then_blk = block_new();
        block_add(then_blk, create_fail_stmt());

        Block* else_blk = block_new();
        block_add(else_blk, create_ok_stmt());

        Statement* stmt = stmt_if(expr_bool(false), then_blk, else_blk);
        Program* p = program_new();
        program_add_statement(p, stmt);

        bool result = execute(p);
        program_free(p);
        passed &= test_assert(result == true, "if(false) should skip then-block (failing stmt in then)");
    }

    // 2. if (false) { ok } else { fail } -> Should FAIL
    // This confirms else-branch IS taken.
    {
        Block* then_blk = block_new();
        block_add(then_blk, create_ok_stmt());

        Block* else_blk = block_new();
        block_add(else_blk, create_fail_stmt());

        Statement* stmt = stmt_if(expr_bool(false), then_blk, else_blk);
        Program* p = program_new();
        program_add_statement(p, stmt);

        bool result = execute(p);
        program_free(p);
        passed &= test_assert(result == false, "if(false) should execute else-block (failing stmt in else)");
    }

    return passed;
}

bool test_if_no_else_execution(void) {
    bool passed = true;

    // 1. if (false) { fail } -> Should PASS
    // This confirms then-branch is NOT taken.
    {
        Block* then_blk = block_new();
        block_add(then_blk, create_fail_stmt());

        Statement* stmt = stmt_if(expr_bool(false), then_blk, NULL);
        Program* p = program_new();
        program_add_statement(p, stmt);

        bool result = execute(p);
        program_free(p);
        passed &= test_assert(result == true, "if(false) without else should do nothing");
    }

    // 2. if (true) { fail } -> Should FAIL
    // This confirms then-branch IS taken.
    {
        Block* then_blk = block_new();
        block_add(then_blk, create_fail_stmt());

        Statement* stmt = stmt_if(expr_bool(true), then_blk, NULL);
        Program* p = program_new();
        program_add_statement(p, stmt);

        bool result = execute(p);
        program_free(p);
        passed &= test_assert(result == false, "if(true) without else should execute then-block");
    }

    return passed;
}
