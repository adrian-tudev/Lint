#include "ast/grammar.h"

#include <stdlib.h>
#include <stdio.h>

#include "lexer/token.h"

Statement* stmt_copy(const Statement* stmt);
Block* block_copy(const Block* block);

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

Expression *expr_invalid(void) {
  Expression *e = expr_alloc(EXPR_INVALID);
  if (!e) return NULL;
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

Expression *expr_fn_call(const char *identifier, Vector *arguments) {
  Expression *e = expr_alloc(EXPR_FN_CALL);
  if (!e) return NULL;
  e->as.fn_call.identifier = identifier;
  e->as.fn_call.arguments = *arguments;
  return e;
}

void print(Expression expr) {
  switch (expr.kind) {
    case EXPR_BOOL:
      printf("%s\n", (expr.as.boolean == 1 ? "true" : "false"));
      break;
    case EXPR_NUMBER:
      printf("%f\n", expr.as.number);
      break;
    case EXPR_INVALID:
      break;
    default:
      printf("weird type evaluated\n");
  }
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
    case EXPR_FN_CALL:
      for (size_t i = 0; i < expr->as.fn_call.arguments.size; i++) {
        expr_free((Expression*)vec_get(&expr->as.fn_call.arguments, i));
      }
      vec_free(&expr->as.fn_call.arguments);
      break;
    case EXPR_NUMBER:
    case EXPR_BOOL:
    case EXPR_INVALID:
    case EXPR_IDENTIFIER:
    case EXPR_STRING:
    default:
      break;
  }
  free(expr);
}

