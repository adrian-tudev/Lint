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
    // We manually allocate the Block struct to avoid initializing (and leaking)
    // a new vector via block_new() -> vec_init().
    Block *blk = (Block *)malloc(sizeof(Block));
    if (blk == NULL) {
      free(stmt); // Avoid leak of stmt if allocation fails
      return NULL;
    }

    *blk = stmt->as.block;
    // Free the statement wrapper, but NOT the block content (since we moved it)
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

  case LET:
    return parse_assignment_statement();
  case RETURN:
    break;

  // if parses if/elif/else statements
  case IF:
    return parse_if_statement();
    break;

  case FOR:
    break;
  case WHILE:
    break;
  
  case LEFT_BRACE:
    return parse_block_statement();
    break;
  
  // TODO: handle identifier as start of assignment statement or expression
  case IDENTIFIER:
    break;

  // TODO: match against only valid expression symbols
  default: {
    Expression *expr = parse_expression();
    if (expr == NULL) return NULL;
    if (!match(SEMICOLON)) {
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
  assert(match(LEFT_BRACE));
  Block *block = block_new();
  while (true) {
    const Token *token = peek();
    if (token == NULL) {
      error_log("Unexpected end of input in block statement\n");
      block_free(block);
      return NULL;
    }
    if (token->type == RIGHT_BRACE) {
      match(RIGHT_BRACE);
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
  free(block); // stmt_block copies the content, so we free the container
  return s;
}

static Statement *parse_assignment_statement(void) {
  // guarantee we have 'let' token
  assert(match(LET));

  // store the identifier token
  const Token *variable = peek();
  if (variable == NULL) {
    error_log("Unexpected end of input after 'let'\n");
    return NULL;
  }
  if (match(IDENTIFIER)) {
    // expect '=' token
    if (!match(EQUAL)) {
      error_log("Expected '=' after identifier in let statement at %u:%u\n",
                variable->row, variable->column);
      return NULL;
    }

    // parse the rvalue expression
    Expression *rvalue = parse_expression();
    if (rvalue == NULL) {
      error_log("Expected expression after '=' in let statement at %u:%u\n",
                variable->row, variable->column);
      return NULL;
    }

    // expect ';' token
    if (!match(SEMICOLON)) {
      error_log("Expected ';' after expression in let statement at %u:%u\n",
                variable->row, variable->column);
      return NULL;
    }

    return stmt_assign(variable->token, rvalue);
  } else {
    error_log("Expected identifier after 'let' at %u:%u\n", variable->row,
              variable->column);
  }

  return NULL;
}

static Statement* parse_if_statement(void) {
  assert(match(IF));

  if (match(LEFT_PARENTHESIS)) {
    // condition
    Expression* condition = parse_expression();
    if (condition == NULL) {
      error_log("Expected expression after '(' in if statement\n");
      return NULL;
    }

    if (!match(RIGHT_PARENTHESIS)) {
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
    if (match(ELSE)) {
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