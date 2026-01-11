#include "execution/interpreter.h"

#include "ast/grammar.h"
#include "utils/error.h"

#include "execution/eval.h"

// =====================
// Prototypes
// =====================

static bool execute_assignment(Assignment assignment);
static bool execute_if_stmt(IfStmt stmt);

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
        if (!execute_statement(stmt)) return false;
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
    if (!execute_statement(stmt)) return false;
  }
  return true;
}

bool execute_statement(Statement* statement) {
  StatementKind kind = statement->kind;
  switch (kind) {
    case STMT_EXPR:
      eval_expression(statement->as.expr);
      break;
    case STMT_ASSIGN:
      return execute_assignment(statement->as.assignment);
    case STMT_IF:
      return execute_if_stmt(statement->as.if_stmt);
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
static bool execute_assignment(Assignment assignment) {
  printf("[DEBUG] assigned %s to ", assignment.identifier);
  print(eval_expression(assignment.rvalue));
  return true;
}

