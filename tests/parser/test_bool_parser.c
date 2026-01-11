#include "test.h"

#include "parser/parser.h"
#include "execution/interpreter.h"
#include "execution/eval.h"

bool bool_expr_parsing(void) {
  bool passed_tests = true;

  { // true literal
    const char* source = "true;";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    for (size_t i = 0; i < program->items.size; i++) {
      Statement* stmt = ((TopLevel*)vec_get(&program->items, i))->as.statement;
      Expression* expr = stmt->as.expr;
      Expression result_expr = eval_expression(expr);
      passed_tests &= test_assert(result_expr.kind == EXPR_BOOL, "Result kind should be EXPR_BOOL");
      bool result = result_expr.as.boolean;
      passed_tests &= test_assert(result == true, "true should be true");
    }
  }

  { // false literal
    const char* source = "false;";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    for (size_t i = 0; i < program->items.size; i++) {
      Statement* stmt = ((TopLevel*)vec_get(&program->items, i))->as.statement;
      Expression* expr = stmt->as.expr;
      Expression result_expr = eval_expression(expr);
      passed_tests &= test_assert(result_expr.kind == EXPR_BOOL, "Result kind should be EXPR_BOOL");
      bool result = result_expr.as.boolean;
      passed_tests &= test_assert(result == false, "false should be false");
    }
  }

  { // negation
    const char* source = "!true;";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    for (size_t i = 0; i < program->items.size; i++) {
      Statement* stmt = ((TopLevel*)vec_get(&program->items, i))->as.statement;
      Expression* expr = stmt->as.expr;
      Expression result_expr = eval_expression(expr);
      passed_tests &= test_assert(result_expr.kind == EXPR_BOOL, "Result kind should be EXPR_BOOL");
      bool result = result_expr.as.boolean;
      passed_tests &= test_assert(result == false, "!true should be false");
    }
  }

  { // equality
    const char* source = "true == true;";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    for (size_t i = 0; i < program->items.size; i++) {
      Statement* stmt = ((TopLevel*)vec_get(&program->items, i))->as.statement;
      Expression* expr = stmt->as.expr;
      Expression result_expr = eval_expression(expr);
      passed_tests &= test_assert(result_expr.kind == EXPR_BOOL, "Result kind should be EXPR_BOOL");
      bool result = result_expr.as.boolean;
      passed_tests &= test_assert(result == true, "true == true should be true");
    }
  }

  { // inequality
    const char* source = "true != false;";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    for (size_t i = 0; i < program->items.size; i++) {
      Statement* stmt = ((TopLevel*)vec_get(&program->items, i))->as.statement;
      Expression* expr = stmt->as.expr;
      Expression result_expr = eval_expression(expr);
      passed_tests &= test_assert(result_expr.kind == EXPR_BOOL, "Result kind should be EXPR_BOOL");
      bool result = result_expr.as.boolean;
      passed_tests &= test_assert(result == true, "true != false should be true");
    }
  }

  return passed_tests;
}
