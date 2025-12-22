#include "interpreter.h"
#include "utils/error.h"


// =====================
// Prototypes
// =====================

static Expression eval_binary_expression(Expression left, Expression right, OperatorKind op);
static Expression eval_unary_expression(Expression left, Expression right, OperatorKind op);

// =====================
// Public Functions
// =====================

bool execute(Program* program) {
  Vector items = program->items;
  for (size_t i = 0; i < items.size; i++) {
    TopLevel* item = (TopLevel*) vec_get(&items, i);
    switch (item->kind) {
      case TOP_STATEMENT: {
        Statement* stmt = item->as.statement;
        execute_statement(stmt);
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

bool execute_statement(Statement* statement) {
  StatementKind kind = statement->kind;
  switch (kind) {
    case STMT_EXPR:
      break;
    default:
      break;
  }
  return true;
}

Expression eval_expression(Expression* expression) {
  switch (expression->kind) {
    // default datatypes
    case EXPR_NUMBER: case EXPR_BOOL: case EXPR_STRING:
      return *expression;
    
    case EXPR_BINARY:
      return eval_binary_expression(
        *expression->as.binary.left,
        *expression->as.binary.right, 
        expression->as.binary.op);
    
    case EXPR_UNARY:
      return eval_unary_expression(
        *expression->as.binary.left,
        *expression->as.binary.right, 
        expression->as.binary.op);

    default:
      error_log("Unrecognized expression kind\n");
      break;
  }
}

static Expression eval_binary_expression(Expression left, Expression right, OperatorKind op) {
}

static Expression eval_unary_expression(Expression left, Expression right, OperatorKind op) {

}