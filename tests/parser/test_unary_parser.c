#include "test.h"

#include "ast/parser.h"
#include "interpreter.h"

bool unary_expr_parsing(void) {
  bool passed_tests = true;

  { // --5
    const char* source = "--5;";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    for (size_t i = 0; i < program->items.size; i++) {
      Statement* stmt = ((TopLevel*)vec_get(&program->items, i))->as.statement;
      Expression* expr = stmt->as.expr;
      Expression result_expr = eval_expression(expr);
      passed_tests &= test_assert(result_expr.kind == EXPR_NUMBER, "Result kind should be EXPR_NUMBER");
      float result = result_expr.as.number;
      passed_tests &= test_assert(result == 5.0, "--5 should equal 5.0");
    }
  }

  { // 5+-(-.5)
    const char* source = "5+-(-.5);";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    for (size_t i = 0; i < program->items.size; i++) {
      Statement* stmt = ((TopLevel*)vec_get(&program->items, i))->as.statement;
      Expression* expr = stmt->as.expr;
      Expression result_expr = eval_expression(expr);
      passed_tests &= test_assert(result_expr.kind == EXPR_NUMBER, "Result kind should be EXPR_NUMBER");
      float result = result_expr.as.number;
      passed_tests &= test_assert(result == 5.5, "5+-(-.5) should equal 5.5");
    }
  }

  { // 5++-(-0)
    const char* source = "5++-(-0);";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    for (size_t i = 0; i < program->items.size; i++) {
      Statement* stmt = ((TopLevel*)vec_get(&program->items, i))->as.statement;
      Expression* expr = stmt->as.expr;
      Expression result_expr = eval_expression(expr);
      passed_tests &= test_assert(result_expr.kind == EXPR_NUMBER, "Result kind should be EXPR_NUMBER");
      float result = result_expr.as.number;
      passed_tests &= test_assert(result == 5.0, "5++-(-0) should equal 5.0");
    }
  }

  return passed_tests;
}
