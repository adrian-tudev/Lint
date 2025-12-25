#ifndef GRAMMAR_TYPES_H_
#define GRAMMAR_TYPES_H_

#include <stdbool.h>
#include <stdint.h>

#include "utils/vector.h"

// =====================
// Prototypes
// =====================

typedef struct Expression Expression;
typedef struct Statement Statement;
typedef struct Block Block;
typedef struct Function Function;
typedef struct Program Program;

// =====================
// Operators
// =====================

typedef enum {
  // arithmetic operators
  OP_ADD,
  OP_MUL,
  OP_SUB,
  OP_DIV,

  // boolean operators
  OP_AND,
  OP_OR,
  OP_NOT,
  OP_EQUAL,

  OP_NOT_EQUAL,
  OP_LESS_THAN,
  OP_LESS_OR_EQUAL,
  OP_GREATER_THAN,
  OP_GREATER_OR_EQUAL,
} OperatorKind;

// =====================
// Expressions
// =====================

typedef enum {
  EXPR_NUMBER,
  EXPR_BOOL,
  EXPR_IDENTIFIER,
  EXPR_STRING,
  EXPR_UNARY,
  EXPR_BINARY,
  EXPR_INVALID,
} ExpressionKind;

struct Expression {
  ExpressionKind kind;
  uint32_t row;
  uint32_t column;

  union {
    double number;
    bool boolean;
    const char *identifier;
    const char *string;
    struct {
      OperatorKind op;
      Expression *operand;
    } unary;
    struct {
      OperatorKind op;
      Expression *left;
      Expression *right;
    } binary;
  } as;
};

// =====================
// Statements
// =====================

typedef struct {
  const char *identifier;
  Expression *rvalue;
} Assignment;

typedef struct {
  // indicates if there is a return value (void vs value)
  bool has_value;
  Expression *value;
} ReturnStmt;

struct Block {
  // Vector<Statement*>
  Vector statements;
};

typedef struct {
  Expression *condition;
  Block *then_body;
  Block *else_body; // optional
} IfStmt;

typedef struct {
  Expression *condition;
  Block *body;
} WhileStmt;

typedef enum {
  STMT_EXPR,
  STMT_ASSIGN,
  STMT_RETURN,
  STMT_IF,
  STMT_WHILE,
  // STMT_BLOCK,
} StatementKind;

struct Statement {
  StatementKind kind;
  uint32_t row;
  uint32_t column;

  union {
    Expression *expr;
    Assignment assignment;
    ReturnStmt ret;
    IfStmt if_stmt;
    WhileStmt while_stmt;
    // Block block; // inline block statement
  } as;
};

// =====================
// Function + Main Program
// =====================

struct Function {
  const char *identifier;
  // Vector<const char*> (parameter names)
  Vector params;
  Block body;
};

typedef enum {
  TOP_STATEMENT,
  TOP_FUNCTION,
} TopLevelKind;

typedef struct {
  TopLevelKind kind;
  union {
    Statement *statement;
    Function *function;
  } as;
} TopLevel;

struct Program {
  // Vector<TopLevel*>
  Vector items;
};


#endif // GRAMMAR_TYPES_H_