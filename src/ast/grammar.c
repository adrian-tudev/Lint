#include "ast/grammar.h"

#include <stdlib.h>
#include <stdio.h>

static Expression *expr_alloc(ExpressionKind kind) {
  Expression *e = (Expression *)malloc(sizeof(Expression));
  if (!e) return NULL;
  e->kind = kind;
  e->row = 0;
  e->column = 0;
  return e;
}

Expression *expr_number(double value) {
  Expression *e = expr_alloc(EXPR_NUMBER);
  if (!e) return NULL;
  e->as.number = value;
  return e;
}

Expression *expr_bool(bool value) {
  Expression *e = expr_alloc(EXPR_BOOL);
  if (!e) return NULL;
  e->as.boolean = value;
  return e;
}

Expression *expr_identifier(const char *name) {
  Expression *e = expr_alloc(EXPR_IDENTIFIER);
  if (!e) return NULL;
  e->as.identifier = name;
  return e;
}

Expression *expr_string(const char *value) {
  Expression *e = expr_alloc(EXPR_STRING);
  if (!e) return NULL;
  e->as.string = value;
  return e;
}

Expression *expr_unary(OperatorKind op, Expression *operand) {
  Expression *e = expr_alloc(EXPR_UNARY);
  if (!e) return NULL;
  e->as.unary.op = op;
  e->as.unary.operand = operand;
  return e;
}

Expression *expr_binary(OperatorKind op, Expression *left, Expression *right) {
  Expression *e = expr_alloc(EXPR_BINARY);
  if (!e) return NULL;
  e->as.binary.op = op;
  e->as.binary.left = left;
  e->as.binary.right = right;
  return e;
}

void expr_free(Expression *expr) {
  if (!expr) return;
  switch (expr->kind) {
    case EXPR_UNARY:
      expr_free(expr->as.unary.operand);
      break;
    case EXPR_BINARY:
      expr_free(expr->as.binary.left);
      expr_free(expr->as.binary.right);
      break;
    case EXPR_NUMBER:
    case EXPR_BOOL:
    case EXPR_IDENTIFIER:
    case EXPR_STRING:
    default:
      break;
  }
  free(expr);
}

void block_init(Block *block) {
  vec_init(&block->statements);
}

Block *block_new(void) {
  Block *b = (Block *)malloc(sizeof(Block));
  if (!b) return NULL;
  block_init(b);
  return b;
}

bool block_add(Block *block, Statement *stmt) {
  if (!block) return false;
  return vec_push(&block->statements, stmt) != 0;
}

void block_clear(Block *block) {
  if (!block) return;
  for (size_t i = 0; i < block->statements.size; i++) {
    Statement *stmt = (Statement *)vec_get(&block->statements, i);
    stmt_free(stmt);
  }
  vec_free(&block->statements);
}

void block_free(Block *block) {
  if (!block) return;
  block_clear(block);
  free(block);
}

static Statement *stmt_alloc(StatementKind kind) {
  Statement *s = (Statement *)malloc(sizeof(Statement));
  if (!s) return NULL;
  s->kind = kind;
  s->row = 0;
  s->column = 0;
  return s;
}

Statement *stmt_expr(Expression *expr) {
  Statement *s = stmt_alloc(STMT_EXPR);
  if (!s) return NULL;
  s->as.expr = expr;
  return s;
}

Statement *stmt_assign(const char *identifier, Expression *rvalue) {
  Statement *s = stmt_alloc(STMT_ASSIGN);
  if (!s) return NULL;
  s->as.assignment.identifier = identifier;
  s->as.assignment.rvalue = rvalue;
  return s;
}

Statement *stmt_return(Expression *value_or_null) {
  Statement *s = stmt_alloc(STMT_RETURN);
  if (!s) return NULL;
  s->as.ret.has_value = (value_or_null != NULL);
  s->as.ret.value = value_or_null;
  return s;
}

Statement *stmt_if(Expression *condition, Block *then_body, Block *else_body_or_null) {
  Statement *s = stmt_alloc(STMT_IF);
  if (!s) return NULL;
  s->as.if_stmt.condition = condition;
  s->as.if_stmt.then_body = then_body;
  s->as.if_stmt.else_body = else_body_or_null;
  return s;
}

