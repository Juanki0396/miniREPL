#ifndef SYNTAX_TREE_H
#def SYNTAX_TREE_H

typedef enum Expr_Type {
  BINARY;
  GROUPING;
  UNARY;
  LITERAL;
} Expr_Type_e;

typedef union Syntax {
  Binary_s binary;
  Grouping_s grouping;
  Unary_s unary;
  Literal_s literal;
} Syntax_u;

typedef struct Expresion {
  Expr_Type_e type;
  Syntax_u object
} Expr_s;

typedef struct Binary {
  Expr_s left;
  Token_s operator;
  Expr_s right;
  bool valid;
} Binary_s;

typedef struct Grouping {
  Expr_s expression;
  bool valid;
} Grouping_s;

typedef struct Unary {
  Token_s operator;
  Expr_s right;
  bool valid;
} Unary_s;

typedef struct Literal {
  Token_s object;
  bool valid;
} Literal_s;

const char* visit_binary_constcharptr(Binary_s expr);
const char* visit_grouping_constcharptr(Grouping_s expr);
const char* visit_unary_constcharptr(Unary_s expr);
const char* visit_literal_constcharptr(Literal_s expr);
void print_expr(Expr_s expr);

#endif
