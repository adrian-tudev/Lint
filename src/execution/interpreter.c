#include "execution/interpreter.h"

#include "ast/grammar.h"
#include "utils/error.h"

// =====================
// Prototypes
// =====================

static Expression eval_binary_expression(Expression left, Expression right, OperatorKind op);
static Expression eval_unary_expression(Expression expr, OperatorKind op);
static bool execute_assignment(Assignment assignment);
static bool execute_if_stmt(IfStmt stmt);

static Expression bool_expr(bool value);
static Expression num(double value);

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

Expression eval_expression(Expression* expression) {
  if (expression == NULL) return bool_expr(false);
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
    case EXPR_IDENTIFIER:
      printf("not implemented yet\n");
      break;
    default:
      error_log("Unrecognized expression kind: %d\n", expression->kind);
  }
  // TODO: return invalid expr
  return bool_expr(false);
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

static Expression invalid_expr(void) {
  Expression result;
  result.kind = EXPR_INVALID;
  return result;
}

// recursively evaluate a binary expression
static Expression eval_binary_expression(Expression left, Expression right, OperatorKind op) {
  Expression l = eval_expression(&left);
  Expression r = eval_expression(&right);

  if (l.kind != r.kind) {
    error_log("Type mismatch in binary operation!\n");
    goto ret_error;
  }

  if (l.kind == EXPR_NUMBER && l.kind == r.kind && !is_numerical_op(op)) {
    error_log("Can't perform boolean operation on numericals.\n");
    goto ret_error;
  }

  if (l.kind == EXPR_BOOL && l.kind == r.kind && !is_boolean_op(op)) {
    error_log("Can't perform numerical operation on booleans.\n");
    goto ret_error;
  }


  switch (op) {
    case OP_ADD:
      return num(l.as.number + r.as.number);
    case OP_SUB:
      return num(l.as.number - r.as.number);
    case OP_MUL:
      return num(l.as.number * r.as.number);
    case OP_DIV:
      if (r.as.number == 0) {
        error_log("Division by zero not allowed.\n");
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

ret_error:

  return invalid_expr();
}

static Expression eval_unary_expression(Expression expr, OperatorKind op) {
  Expression e = eval_expression(&expr);
  switch (op) {
    case OP_NOT:
      return bool_expr(!e.as.boolean);
    case OP_SUB:
      return num(-e.as.number);
    case OP_ADD:
      return num(+e.as.number);
    default:
      error_log("Unsupported unary operator\n");
      break;
  }

  return bool_expr(false);
}
