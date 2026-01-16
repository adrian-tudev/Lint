#include "test.h"
#include "parser/parser.h"
#include "execution/interpreter.h"

bool parser_edge_cases(void) {
  bool passed_tests = true;

  { // Single identifier statement
    const char* source = "x;";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    
    passed_tests &= test_assert(program != NULL, "Program should not be NULL");
    if (program) {
        passed_tests &= test_assert(program->items.size == 1, "Should parse single identifier statement");
        if (program->items.size == 1) {
            TopLevel* tl = (TopLevel*)vec_get(&program->items, 0);
            passed_tests &= test_assert(tl->kind == TOP_STATEMENT, "Should be a statement");
            if (tl->kind == TOP_STATEMENT) {
                Statement* stmt = tl->as.statement;
                passed_tests &= test_assert(stmt->kind == STMT_EXPR, "Should be an expression statement");
                if (stmt->kind == STMT_EXPR) {
                    Expression* expr = stmt->as.expr;
                    passed_tests &= test_assert(expr->kind == EXPR_IDENTIFIER, "Should be an identifier expression");
                    if (expr->kind == EXPR_IDENTIFIER) {
                         passed_tests &= test_assert(strcmp(expr->as.identifier, "x") == 0, "Identifier should be 'x'");
                    }
                }
            }
        }
    }

    vec_free(&tokens);
    program_free(program);
  }

  { // Trailing binary operator (Crash reproduction)
    const char* source = "1 +";
    Vector tokens = tokenize(source, 0);
    // This is expected to fail parsing, but NOT crash
    Program* program = parse(tokens);
    
    // If we get here without segfault, we passed the crash test.
    // We should also check that the program is empty or validly indicates error.
    // Currently parse() returns a program with 0 statements if expression parsing fails.
    passed_tests &= test_assert(program->items.size == 0, "1 + should result in empty program (parse error)");
    
    vec_free(&tokens);
    program_free(program);
  }

  { // Unclosed parenthesis at EOF (Crash reproduction)
    const char* source = "(";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    
    passed_tests &= test_assert(program->items.size == 0, "( should result in empty program (parse error)");
    
    vec_free(&tokens);
    program_free(program);
  }

  { // Unclosed parenthesis with content at EOF
    const char* source = "(1";
    Vector tokens = tokenize(source, 0);
    Program* program = parse(tokens);
    
    passed_tests &= test_assert(program->items.size == 0, "\"(1\" should result in empty program (parse error)");
    
    vec_free(&tokens);
    program_free(program);
  }

  return passed_tests;
}
