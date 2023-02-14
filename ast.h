#ifndef AST_H
#define AST_H

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
  i64, i32, i16, i8,
  u64, u32, u16, u8,
  f64, f32, f16, f8,
  bool,
} type_t;

struct AExpr {
  AExpr_t type;
  union {
    int   num;
    char* loc;
    struct {struct AExpr *l; struct AExpr *r;} binary;
  } exp;
};

struct AExpr*
new_num(memory* m, int num);

struct AExpr*
new_loc(memory* m, char *varname);

struct AExpr*
new_infix(memory* m, AExpr_t t, struct AExpr *l, struct AExpr *r);

void print_ast(struct AExpr *expr);

#endif
