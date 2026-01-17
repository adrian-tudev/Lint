#include "parser/statement.h"

#include <assert.h>
#include <stdlib.h>
#include "parser/expression.h"
#include "utils/error.h"

static Statement *parse_block_statement(void);
static Statement *parse_assignment_statement(void);
static Statement *parse_return_statement(void);
static Statement *parse_if_statement(void);
static Statement *parse_while_statement(void);

// Helper to convert any statement (including STMT_BLOCK) into a Block*
static Block *statement_to_block(Statement *stmt) {
  if (stmt == NULL) return NULL;
  
  // If it's already a block statement, unwrap it to get the Block content
  if (stmt->kind == STMT_BLOCK) {
    Block *blk = stmt->as.block;
    free(stmt); 
    return blk;
  }

  // Otherwise, wrap the single statement in a new block
  Block *blk = block_new();
  block_add(blk, stmt);
  return blk;
}

Statement *parse_statement(void) {
  const Token *token = peek();
  if (token == NULL) return NULL;

  switch (token->type) {

  case TOK_LET:
    return parse_assignment_statement();
  case TOK_RETURN:
    break;

  // if parses if/elif/else statements
  case TOK_IF:
    return parse_if_statement();
    break;

  case TOK_FOR:
    break;
  case TOK_WHILE:
    break;
  
  case TOK_LEFT_BRACE:
    return parse_block_statement();
    break;
  
  // TODO: handle identifier as start of assignment statement or expression
  case TOK_IDENTIFIER: {
    const Token* nxt_tok = peek_next();
    // fall through if next token is null, let expression handle it
    if (nxt_tok == NULL) {}
    // assignment
    else if (peek_next()->type == TOK_EQUAL)
      return parse_assignment_statement();
    // any other case, fall through
    else {}
  }

  // TODO: match against only valid expression symbols
  default: {
    Expression *expr = parse_expression();
    if (expr == NULL) return NULL;
    if (!match(TOK_SEMICOLON)) {
      error_log("Expected ';' after expression\n");
      return NULL;
    }
    return stmt_expr(expr);
  }
    break;
  }

  return NULL;
}

static Statement *parse_block_statement(void) {
  assert(match(TOK_LEFT_BRACE));
  Block *block = block_new();
  while (true) {
    const Token *token = peek();
    if (token == NULL) {
      error_log("Unexpected end of input in block statement\n");
      block_free(block);
      return NULL;
    }
    if (token->type == TOK_RIGHT_BRACE) {
      match(TOK_RIGHT_BRACE);
      break;
    }

    Statement *stmt = parse_statement();
    if (stmt == NULL) {
      error_log("Failed to parse statement in block at %u:%u\n",
                token->row, token->column);
      block_free(block);
      return NULL;
    }
    block_add(block, stmt);
  }
  Statement *s = stmt_block(block);
  // Do NOT free block here, as stmt_block now takes ownership of the pointer.
  return s;
}

static Statement *parse_assignment_statement(void) {
  // check if Let keyword exists to determine reassignment
  bool reassignment = !match(TOK_LET);

  // store the identifier token
  const Token *variable = peek();
  if (variable == NULL) {
    error_log("Unexpected end of input after 'let'\n");
    return NULL;
  }
  if (match(TOK_IDENTIFIER)) {
    // expect '=' token
    if (!match(TOK_EQUAL)) {
      error_log("Expected '=' after identifier in assignment at %u:%u\n",
                variable->row, variable->column);
      return NULL;
    }

    // parse the rvalue expression
    Expression *rvalue = parse_expression();
    if (rvalue == NULL) {
      error_log("Expected expression after '=' in assignment at %u:%u\n",
                variable->row, variable->column);
      return NULL;
    }

    // expect ';' token
    if (!match(TOK_SEMICOLON)) {
      error_log("Expected ';' after expression in assignment at %u:%u\n",
                variable->row, variable->column);
      return NULL;
    }

    return stmt_assign(variable->token, rvalue, reassignment);
  } else {
    error_log("Expected identifier after 'let' at %u:%u\n", variable->row,
              variable->column);
  }

  return NULL;
}

static Statement* parse_if_statement(void) {
  assert(match(TOK_IF));

  if (match(TOK_LEFT_PARENTHESIS)) {
    // condition
    Expression* condition = parse_expression();
    if (condition == NULL) {
      error_log("Expected expression after '(' in if statement\n");
      return NULL;
    }

    if (!match(TOK_RIGHT_PARENTHESIS)) {
      error_log("Expected ')' after condition in if statement\n");
      return NULL;
    }

    Statement* then_stmt = parse_block_statement();
    if (then_stmt == NULL) {
      error_log("Expected block statement after if condition\n");
      return NULL;
    }
    Block* then_body = statement_to_block(then_stmt);

    Block* else_body = NULL;
    if (match(TOK_ELSE)) {
      Statement* else_stmt = parse_block_statement();
      if (else_stmt == NULL) {
        error_log("Expected block statement after else\n");
        return NULL;
      }
      else_body = statement_to_block(else_stmt);
    }

    return stmt_if(condition, then_body, else_body);
  }
  return NULL;
}
