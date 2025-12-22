#ifndef AST_H_
#define AST_H_

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
  STMT_BLOCK,
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
    Block block; // inline block statement
  } as;
};

// =====================
// Functions + Program
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

// =====================
// Constructors / destructors
//
// Ownership model:
// - AST nodes own their child node pointers.
// - AST nodes do NOT own identifier/string memory.
// =====================

Expression *expr_number(double value);
Expression *expr_bool(bool value);
Expression *expr_identifier(const char *name);
Expression *expr_string(const char *value);
Expression *expr_unary(OperatorKind op, Expression *operand);
Expression *expr_binary(OperatorKind op, Expression *left, Expression *right);
void expr_free(Expression *expr);

Block *block_new(void);
void block_init(Block *block);
bool block_add(Block *block, Statement *stmt);
void block_clear(Block *block);
void block_free(Block *block);

Statement *stmt_expr(Expression *expr);
Statement *stmt_assign(const char *identifier, Expression *rvalue);
Statement *stmt_return(Expression *value_or_null);
Statement *stmt_if(Expression *condition, Block *then_body, Block *else_body_or_null);
Statement *stmt_while(Expression *condition, Block *body);
Statement *stmt_block(void);
void stmt_free(Statement *stmt);

Function *function_new(const char *identifier);
bool function_add_param(Function *fn, const char *param);
bool function_add_stmt(Function *fn, Statement *stmt);
void function_free(Function *fn);

Program *program_new(void);
bool program_add_statement(Program *p, Statement *stmt);
bool program_add_function(Program *p, Function *fn);
void program_free(Program *p);

#endif // AST_H_