static void block_init(Block *block) {
  vec_init(&block->statements);
  block->ctx = hm_create();
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

static void block_clear(Block *block) {
  if (!block) return;
  for (size_t i = 0; i < block->statements.size; i++) {
    Statement *stmt = (Statement *)vec_get(&block->statements, i);
    stmt_free(stmt);
  }
  vec_free(&block->statements);
  hm_free(block->ctx);
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

Statement *stmt_assign(const char *identifier, Expression *rvalue, bool reassignment) {
  Statement *s = stmt_alloc(STMT_ASSIGN);
  if (!s) return NULL;
  s->as.assignment.identifier = identifier;
  s->as.assignment.rvalue = rvalue;
  s->as.assignment.reassignment = reassignment;
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

Statement *stmt_block(Block *block) {
  Statement *s = stmt_alloc(STMT_BLOCK);
  if (!s) return NULL;
  s->as.block = block;
  return s;
}

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
    case STMT_BLOCK:
      block_free(stmt->as.block);
      break;
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
  fn->body = block_new();
  if (!fn->body) {
    free(fn);
    return NULL;
  }
  return fn;
}

bool function_add_param(Function *fn, const char *param) {
  if (!fn) return false;
  return vec_push(&fn->params, (void *)param) != 0;
}

bool function_add_stmt(Function *fn, Statement *stmt) {
  if (!fn) return false;
  return block_add(fn->body, stmt);
}

void function_free(Function *fn) {
  if (!fn) return;
  block_free(fn->body);
  vec_free(&fn->params);
  free(fn);
}

Expression* expr_copy(const Expression* expr) {
    if (!expr) return NULL;
    switch (expr->kind) {
        case EXPR_NUMBER:
            return expr_number(expr->as.number);
        case EXPR_BOOL:
            return expr_bool(expr->as.boolean);
        case EXPR_IDENTIFIER:
            return expr_identifier(expr->as.identifier);
        case EXPR_STRING:
            return expr_string(expr->as.string);
        case EXPR_UNARY: {
            Expression* operand = expr_copy(expr->as.unary.operand);
            if (!operand) return NULL;
            return expr_unary(expr->as.unary.op, operand);
        }
        case EXPR_BINARY: {
            Expression* left = expr_copy(expr->as.binary.left);
            if (!left) return NULL;
            Expression* right = expr_copy(expr->as.binary.right);
            if (!right) {
                expr_free(left);
                return NULL;
            }
            return expr_binary(expr->as.binary.op, left, right);
        }
        case EXPR_FN_CALL: {
            Vector new_arguments;
            vec_init(&new_arguments);
            for (size_t i = 0; i < expr->as.fn_call.arguments.size; i++) {
                Expression* arg = (Expression*)vec_get(&expr->as.fn_call.arguments, i);
                Expression* new_arg = expr_copy(arg);
                if (!new_arg) {
                    // Free already copied arguments and the vector itself
                    for (size_t j = 0; j < new_arguments.size; j++) {
                        expr_free((Expression*)vec_get(&new_arguments, j));
                    }
                    vec_free(&new_arguments);
                    return NULL;
                }
                vec_push(&new_arguments, new_arg);
            }
            return expr_fn_call(expr->as.fn_call.identifier, &new_arguments);
        }
        case EXPR_INVALID:
            return expr_invalid();
        default:
            return NULL;
    }
}

Statement* stmt_copy(const Statement* stmt) {
    if (!stmt) return NULL;
    switch (stmt->kind) {
        case STMT_EXPR: {
            Expression* expr = expr_copy(stmt->as.expr);
            if (!expr) return NULL;
            return stmt_expr(expr);
        }
        case STMT_ASSIGN: {
            Expression* rvalue = expr_copy(stmt->as.assignment.rvalue);
            if (!rvalue) return NULL;
            return stmt_assign(stmt->as.assignment.identifier, rvalue, stmt->as.assignment.reassignment);
        }
        case STMT_RETURN: {
            Expression* value = NULL;
            if (stmt->as.ret.has_value) {
                value = expr_copy(stmt->as.ret.value);
                if (!value) return NULL;
            }
            return stmt_return(value);
        }
        case STMT_IF: {
            Expression* condition = expr_copy(stmt->as.if_stmt.condition);
            if (!condition) return NULL;
            Block* then_body = block_copy(stmt->as.if_stmt.then_body);
            if (!then_body) {
                expr_free(condition);
                return NULL;
            }
            Block* else_body = NULL;
            if (stmt->as.if_stmt.else_body) {
                else_body = block_copy(stmt->as.if_stmt.else_body);
                if (!else_body) {
                    expr_free(condition);
                    block_free(then_body);
                    return NULL;
                }
            }
            return stmt_if(condition, then_body, else_body);
        }
        case STMT_WHILE: {
            Expression* condition = expr_copy(stmt->as.while_stmt.condition);
            if (!condition) return NULL;
            Block* body = block_copy(stmt->as.while_stmt.body);
            if (!body) {
                expr_free(condition);
                return NULL;
            }
            return stmt_while(condition, body);
        }
        case STMT_BLOCK: {
            Block* body = block_copy(stmt->as.block);
            if (!body) return NULL;
            return stmt_block(body);
        }
        default:
            return NULL;
    }
}

Block* block_copy(const Block* block) {
    if (!block) return NULL;
    Block* new_block = block_new();
    if (!new_block) return NULL;

    for (size_t i = 0; i < block->statements.size; i++) {
        Statement* stmt = (Statement*)vec_get(&block->statements, i);
        Statement* new_stmt = stmt_copy(stmt);
        if (!new_stmt) {
            block_free(new_block);
            return NULL;
        }
        block_add(new_block, new_stmt);
    }

    hm_free(new_block->ctx); // free empty hashmap
    new_block->ctx = hm_copy(block->ctx);
    if (!new_block->ctx) {
        block_free(new_block);
        return NULL;
    }
    return new_block;
}

Function* function_copy(const Function* fn) {
    if (!fn) return NULL;
    Function* new_fn = function_new(fn->identifier);
    if (!new_fn) return NULL;

    for (size_t i = 0; i < fn->params.size; i++) {
        const char* param = (const char*)vec_get(&fn->params, i);
        vec_push(&new_fn->params, (void*)param);
    }

    block_free(new_fn->body); // free the empty block
    new_fn->body = block_copy(fn->body);
    if (!new_fn->body) {
        // new_fn is not completely valid, so free it
        // but function_free will try to free body, which is NULL
        // so we need to be careful
        free(new_fn);
        return NULL;
    }

    return new_fn;
}

Program *program_new(void) {
  Program *p = (Program *)malloc(sizeof(Program));
  if (!p) return NULL;
  vec_init(&p->items);
  p->ctx = hm_create();
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
  hm_free(p->ctx);
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

OperatorKind token_type_to_op(TokenType type) {
  switch (type) {
    case TOK_PLUS: return OP_ADD;
    case TOK_MINUS: return OP_SUB;
    case TOK_STAR: return OP_MUL;
    case TOK_SLASH: return OP_DIV;
    case TOK_BANG: return OP_NOT;
    case TOK_EQUAL_EQUAL: return OP_EQUAL;
    case TOK_BANG_EQUAL: return OP_NOT_EQUAL;
    case TOK_LESS: return OP_LESS_THAN;
    case TOK_LESS_EQUAL: return OP_LESS_OR_EQUAL;
    case TOK_GREATER: return OP_GREATER_THAN;
    case TOK_GREATER_EQUAL: return OP_GREATER_OR_EQUAL;
    case TOK_AND: return OP_AND;
    case TOK_OR: return OP_OR;
    default: return (OperatorKind)-1;
  }
}

bool is_numerical_op(OperatorKind op) {
  switch (op) {
    case OP_AND:
    case OP_OR:
    case OP_NOT:
      return false;
    default:
      return true;
  }
}

bool is_boolean_op(OperatorKind op) {
  switch (op) {
    case OP_AND:
    case OP_OR:
    case OP_NOT:
    case OP_EQUAL:
    case OP_NOT_EQUAL:
      return true;
    default:
      return false;
  }
}

// =====================
// AST Printing
// =====================

static void print_indent(int indent) {
  for (int i = 0; i < indent; i++) printf("  ");
}

static const char* op_to_string(OperatorKind op) {
  switch (op) {
    case OP_ADD: return "+";
    case OP_SUB: return "-";
    case OP_MUL: return "*";
    case OP_DIV: return "/";
    case OP_AND: return "&&";
    case OP_OR: return "||";
    case OP_NOT: return "!";
    case OP_EQUAL: return "==";
    case OP_NOT_EQUAL: return "!=";
    case OP_LESS_THAN: return "<";
    case OP_LESS_OR_EQUAL: return "<=";
    case OP_GREATER_THAN: return ">";
    case OP_GREATER_OR_EQUAL: return ">=";
    default: return "?";
  }
}

void ast_print_expr(const Expression *expr, int indent) {
  if (!expr) {
    print_indent(indent);
    printf("(null)\n");
    return;
  }
  print_indent(indent);
  switch (expr->kind) {
    case EXPR_NUMBER:
      printf("Number(%g)\n", expr->as.number);
      break;
    case EXPR_BOOL:
      printf("Bool(%s)\n", expr->as.boolean ? "true" : "false");
      break;
    case EXPR_IDENTIFIER:
      printf("Identifier(%s)\n", expr->as.identifier);
      break;
    case EXPR_STRING:
      printf("String(\"%s\")\n", expr->as.string);
      break;
    case EXPR_UNARY:
      printf("Unary(%s)\n", op_to_string(expr->as.unary.op));
      ast_print_expr(expr->as.unary.operand, indent + 1);
      break;
    case EXPR_BINARY:
      printf("Binary(%s)\n", op_to_string(expr->as.binary.op));
      ast_print_expr(expr->as.binary.left, indent + 1);
      ast_print_expr(expr->as.binary.right, indent + 1);
      break;
    case EXPR_FN_CALL:
      printf("FnCall(%s)\n", expr->as.fn_call.identifier);
      for (size_t i = 0; i < expr->as.fn_call.arguments.size; i++) {
        ast_print_expr((Expression*)vec_get(&expr->as.fn_call.arguments, i), indent + 1);
      }
      break;
    case EXPR_INVALID:
      printf("Invalid\n");
      break;
  }
}

void ast_print_stmt(const Statement *stmt, int indent) {
  if (!stmt) {
    print_indent(indent);
    printf("(null)\n");
    return;
  }
  print_indent(indent);
  switch (stmt->kind) {
    case STMT_EXPR:
      printf("ExprStmt\n");
      ast_print_expr(stmt->as.expr, indent + 1);
      break;
    case STMT_ASSIGN:
      printf("Assign(%s)\n", stmt->as.assignment.identifier);
      ast_print_expr(stmt->as.assignment.rvalue, indent + 1);
      break;
    case STMT_RETURN:
      printf("Return\n");
      if (stmt->as.ret.has_value) {
        ast_print_expr(stmt->as.ret.value, indent + 1);
      }
      break;
    case STMT_IF:
      printf("If\n");
      print_indent(indent + 1);
      printf("condition:\n");
      ast_print_expr(stmt->as.if_stmt.condition, indent + 2);
      print_indent(indent + 1);
      printf("then:\n");
      ast_print_block(stmt->as.if_stmt.then_body, indent + 2);
      if (stmt->as.if_stmt.else_body) {
        print_indent(indent + 1);
        printf("else:\n");
        ast_print_block(stmt->as.if_stmt.else_body, indent + 2);
      }
      break;
    case STMT_WHILE:
      printf("While\n");
      print_indent(indent + 1);
      printf("condition:\n");
      ast_print_expr(stmt->as.while_stmt.condition, indent + 2);
      print_indent(indent + 1);
      printf("body:\n");
      ast_print_block(stmt->as.while_stmt.body, indent + 2);
      break;
    case STMT_BLOCK:
      printf("Block\n");
      ast_print_block(stmt->as.block, indent + 1);
      break;
  }
}

void ast_print_block(const Block *block, int indent) {
  if (!block) {
    print_indent(indent);
    printf("(null)\n");
    return;
  }
  for (size_t i = 0; i < block->statements.size; i++) {
    Statement *stmt = (Statement *)vec_get(&block->statements, i);
    ast_print_stmt(stmt, indent);
  }
}

void ast_print_function(const Function *fn, int indent) {
  if (!fn) {
    print_indent(indent);
    printf("(null)\n");
    return;
  }
  print_indent(indent);
  printf("Function(%s)\n", fn->identifier);
  print_indent(indent + 1);
  printf("params: [");
  for (size_t i = 0; i < fn->params.size; i++) {
    if (i > 0) printf(", ");
    printf("%s", (const char *)vec_get(&fn->params, i));
  }
  printf("]\n");
  print_indent(indent + 1);
  printf("body:\n");
  ast_print_block(fn->body, indent + 2);
}

void ast_print_program(const Program *p) {
  if (!p) {
    printf("Program: (null)\n");
    return;
  }
  printf("Program\n");
  for (size_t i = 0; i < p->items.size; i++) {
    TopLevel *item = (TopLevel *)vec_get(&p->items, i);
    if (!item) continue;
    switch (item->kind) {
      case TOP_STATEMENT:
        ast_print_stmt(item->as.statement, 1);
        break;
      case TOP_FUNCTION:
        ast_print_function(item->as.function, 1);
        break;
    }
  }
}
