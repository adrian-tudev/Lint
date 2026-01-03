#include "test.h"

#include "ast/grammar.h"
#include "interpreter.h"

bool bool_expr(void) {
  bool passed_tests = true;

  { // sanity check: create a boolean expression and verify its fields
    Expression* expr = expr_bool(true);
    passed_tests &= test_assert(expr->kind == EXPR_BOOL, "Expression kind should be EXPR_BOOL");
    passed_tests &= test_assert(expr->as.boolean == true, "Expression boolean value should be true");
    expr_free(expr);
  }

  { // boolean operation: AND
    Expression* l = expr_bool(true);
    Expression* r = expr_bool(false);

    Expression* and_expr = expr_binary(OP_AND, l, r);

    Expression result = eval_expression(and_expr);
    passed_tests &= test_assert(result.kind == EXPR_BOOL, "Result kind should be EXPR_BOOL");
    passed_tests &= test_assert(result.as.boolean == false, "true AND false should be false");

    expr_free(and_expr);
  }

  { // boolean operation: OR
    Expression* l = expr_bool(true);
    Expression* r = expr_bool(false);

    Expression* or_expr = expr_binary(OP_OR, l, r);

    Expression result = eval_expression(or_expr);
    passed_tests &= test_assert(result.kind == EXPR_BOOL, "Result kind should be EXPR_BOOL");
    passed_tests &= test_assert(result.as.boolean == true, "true OR false should be true");

    expr_free(or_expr);
  }

  { // boolean operation: NOT
    Expression* operand = expr_bool(false);

    Expression* not_expr = expr_unary(OP_NOT, operand);

    Expression result = eval_expression(not_expr);
    passed_tests &= test_assert(result.kind == EXPR_BOOL, "Result kind should be EXPR_BOOL");
    passed_tests &= test_assert(result.as.boolean == true, "NOT false should be true");

    expr_free(not_expr);
  }

  {
    // comparison operation: EQUAL
    Expression* l = expr_bool(true);
    Expression* r = expr_bool(true);
    Expression* equal_expr = expr_binary(OP_EQUAL, l, r);
    Expression result = eval_expression(equal_expr);
    passed_tests &= test_assert(result.kind == EXPR_BOOL, "Result kind should be EXPR_BOOL");
    passed_tests &= test_assert(result.as.boolean == true, "true EQUAL true should be true");
    expr_free(equal_expr);
  }

  {
    // more complex boolean expression: (true OR false) AND (NOT false)
    Expression* left = expr_binary(OP_OR, expr_bool(true), expr_bool(false));
    Expression* right = expr_unary(OP_NOT, expr_bool(false));
    Expression* complex_expr = expr_binary(OP_AND, left, right);
    Expression result = eval_expression(complex_expr);
    passed_tests &= test_assert(result.kind == EXPR_BOOL, "Result kind should be EXPR_BOOL");
    passed_tests &= test_assert(result.as.boolean == true, "(true OR false) AND (NOT false) should be true");
    expr_free(complex_expr);
  }

  {
    // comparison operation: LESS_THAN
    Expression* l = expr_number(3);
    Expression* r = expr_number(5);
    Expression* less_than_expr = expr_binary(OP_LESS_THAN, l, r);
    Expression result = eval_expression(less_than_expr);
    passed_tests &= test_assert(result.kind == EXPR_BOOL, "Result kind should be EXPR_BOOL");
    passed_tests &= test_assert(result.as.boolean == true, "3 LESS_THAN 5 should be true");
    expr_free(less_than_expr);
  }

  return passed_tests;
}