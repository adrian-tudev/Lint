#include "test.h"

#include "parser/statement.h"
#include "parser/parse_context.h"
#include "ast/grammar.h"
#include "lexer/token.h"

#include <string.h>

bool let_statement_parsing(void) {
  bool passed_tests = true;

  { // let x = 5;
    const char* source = "let x = 5;";
    Vector tokens = tokenize(source, 0);
    init_parse_context(tokens);

    Statement* stmt = parse_statement();
    passed_tests &= test_assert(stmt != NULL, "Statement should not be null");
    if (stmt) {
      passed_tests &= test_assert(stmt->kind == STMT_ASSIGN, "Statement should be an assignment");

      Assignment assignment = stmt->as.assignment;
      passed_tests &= test_assert(strcmp(assignment.identifier, "x") == 0, "Identifier should be 'x'");

      Expression* rvalue = assignment.rvalue;
      passed_tests &= test_assert(rvalue->kind == EXPR_NUMBER, "Rvalue should be a number expression");
      passed_tests &= test_assert(rvalue->as.number == 5.0, "Rvalue should be 5.0");

      stmt_free(stmt);
    }

    vec_free(&tokens);
  }

  { // let y = true;
    const char* source = "let y = true;";
    Vector tokens = tokenize(source, 0);
    init_parse_context(tokens);
    Statement* stmt = parse_statement();
    passed_tests &= test_assert(stmt != NULL, "Statement should not be null");

    if(stmt) {
      passed_tests &= test_assert(stmt->kind == STMT_ASSIGN, "Statement should be an assignment");

      Assignment assignment = stmt->as.assignment;
      passed_tests &= test_assert(strcmp(assignment.identifier, "y") == 0, "Identifier should be 'y'");

      Expression* rvalue = assignment.rvalue;
      passed_tests &= test_assert(rvalue->kind == EXPR_BOOL, "Rvalue should be a boolean expression");
      passed_tests &= test_assert(rvalue->as.boolean == true, "Rvalue should be true");

      stmt_free(stmt);
    }
    vec_free(&tokens);
  }

  { // let z = 10 + 2;
    const char* source = "let z = 10 + 2;";
    Vector tokens = tokenize(source, 0);
    init_parse_context(tokens);
    Statement* stmt = parse_statement();
    passed_tests &= test_assert(stmt != NULL, "Statement should not be null");

    if (stmt) {
      passed_tests &= test_assert(stmt->kind == STMT_ASSIGN, "Statement should be an assignment");

      Assignment assignment = stmt->as.assignment;
      passed_tests &= test_assert(strcmp(assignment.identifier, "z") == 0, "Identifier should be 'z'");

      Expression* rvalue = assignment.rvalue;
      passed_tests &= test_assert(rvalue->kind == EXPR_BINARY, "Rvalue should be a binary expression");

      stmt_free(stmt);
    }
    vec_free(&tokens);
  }

  { // let a = b;
    const char* source = "let a = b;";
    Vector tokens = tokenize(source, 0);
    init_parse_context(tokens);
    Statement* stmt = parse_statement();
    passed_tests &= test_assert(stmt != NULL, "Statement should not be null");

    if (stmt) {
      passed_tests &= test_assert(stmt->kind == STMT_ASSIGN, "Statement should be an assignment");

      Assignment assignment = stmt->as.assignment;
      passed_tests &= test_assert(strcmp(assignment.identifier, "a") == 0, "Identifier should be 'a'");

      Expression* rvalue = assignment.rvalue;
      passed_tests &= test_assert(rvalue->kind == EXPR_IDENTIFIER, "Rvalue should be an identifier expression");
      passed_tests &= test_assert(strcmp(rvalue->as.identifier, "b") == 0, "Rvalue should be 'b'");
      stmt_free(stmt);
    }
    vec_free(&tokens);
  }

  return passed_tests;
}
