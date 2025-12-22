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

    Expression* addition = expr_binary(OP_ADD, l, r);

    Expression result = eval_expression(addition);
    passed_tests &= test_assert(result.as.number == 3, "1 + 2 should equal 3");

    expr_free(addition);
  }

  {
    // subtract two numeric numbers
    Expression* l = expr_number(5);
    Expression* r = expr_number(3);

    Expression* subtraction = expr_binary(OP_SUB, l, r);

    Expression result = eval_expression(subtraction);
    passed_tests &= test_assert(result.as.number == 2, "5 - 3 should equal 2");

    expr_free(subtraction);
  }

  {
    // multiply two numeric numbers
    Expression* l = expr_number(4);
    Expression* r = expr_number(3);

    Expression* multiplication = expr_binary(OP_MUL, l, r);

    Expression result = eval_expression(multiplication);
    passed_tests &= test_assert(result.as.number == 12, "4 * 3 should equal 12");

    expr_free(multiplication);
  }

  {
    // divide two numeric numbers
    Expression* l = expr_number(10);
    Expression* r = expr_number(2);

    Expression* division = expr_binary(OP_DIV, l, r);

    Expression result = eval_expression(division);
    passed_tests &= test_assert(result.as.number == 5, "10 / 2 should equal 5");

    expr_free(division);
  }

  {
    // divide by zero
    Expression* l = expr_number(10);
    Expression* r = expr_number(0);

    Expression* division = expr_binary(OP_DIV, l, r);

    // expect an error log for division by zero
    Expression result = eval_expression(division);
    // since division by zero is not defined, we won't check the result value here

    expr_free(division);
  }

  {
    // floating point addition
    Expression *l = expr_number(1.5);
    Expression *r = expr_number(2.5);
    Expression *addition = expr_binary(OP_ADD, l, r);
    Expression result = eval_expression(addition);
    passed_tests &= test_assert(result.as.number == 4.0, "1.5 + 2.5 should equal 4.0");
    expr_free(addition);
  }

  {
    // boolean operation: equality
    Expression *l = expr_bool(true);
    Expression *r = expr_bool(false);
    Expression *equality = expr_binary(OP_EQUAL, l, r);
    Expression result = eval_expression(equality);
    passed_tests &= test_assert(result.as.boolean == false, "true == false should be false");
    expr_free(equality);
  }

  {
    // more complex expression: (1 + 2) * (3 - 1)
    Expression* one = expr_number(1);
    Expression* two = expr_number(2);
    Expression* three = expr_number(3);
    Expression* one_plus_two = expr_binary(OP_ADD, one, two);
    Expression* one_again = expr_number(1);
    Expression* three_minus_one = expr_binary(OP_SUB, three, one_again);
    Expression* complex_expr = expr_binary(OP_MUL, one_plus_two, three_minus_one);
    Expression result = eval_expression(complex_expr);
    passed_tests &= test_assert(result.as.number == 6, "(1 + 2) * (3 - 1) should equal 6");
    expr_free(complex_expr);
  }

  {
    // unary operation: negation
    Expression* five = expr_number(5);
    Expression* negation = expr_unary(OP_SUB, five);
    Expression result = eval_expression(negation);
    passed_tests &= test_assert(result.as.number == -5, "-5 should equal -5");
    expr_free(negation);
  }

  return passed_tests;
}