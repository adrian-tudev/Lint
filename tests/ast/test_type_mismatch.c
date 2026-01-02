#include "test.h"

#include "ast/grammar_init.h"
#include "interpreter.h"

// check type mismatches in expressions
bool type_mismatch(void) {
    bool passed_tests = true;
    
    {
        // adding a number and a boolean should result in a type mismatch
        Expression* l = expr_number(1);
        Expression* r = expr_bool(true);

        Expression* addition = expr_binary(OP_ADD, l, r);

        Expression result = eval_expression(addition);
        passed_tests &= test_assert(result.kind == EXPR_BOOL, "Result kind should be EXPR_BOOL (error)");
        passed_tests &= test_assert(result.as.boolean == false, "1 + true should result in type mismatch and return false");

        expr_free(addition);
    }

    {
        // comparing a number and a boolean should result in a type mismatch
        Expression* l = expr_number(5);
        Expression* r = expr_bool(false);

        Expression* comparison = expr_binary(OP_LESS_THAN, l, r);

        Expression result = eval_expression(comparison);
        passed_tests &= test_assert(result.kind == EXPR_BOOL, "Result kind should be EXPR_BOOL (error)");
        passed_tests &= test_assert(result.as.boolean == false, "5 < false should result in type mismatch and return false");

        expr_free(comparison);
    }

    {
        // logical AND between a number and a boolean should result in a type mismatch
        Expression* l = expr_number(0);
        Expression* r = expr_bool(true);

        Expression* and_expr = expr_binary(OP_AND, l, r);

        Expression result = eval_expression(and_expr);
        passed_tests &= test_assert(result.kind == EXPR_BOOL, "Result kind should be EXPR_BOOL (error)");
        passed_tests &= test_assert(result.as.boolean == false, "0 AND true should result in type mismatch and return false");

        expr_free(and_expr);
    }

    return passed_tests;
}