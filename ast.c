#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <wchar.h>
#include "ast.h"

struct AExpr*
new_num(memory* m, int num) {
  struct AExpr* e = memory_allocate(m, sizeof(struct AExpr)); e->type = AExpr_num;
  e->exp.num = num;
  return e;
}

struct AExpr*
new_loc(memory* m, const char *varname) {
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

struct BExpr*
new_bexp_cst(memory* m, bool cst) {
  struct BExpr* b = memory_allocate(m, sizeof(struct BExpr)); b->type = BExpr_cst;
  b->exp.cst = cst;
  return b;
}

struct BExpr*
new_bexp_unary(memory* m, BExpr_t t, struct BExpr *e) {
  struct BExpr* b = memory_allocate(m, sizeof(struct BExpr)); b->type = t;
  b->exp.unary = e;
  return b;
}

struct BExpr*
new_bexp_binary(memory* m, BExpr_t t, struct BExpr *l, struct BExpr *r) {
  struct BExpr* b = memory_allocate(m, sizeof(struct BExpr)); b->type = t;
  b->exp.binary.l = l;
  b->exp.binary.r = r;
  return b;
}

struct BExpr*
new_bexp_cmp(memory* m, BExpr_t t, struct AExpr *l, struct AExpr *r){
  struct BExpr* b = memory_allocate(m, sizeof(struct BExpr)); b->type = t;
  b->exp.cmp.l = l;
  b->exp.cmp.r = r;
  return b;
}

struct CExpr*
new_cexp_skip(memory* m) {
  struct CExpr* c = memory_allocate(m, sizeof(struct CExpr)); c->type = CExpr_skip;
  return c;
}


struct CExpr*
new_cexp_seq(memory* m, struct CExpr* c0, struct CExpr* c1) {
  struct CExpr* c = memory_allocate(m, sizeof(struct CExpr)); c->type = CExpr_seq;
  c->exp.seq.c0 = c0; c->exp.seq.c1 = c1;
  return c;
}

struct CExpr*
new_cexp_assign(memory* m, const char* varname, struct AExpr* a){
  struct CExpr* c = memory_allocate(m, sizeof(struct CExpr)); c->type = CExpr_assign;
  c->exp.assign.x = varname;
  c->exp.assign.a = a;
  return c;
}

struct CExpr*
new_cexp_cond(memory* m, struct BExpr* b, struct CExpr* istrue, struct CExpr* isfalse) {
  struct CExpr* c = memory_allocate(m, sizeof(struct CExpr)); c->type = CExpr_cond;
  c->exp.cond.b = b;
  c->exp.cond.c0 = istrue;
  c->exp.cond.c1 = isfalse;
  return c;
}

struct CExpr*
new_cexp_while(memory* m, struct BExpr* b, struct CExpr* c0) {
  struct CExpr* c = memory_allocate(m, sizeof(struct CExpr)); c->type = CExpr_while;
  c->exp.loop.b = b;
  c->exp.loop.c = c0;
  return c;
}

struct CExpr*
new_cexp_repeat(memory* m, struct CExpr* c0, struct BExpr* b) {
  struct CExpr* c = memory_allocate(m, sizeof(struct CExpr)); c->type = CExpr_repeat;
  c->exp.loop.b = b;
  c->exp.loop.c = c0;
  return c;
}

bool aexp_eq(struct AExpr* a0, struct AExpr* a1) {
  if (a0->type != a1->type) return false;
  switch (a0->type) {
  case AExpr_num:
    return a0->exp.num == a1->exp.num;

  case AExpr_loc:
    return strcmp(a0->exp.loc, a1->exp.loc);

  case AExpr_add:
  case AExpr_sub:
  case AExpr_mul:
    return aexp_eq(a0->exp.binary.l, a1->exp.binary.l)
        && aexp_eq(a0->exp.binary.r, a1->exp.binary.r);

  default:
    fprintf(stderr, "Unmatched case in %s: %d\n", __func__, a0->type);
    return true;
  }
}

bool bexp_eq(struct BExpr* b0, struct BExpr* b1) {
  if (b0->type != b1->type) return false;
  switch (b0->type) {
  case BExpr_cst:
    return b0->exp.cst == b1->exp.cst;

  case BExpr_neg:
    return bexp_eq(b0->exp.unary, b1->exp.unary);

  case BExpr_and:
    return bexp_eq(b0->exp.binary.l, b1->exp.binary.l)
        && bexp_eq(b0->exp.binary.r, b1->exp.binary.r);

  case BExpr_eq:
  case BExpr_le:
    return aexp_eq(b0->exp.cmp.l, b1->exp.cmp.l)
        && aexp_eq(b0->exp.cmp.r, b1->exp.cmp.r);

  default:
    fprintf(stderr, "Unmatched case in %s: %d\n", __func__, b0->type);
    return true;
  }
}

bool ast_eq(struct CExpr* c0, struct CExpr* c1) {
  if (c0->type != c1->type) return false;
  switch (c0->type) {
  case CExpr_skip:
    return true;

  case CExpr_seq:
    return ast_eq(c0->exp.seq.c0, c1->exp.seq.c0)
        && ast_eq(c0->exp.seq.c1, c1->exp.seq.c1);

  case CExpr_assign:
    return strcmp(c0->exp.assign.x, c1->exp.assign.x)
       && aexp_eq(c0->exp.assign.a, c1->exp.assign.a);

  case CExpr_cond:
    return bexp_eq(c0->exp.cond.b,  c1->exp.cond.b)
        &&  ast_eq(c0->exp.cond.c0, c1->exp.cond.c0)
        &&  ast_eq(c0->exp.cond.c1, c1->exp.cond.c1);

  case CExpr_while:
  case CExpr_repeat:
    return bexp_eq(c0->exp.loop.b, c1->exp.loop.b)
        &&  ast_eq(c0->exp.loop.c, c1->exp.loop.c);


  default:
    fprintf(stderr, "Unmatched case in %s: %d\n", __func__, c0->type);
    return true;
  }
}

void print_aexp(struct AExpr *expr) {
  switch (expr->type) {
  case AExpr_num:
    wprintf(L"%d", expr->exp.num);
    break;

  case AExpr_loc:
    wprintf(L"%s", expr->exp.loc);
    break;

  case AExpr_add:
    wprintf(L"(");
    print_aexp(expr->exp.binary.l);
    wprintf(L" + ");;
    print_aexp(expr->exp.binary.r);
    wprintf(L")");
    break;

  case AExpr_sub:
    wprintf(L"(");
    print_aexp(expr->exp.binary.l);
    wprintf(L" - ");;
    print_aexp(expr->exp.binary.r);
    wprintf(L")");
    break;

  case AExpr_mul:
    wprintf(L"(");
    print_aexp(expr->exp.binary.l);
    wprintf(L" * ");;
    print_aexp(expr->exp.binary.r);
    wprintf(L")");
    break;

  default:
    fprintf(stderr, "Unmatched case in %s: %d\n", __func__, expr->type);
    break;
  }
}

void print_bexp(struct BExpr *expr) {
  switch (expr->type) {
  case BExpr_cst:
    wprintf(L"%s", expr->exp.cst ? "true" : "false");
    break;

  case BExpr_eq:
    print_aexp(expr->exp.cmp.l);
    wprintf(L" = ");
    print_aexp(expr->exp.cmp.r);
    break;

  case BExpr_le:
    print_aexp(expr->exp.cmp.l);
    wprintf(L" ≤ ");
    print_aexp(expr->exp.cmp.r);
    break;

  case BExpr_neg:
    wprintf(L"¬");
    print_bexp(expr->exp.unary);
    break;

  case BExpr_and:
    print_bexp(expr->exp.binary.l);
    wprintf(L" ∧ ");
    print_bexp(expr->exp.binary.r);
    break;

  default:
    fprintf(stderr, "Unmatched case in %s: %d\n", __func__, expr->type);
    break;
  }
}

void print_ast(struct CExpr *expr) {
  static CExpr_t lasttype;
  bool put_closing = lasttype != CExpr_seq;
  lasttype = expr->type;

  switch (expr->type) {
  case CExpr_skip:
    wprintf(L"skip");
    break;

  case CExpr_seq:
    if (put_closing) wprintf(L"(");
    print_ast(expr->exp.seq.c0);
    wprintf(L"; ");
    print_ast(expr->exp.seq.c1);
    if (put_closing) wprintf(L")");
    break;

  case CExpr_assign:
    wprintf(L"%s", expr->exp.assign.x);
    wprintf(L" := ");
    print_aexp(expr->exp.assign.a);
    break;

  case CExpr_cond:
    wprintf(L"if ");
    print_bexp(expr->exp.cond.b);
    wprintf(L" then ");
    print_ast(expr->exp.cond.c0);
    wprintf(L" else ");
    print_ast(expr->exp.cond.c1);
    break;

  case CExpr_while:
    wprintf(L"while ");
    print_bexp(expr->exp.loop.b);
    wprintf(L" do ");
    print_ast(expr->exp.loop.c);
    break;

  case CExpr_repeat:
    wprintf(L"repeat ");
    print_ast(expr->exp.loop.c);
    wprintf(L" while ");
    print_bexp(expr->exp.loop.b);
    break;

  default:
    fprintf(stderr, "Unmatched case in %s: %d\n", __func__, expr->type);
    break;
  }
}
