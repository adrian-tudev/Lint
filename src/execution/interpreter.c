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

// Given a parent scope copy it over to the current scope
// and after executing the current block, update parent scope
bool execute_block(Block* block, HashMap* parent_scope) {
  HashMap* ctx = hm_copy(parent_scope);
  size_t cnt = 0;
  char** global_identifiers = hm_get_keys(parent_scope, &cnt);

  bool success = true;
  Vector stmts = block->statements;
  for (size_t i = 0; i < stmts.size; i++) {
    Statement* stmt = (Statement*) vec_get(&stmts, i);
    if (!execute_statement(stmt, ctx)) {
      success = false;
      break;
    }
  }

  if (success) {
    for (size_t i = 0; i < cnt; i++) {
      Value* new_val = hm_get(ctx, global_identifiers[i]);
      hm_set(parent_scope, global_identifiers[i], copy_value(new_val));
    }
  }

  if (global_identifiers) free(global_identifiers);
  hm_free(ctx);
  return success;
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

  // cannot reassign a nonexisting value
  if (identifier == NULL && assignment.reassignment) {
    error_log("Can't reassign unbound identifier: %s!\n", assignment.identifier);
    return false;
  }

  // disallow variable shadowing
  if (identifier != NULL && !assignment.reassignment) {
    error_log("Redefinition of existing variable: %s!\n", assignment.identifier);
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
  return true;
}

static bool execute_while_stmt(WhileStmt loop, HashMap* scope) {
  return false;
}
