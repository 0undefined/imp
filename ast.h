#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include "mem.h"

typedef enum {
  AExpr_num,
  AExpr_loc,

  AExpr_add,
  AExpr_sub,
  AExpr_mul,
  AExpr_div,
} AExpr_t;

typedef enum {
  BExpr_cst,
  BExpr_eq,
  BExpr_le,
  BExpr_neg,
  BExpr_and,
} BExpr_t;

typedef enum {
  CExpr_skip,
  CExpr_seq,
  CExpr_assign,
  CExpr_cond,
  CExpr_while,
  CExpr_repeat,
} CExpr_t;

struct AExpr {
  AExpr_t type;
  union {
    int   num;
    const char* loc;
    struct {struct AExpr *l; struct AExpr *r;} binary;
  } exp;
};

struct BExpr {
  BExpr_t type;
  union {
    bool  cst;
    struct BExpr* unary;
    struct {struct BExpr *l; struct BExpr *r;} binary;
    struct {struct AExpr *l; struct AExpr *r;} cmp;
  } exp;
};

struct CExpr {
  CExpr_t type;
  union {
    struct {struct CExpr* c0; struct CExpr* c1;} seq;
    struct {const char*   x;  struct AExpr* a;} assign;
    struct {struct BExpr* b;  struct CExpr* c0; struct CExpr* c1;} cond;
    struct {struct BExpr* b;  struct CExpr* c;} loop;
  } exp;
};

struct AExpr*
new_num(memory* m, int num);

struct AExpr*
new_loc(memory* m, const char *varname);

struct AExpr*
new_infix(memory* m, AExpr_t t, struct AExpr *l, struct AExpr *r);

struct BExpr*
new_bexp_cst(memory* m, bool cst);

struct BExpr*
new_bexp_unary(memory* m, BExpr_t t, struct BExpr *e);

struct BExpr*
new_bexp_binary(memory* m, BExpr_t t, struct BExpr *l, struct BExpr *r);

struct BExpr*
new_bexp_cmp(memory* m, BExpr_t t, struct AExpr *l, struct AExpr *r);

struct CExpr*
new_cexp_skip(memory* m);

struct CExpr*
new_cexp_seq(memory* m, struct CExpr* c0, struct CExpr* c1);

struct CExpr*
new_cexp_assign(memory* m, const char* varname, struct AExpr* a);

struct CExpr*
new_cexp_cond(memory* m, struct BExpr* b, struct CExpr* istrue, struct CExpr* isfalse);

struct CExpr*
new_cexp_while(memory* m, struct BExpr* b, struct CExpr* c0);

struct CExpr*
new_cexp_repeat(memory* m, struct CExpr* c0, struct BExpr* b);


void print_aexp(struct AExpr *expr);
void print_bexp(struct BExpr *expr);
void print_ast(struct CExpr *expr);

#endif
