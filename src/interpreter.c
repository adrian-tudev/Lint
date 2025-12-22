#include "interpreter.h"
#include "utils/error.h"


// =====================
// Prototypes
// =====================

static Expression eval_binary_expression(Expression left, Expression right, OperatorKind op);
static Expression eval_unary_expression(Expression expr, OperatorKind op);

static Expression bool_expr(bool value);
static Expression num(double value);

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
        *expression->as.unary.operand,
        expression->as.unary.op);

    default:
      error_log("Unrecognized expression kind\n");
      return bool_expr(false);
  }
}

// =====================
// Private Functions
// =====================

static Expression num(double value) {
  Expression result;
  result.kind = EXPR_NUMBER;
  result.as.number = value;
  return result;
}

static Expression bool_expr(bool value) {
  Expression result;
  result.kind = EXPR_BOOL;
  result.as.boolean = value;
  return result;
}

// recursively evaluate a binary expression
static Expression eval_binary_expression(Expression left, Expression right, OperatorKind op) {
  Expression l = eval_expression(&left);
  Expression r = eval_expression(&right);

  // TODO:  type checking
  switch (op) {
    case OP_ADD:
      if (l.kind != r.kind) {
        error_log("Type mismatch in binary expression\n");
        break;
      }
      return num(l.as.number + r.as.number);
    case OP_SUB:
      return num(l.as.number - r.as.number);
    case OP_MUL:
      return num(l.as.number * r.as.number);
    case OP_DIV:
      if (r.as.number == 0) {
        error_log("Division by zero\n");
        break;
      }
      return num(l.as.number / r.as.number);

    case OP_EQUAL:
      return bool_expr(l.as.boolean == r.as.boolean);
    case OP_NOT_EQUAL:
      return bool_expr(l.as.boolean != r.as.boolean);

    case OP_LESS_THAN:
      return bool_expr(l.as.number < r.as.number);
    case OP_LESS_OR_EQUAL:
      return bool_expr(l.as.number <= r.as.number);

    case OP_GREATER_THAN:
      return bool_expr(l.as.number > r.as.number);
    case OP_GREATER_OR_EQUAL:
      return bool_expr(l.as.number >= r.as.number);
    
    case OP_AND:
      return bool_expr(l.as.boolean && r.as.boolean);
    case OP_OR:
      return bool_expr(l.as.boolean || r.as.boolean);

    default:
      error_log("Unsupported binary operator\n");
      break;
  }

  return bool_expr(false);
}

static Expression eval_unary_expression(Expression expr, OperatorKind op) {
  Expression e = eval_expression(&expr);
  switch (op) {
    case OP_NOT:
      return bool_expr(!e.as.boolean);
    case OP_SUB:
      return num(-e.as.number);
  }

  return bool_expr(false);
}