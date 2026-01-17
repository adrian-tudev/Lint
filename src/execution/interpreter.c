#include "execution/interpreter.h"

#include <assert.h>
#include <stdbool.h>

#include "ast/grammar.h"
#include "execution/eval.h"
#include "utils/error.h"

// =====================
// Prototypes
// =====================

static bool execute_assignment(Assignment assignment, HashMap* ctx);
static bool execute_if_stmt(IfStmt stmt, HashMap* scope);
static bool execute_while_stmt(WhileStmt loop, HashMap* scope);

static RuntimeConfig cfg = (RuntimeConfig) {
  .debug_info = false,
  .repl = true
};

// =====================
// Public Functions
// =====================

void set_interpreter_config(RuntimeConfig config) {
  cfg = config;
}

bool execute(Program* program) {
  Vector items = program->items;
  for (size_t i = 0; i < items.size; i++) {
    TopLevel* item = (TopLevel*) vec_get(&items, i);
    if (!execute_top_level(item, program->ctx)) return false;
  }
  return true;
}

bool execute_top_level(TopLevel* item, HashMap* ctx) {
  // exit if item is malformed
  if (item == NULL) return false;

  switch (item->kind) {
    case TOP_STATEMENT: {
      Statement* stmt = item->as.statement;
      if (!execute_statement(stmt, ctx)) return false;
      break;
    }
    case TOP_FUNCTION: {
      Function* fn = item->as.function;
      execute_function_def(fn);
      break;
    }
    default:
      // Unknown top-level kind
      return false;
  }
  return true;
}

bool execute_function_def(Function* function) {
  return true;
}

bool execute_block(Block* block, HashMap* parent_scope) {
  // copy values from upper scope
  size_t key_cnt = 0;
  char** keys = hm_get_keys(parent_scope, &key_cnt);
  for (size_t i = 0; i < key_cnt; i++) {
    Value* val = hm_get(parent_scope, keys[i]);
    assert(val != NULL);
    hm_set(block->ctx, keys[i], val);
  }

  Vector stmts = block->statements;
  for (size_t i = 0; i < stmts.size; i++) {
    Statement* stmt = (Statement*) vec_get(&stmts, i);
    if (!execute_statement(stmt, block->ctx)) return false;
  }
  return true;
}

bool execute_statement(Statement* statement, HashMap* scope) {
  StatementKind kind = statement->kind;
  switch (kind) {
    case STMT_EXPR:
      if (cfg.repl)
        print(eval_expression(statement->as.expr, scope));
      else
        eval_expression(statement->as.expr, scope);
      break;
    case STMT_ASSIGN:
      return execute_assignment(statement->as.assignment, scope);
    case STMT_IF:
      return execute_if_stmt(statement->as.if_stmt, scope);
    case STMT_WHILE:
      return execute_while_stmt(statement->as.while_stmt, scope);
    case STMT_BLOCK:
      return execute_block(statement->as.block, scope);
    default:
      break;
  }
  return true;
}

static bool execute_if_stmt(IfStmt stmt, HashMap* scope) {
  Expression* condition = stmt.condition;
  Expression res = eval_expression(condition, scope);

  if (res.kind != EXPR_BOOL) {
    error_log("Condition in if statement must be boolean expression!\n");
    return false;
  }

  // IF-ELSE execution
  if (res.as.boolean == 1 && stmt.then_body != NULL)
    return execute_block(stmt.then_body, scope);
  else if (res.as.boolean == 0 && stmt.else_body != NULL)
    return execute_block(stmt.else_body, scope);
  
  return true;
}

// store identifier in table
static bool execute_assignment(Assignment assignment, HashMap* ctx) {
  Value* identifier = hm_get(ctx, assignment.identifier);
  if (identifier == NULL && assignment.reassignment) {
    error_log("Can't reassign unbound identifier: %s!\n", assignment.identifier);
    return false;
  }

  Expression res = eval_expression(assignment.rvalue, ctx);
  Value* val;
  switch (res.kind) {
    case EXPR_BOOL:
      val = new_bool_value(res.as.boolean);
      break;
    case EXPR_NUMBER:
      val = new_int_value((int64_t)res.as.number);
      break;
    case EXPR_STRING:
      val = new_string_value(res.as.string);
      break;
    default:
      error_log("expected expression on assignment\n");
      return false;
  }
  hm_set(ctx, assignment.identifier, val);
  //printf("[DEBUG] assigned %s to ", assignment.identifier);
  //print(res);
  return true;
}

static bool execute_while_stmt(WhileStmt loop, HashMap* scope) {
  return false;
}
