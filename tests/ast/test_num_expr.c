#include "test.h"

#include "ast/grammar.h"
#include "interpreter.h"

// test numeric expressions in the AST
bool num_expr(void) {
  bool passed_tests = true;

  { // sanity check: create a numeric expression and verify its fields
    Expression* expr = expr_number(42.0);
    passed_tests &= test_assert(expr->kind == EXPR_NUMBER, "Expression kind should be EXPR_NUMBER");
    passed_tests &= test_assert(expr->as.number == 42.0, "Expression number value should be 42.0");
    expr_free(expr);
  }

  { // add two numeric numbers
    Expression* l = expr_number(1);
    Expression* r = expr_number(2);

    Expression* addition = expr_binary("+", l, r);

    int result = eval_expression(addition);
    passed_tests &= test_assert(result == 3, "1 + 2 should equal 3");

    expr_free(addition);
  }

  return passed_tests;
}