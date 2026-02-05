#include "parser/function.h"
#include "parser/statement.h"

#include "utils/error.h"

#include <assert.h>

// used to parse after the identifier and left parenthesis
static bool parse_parameters(Vector* params);

Function* parse_function_def(void) {
  // TODO: generate error messages if match returns false ?
  assert(match(TOK_FUNCTION));

  if (peek() == NULL) {
    error_log("Function definition without name\n");
    return NULL;
  }
  const char* function_name = peek()->token;
  assert(match(TOK_IDENTIFIER));
  assert(match(TOK_LEFT_PARENTHESIS));

  Function* fn = function_new(function_name);
  if (!parse_parameters(&fn->params)) {
    function_free(fn);
    return NULL;
  }

  Block* fn_body = statement_to_block(parse_block_statement());
  if (fn_body == NULL) {
    error_log("Couldn't parse function body\n");
    function_free(fn);
    return NULL;
  }
  block_free(fn->body);
  fn->body = fn_body;

  return fn;
}

static bool parse_parameters(Vector* params) {
  if (peek() != NULL && peek()->type != TOK_RIGHT_PARENTHESIS) {
    do {
      if (peek() == NULL || peek()->type != TOK_IDENTIFIER) {
        error_log("Expected identifier for function parameter at %d:%d\n", 
            peek()->row, peek()->column);
        return false;
      }
      const char* param_name = peek()->token;
      vec_push(params, (char*)param_name);
      match(TOK_IDENTIFIER);
    } while (match(TOK_COMMA));
  }

  assert(match(TOK_RIGHT_PARENTHESIS));
  return true;
}

