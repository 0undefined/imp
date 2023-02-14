#include <stdio.h>
#include "ast.h"

struct AExpr*
new_num(memory* m, int num) {
  struct AExpr* e = memory_allocate(m, sizeof(struct AExpr)); e->type = AExpr_num;
  e->exp.num = num;
  return e;
}

struct AExpr*
new_loc(memory* m, char *varname) {
  struct AExpr* e = memory_allocate(m, sizeof(struct AExpr)); e->type = AExpr_loc;
  e->exp.loc = varname;
  return e;
}

struct AExpr*
new_infix(memory* m, AExpr_t t, struct AExpr *l, struct AExpr *r) {
  struct AExpr* e = memory_allocate(m, sizeof(struct AExpr)); e->type = t;
  e->exp.binary.l = l;
  e->exp.binary.r = r;
  return e;
}

void print_ast(struct AExpr *expr) {
  switch (expr->type) {
    case AExpr_num:
      printf("%d", expr->exp.num);
      break;
    case AExpr_loc:
      printf("%s", expr->exp.loc);
      break;
    case AExpr_add:
      putchar('(');
      print_ast((struct AExpr*)expr->exp.binary.l);
      putchar('+');
      print_ast((struct AExpr*)expr->exp.binary.r);
      putchar(')');
      break;
    case AExpr_sub:
      putchar('(');
      print_ast((struct AExpr*)expr->exp.binary.l);
      putchar('-');
      print_ast((struct AExpr*)expr->exp.binary.r);
      putchar(')');
      break;
    case AExpr_mul:
      putchar('(');
      print_ast((struct AExpr*)expr->exp.binary.l);
      putchar('*');
      print_ast((struct AExpr*)expr->exp.binary.r);
      putchar(')');
      break;
    default:
      printf("?");
      break;
  }
}