Statement *stmt_while(Expression *condition, Block *body) {
  Statement *s = stmt_alloc(STMT_WHILE);
  if (!s) return NULL;
  s->as.while_stmt.condition = condition;
  s->as.while_stmt.body = body;
  return s;
}

// Statement *stmt_block(void) {
//   Statement *s = stmt_alloc(STMT_BLOCK);
//   if (!s) return NULL;
//   block_init(&s->as.block);
//   return s;
// }

void stmt_free(Statement *stmt) {
  if (!stmt) return;
  switch (stmt->kind) {
    case STMT_EXPR:
      expr_free(stmt->as.expr);
      break;
    case STMT_ASSIGN:
      expr_free(stmt->as.assignment.rvalue);
      break;
    case STMT_RETURN:
      if (stmt->as.ret.has_value) expr_free(stmt->as.ret.value);
      break;
    case STMT_IF:
      expr_free(stmt->as.if_stmt.condition);
      block_free(stmt->as.if_stmt.then_body);
      block_free(stmt->as.if_stmt.else_body);
      break;
    case STMT_WHILE:
      expr_free(stmt->as.while_stmt.condition);
      block_free(stmt->as.while_stmt.body);
      break;
    // case STMT_BLOCK:
    //   block_clear(&stmt->as.block);
    //   break;
    default:
      break;
  }
  free(stmt);
}

Function *function_new(const char *identifier) {
  Function *fn = (Function *)malloc(sizeof(Function));
  if (!fn) return NULL;
  fn->identifier = identifier;
  vec_init(&fn->params);
  block_init(&fn->body);
  return fn;
}

bool function_add_param(Function *fn, const char *param) {
  if (!fn) return false;
  return vec_push(&fn->params, (void *)param) != 0;
}

bool function_add_stmt(Function *fn, Statement *stmt) {
  if (!fn) return false;
  return block_add(&fn->body, stmt);
}

void function_free(Function *fn) {
  if (!fn) return;
  block_clear(&fn->body);
  vec_free(&fn->params);
  free(fn);
}

Program *program_new(void) {
  Program *p = (Program *)malloc(sizeof(Program));
  if (!p) return NULL;
  vec_init(&p->items);
  return p;
}

bool program_add_statement(Program *p, Statement *stmt) {
  if (!p) return false;
  TopLevel *t = (TopLevel *)malloc(sizeof(TopLevel));
  if (!t) return false;
  t->kind = TOP_STATEMENT;
  t->as.statement = stmt;
  return vec_push(&p->items, t) != 0;
}

bool program_add_function(Program *p, Function *fn) {
  if (!p) return false;
  TopLevel *t = (TopLevel *)malloc(sizeof(TopLevel));
  if (!t) return false;
  t->kind = TOP_FUNCTION;
  t->as.function = fn;
  return vec_push(&p->items, t) != 0;
}

void program_free(Program *p) {
  if (!p) return;
  for (size_t i = 0; i < p->items.size; i++) {
    TopLevel *t = (TopLevel *)vec_get(&p->items, i);
    if (!t) continue;
    if (t->kind == TOP_STATEMENT) {
      stmt_free(t->as.statement);
    } else if (t->kind == TOP_FUNCTION) {
      function_free(t->as.function);
    }
    free(t);
  }
  vec_free(&p->items);
  free(p);
}

void print_expression(Expression *expr) {
  if (!expr) {
    printf("NULL Expression\n");
    return;
  }

  switch (expr->kind) {
    case EXPR_NUMBER:
      printf("Number: %f\n", expr->as.number);
      break;
    case EXPR_BOOL:
      printf("Boolean: %s\n", expr->as.boolean ? "true" : "false");
      break;
    case EXPR_IDENTIFIER:
      printf("Identifier: %s\n", expr->as.identifier);
      break;
    case EXPR_STRING:
      printf("String: \"%s\"\n", expr->as.string);
      break;
    case EXPR_UNARY:
      printf("Unary Expression: Operator %d\n", expr->as.unary.op);
      print_expression(expr->as.unary.operand);
      break;
    case EXPR_BINARY:
      printf("Binary Expression: Operator %d\n", expr->as.binary.op);
      printf("Left Operand:\n");
      print_expression(expr->as.binary.left);
      printf("Right Operand:\n");
      print_expression(expr->as.binary.right);
      break;
    default:
      printf("Unknown Expression Kind\n");
      break;
  }
}