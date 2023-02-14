%{
#include <stdio.h>
#include "mem.c"


memory *m = NULL;

#define MkExpr(t)\
   AExpr* e = memory_allocate(m, sizeof(AExpr)); e->type = AExpr_##t

/* Use for parsing infix notation */
#define MkExprBin(t)\
   AExpr* e = memory_allocate(m, sizeof(AExpr)); e->type = AExpr_##t; e->exp.t.l = $1; e->exp.t.r = $3

/* Use for parsing stuff like "(+) x y" */
#define MkExprBinPrefix(t)\
   AExpr* e = memory_allocate(m, sizeof(AExpr)); e->type = AExpr_##t; e->exp.t.l = $2; e->exp.t.r = $3

%define api.value.type {AExpr}

%}

%token LPAR RPAR

%token TRUE FALSE

%token FLOAT
%token NUMBER

%left MUL
%left ADD SUB

%token EOL

%%
calclist: %empty
  | calclist exp EOL { $$ = $2; }
  ;

exp: factor
   | exp ADD factor { MkExprBin(add); $$ = e; }
   | exp SUB factor { MkExprBin(sub); $$ = e; }
   ;

factor: term
   | factor MUL term { MkExprBin(mul); $$ = e; }
   ;

term: NUMBER { MkExpr(num); e->exp.num; $$ = e; }

   | LPAR exp RPAR { $$ = $2; }
   ;

%%

/*
int main(int argc, char **argv) {
  m = memory_new(sizeof(AExpr) * 1024);

  char x = yyparse();
}*/

void
yyerror(const char *s) {
  fprintf(stderr, "Error %d: %s\n", yylineno, s);
}
