#include "test.h"

#include "parser/parser.h"
#include "interpreter.h"

bool complex_expr_parsing(void) {
  bool passed_tests = true;

  { // Associativity: subtraction
    const char* source = "10 - 5 - 2;";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    Statement* stmt = ((TopLevel*)vec_get(&program->items, 0))->as.statement;
    Expression* expr = stmt->as.expr;
    Expression result_expr = eval_expression(expr);
    passed_tests &= test_assert(result_expr.kind == EXPR_NUMBER, "Result kind should be EXPR_NUMBER");
    float result = result_expr.as.number;
    passed_tests &= test_assert(result == 3.0, "10 - 5 - 2 should equal 3.0 (Left Associative)");
    vec_free(&tokens);
    program_free(program);
  }

  { // Associativity: division
    const char* source = "20 / 2 / 2;";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    Statement* stmt = ((TopLevel*)vec_get(&program->items, 0))->as.statement;
    Expression* expr = stmt->as.expr;
    Expression result_expr = eval_expression(expr);
    passed_tests &= test_assert(result_expr.kind == EXPR_NUMBER, "Result kind should be EXPR_NUMBER");
    float result = result_expr.as.number;
    passed_tests &= test_assert(result == 5.0, "20 / 2 / 2 should equal 5.0 (Left Associative)");
    vec_free(&tokens);
    program_free(program);
  }

  { // Precedence: mul/div over add/sub
    const char* source = "2 + 3 * 4;";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    Statement* stmt = ((TopLevel*)vec_get(&program->items, 0))->as.statement;
    Expression* expr = stmt->as.expr;
    Expression result_expr = eval_expression(expr);
    passed_tests &= test_assert(result_expr.kind == EXPR_NUMBER, "Result kind should be EXPR_NUMBER");
    float result = result_expr.as.number;
    passed_tests &= test_assert(result == 14.0, "2 + 3 * 4 should equal 14.0");
    vec_free(&tokens);
    program_free(program);
  }

  { // Precedence: parentheses
    const char* source = "(2 + 3) * 4;";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    Statement* stmt = ((TopLevel*)vec_get(&program->items, 0))->as.statement;
    Expression* expr = stmt->as.expr;
    Expression result_expr = eval_expression(expr);
    passed_tests &= test_assert(result_expr.kind == EXPR_NUMBER, "Result kind should be EXPR_NUMBER");
    float result = result_expr.as.number;
    passed_tests &= test_assert(result == 20.0, "(2 + 3) * 4 should equal 20.0");
    vec_free(&tokens);
    program_free(program);
  }

  { // Complex Boolean/Comparison
    const char* source = "1 + 2 < 5 && 3 * 2 == 6;";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    Statement* stmt = ((TopLevel*)vec_get(&program->items, 0))->as.statement;
    Expression* expr = stmt->as.expr;
    Expression result_expr = eval_expression(expr);
    passed_tests &= test_assert(result_expr.kind == EXPR_BOOL, "Result kind should be EXPR_BOOL");
    bool result = result_expr.as.boolean;
    passed_tests &= test_assert(result == true, "1 + 2 < 5 && 3 * 2 == 6 should be true");
    vec_free(&tokens);
    program_free(program);
  }

  { // Unary operators
    const char* source = "- -5;";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    Statement* stmt = ((TopLevel*)vec_get(&program->items, 0))->as.statement;
    Expression* expr = stmt->as.expr;
    Expression result_expr = eval_expression(expr);
    passed_tests &= test_assert(result_expr.kind == EXPR_NUMBER, "Result kind should be EXPR_NUMBER");
    float result = result_expr.as.number;
    passed_tests &= test_assert(result == 5.0, "- -5 should equal 5.0");
    vec_free(&tokens);
    program_free(program);
  }
  
  { // Unary operators mixed with binary
    const char* source = "5 + -3;";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    Statement* stmt = ((TopLevel*)vec_get(&program->items, 0))->as.statement;
    Expression* expr = stmt->as.expr;
    Expression result_expr = eval_expression(expr);
    passed_tests &= test_assert(result_expr.kind == EXPR_NUMBER, "Result kind should be EXPR_NUMBER");
    float result = result_expr.as.number;
    passed_tests &= test_assert(result == 2.0, "5 + -3 should equal 2.0");
    vec_free(&tokens);
    program_free(program);
  }

  { // Nested Parentheses
    const char* source = "((((1 + 2))));";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    Statement* stmt = ((TopLevel*)vec_get(&program->items, 0))->as.statement;
    Expression* expr = stmt->as.expr;
    Expression result_expr = eval_expression(expr);
    passed_tests &= test_assert(result_expr.kind == EXPR_NUMBER, "Result kind should be EXPR_NUMBER");
    float result = result_expr.as.number;
    passed_tests &= test_assert(result == 3.0, "((((1 + 2)))) should equal 3.0");
    vec_free(&tokens);
    program_free(program);
  }

  { // Chained comparison (should fail type check if evaluated as (1 < 2) < 3)
    const char* source = "1 < 2 < 3;";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    Statement* stmt = ((TopLevel*)vec_get(&program->items, 0))->as.statement;
    Expression* expr = stmt->as.expr;
    // This should result in a type mismatch error during evaluation
    // because (1 < 2) is true (bool), and true < 3 is bool < number.
    Expression res = eval_expression(expr);
    passed_tests &= test_assert(res.kind == EXPR_INVALID, "1 < 2 < 3 should fail type check");
    vec_free(&tokens);
    program_free(program);
  }

  return passed_tests;
}
