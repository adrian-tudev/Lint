#include "test.h"

#include "ast/parser.h"
#include "interpreter.h"

bool numerical_expr_parsing(void) {
  bool passed_tests = true;

  { // addition
    const char* source = "3 + 5;";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    for (size_t i = 0; i < program->items.size; i++) {
      Statement* stmt = ((TopLevel*)vec_get(&program->items, i))->as.statement;
      Expression* expr = stmt->as.expr;
      float result = eval_expression(expr).as.number;
      passed_tests &= test_assert(result == 8.0, "3 + 5 should equal 8.0");
    }
  }

  { // multiplication and subtraction
    const char* source = "10 - 2 * 3;";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    for (size_t i = 0; i < program->items.size; i++) {
      Statement* stmt = ((TopLevel*)vec_get(&program->items, i))->as.statement;
      Expression* expr = stmt->as.expr;
      float result = eval_expression(expr).as.number;
      passed_tests &= test_assert(result == 4.0, "10 - 2 * 3 should equal 4.0");
    }
  }

  { // single number
    const char* source = "42;";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    for (size_t i = 0; i < program->items.size; i++) {
      Statement* stmt = ((TopLevel*)vec_get(&program->items, i))->as.statement;
      Expression* expr = stmt->as.expr;
      float result = eval_expression(expr).as.number;
      passed_tests &= test_assert(result == 42.0, "42 should equal 42.0");
    }
  }

  return passed_tests;
}