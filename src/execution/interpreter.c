#include "execution/interpreter.h"

#include "ast/grammar.h"
#include "utils/error.h"

#include "execution/eval.h"

// =====================
// Prototypes
// =====================

static bool execute_assignment(Assignment assignment, HashMap* ctx);
static bool execute_if_stmt(IfStmt stmt);
static bool execute_while_stmt(WhileStmt loop);

// =====================
// Public Functions
// =====================

bool execute(Program* program) {
  Vector items = program->items;
  for (size_t i = 0; i < items.size; i++) {
    TopLevel* item = (TopLevel*) vec_get(&items, i);
    // exit if item is malformed
    if (item == NULL) return false;
    switch (item->kind) {
      case TOP_STATEMENT: {
        Statement* stmt = item->as.statement;
        if (!execute_statement(stmt, program->ctx)) return false;
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
  }
  return true;
}

bool execute_function_def(Function* function) {
  return true;
}

bool execute_block(Block* block) {
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
      eval_expression(statement->as.expr);
      break;
    case STMT_ASSIGN:
      return execute_assignment(statement->as.assignment, scope);
    case STMT_IF:
      return execute_if_stmt(statement->as.if_stmt);
    case STMT_WHILE:
      return execute_while_stmt(statement->as.while_stmt);
    case STMT_BLOCK:
      return execute_block(statement->as.block);
    default:
      break;
  }
  return true;
}

static bool execute_if_stmt(IfStmt stmt) {
  Expression* condition = stmt.condition;
  Expression res = eval_expression(condition);

  if (res.kind != EXPR_BOOL) {
    error_log("Condition in if statement must be boolean expression!\n");
    return false;
  }

  // IF-ELSE execution
  if (res.as.boolean == 1 && stmt.then_body != NULL)
    return execute_block(stmt.then_body);
  else if (res.as.boolean == 0 && stmt.else_body != NULL)
    return execute_block(stmt.else_body);
  
  return true;
}

// store identifier in table
static bool execute_assignment(Assignment assignment, HashMap* ctx) {
  Expression res = eval_expression(assignment.rvalue);
  Value* val;
  switch (res.kind) {
    case EXPR_BOOL:
      val = new_bool_value(res.as.boolean);
      break;
    case EXPR_NUMBER:
      val = new_int_value(res.as.number);
      break;
    case EXPR_STRING:
      val = new_string_value(res.as.string);
      break;
    default:
      error_log("expected expression on assignment\n");
      return false;
  }
  hm_set(ctx, assignment.identifier, val);
  printf("[DEBUG] assigned %s to ", assignment.identifier);
  print(res);
  return true;
}

static bool execute_while_stmt(WhileStmt loop) {
  return false;
}