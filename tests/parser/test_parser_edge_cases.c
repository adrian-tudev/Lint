#include "test.h"
#include "parser/parser.h"
#include "execution/interpreter.h"

bool parser_edge_cases(void) {
  bool passed_tests = true;

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
