%{
#include <stdio.h>
#include "ast.h"

extern memory *m;
extern struct AExpr *ast;
extern int yylineno;
extern char* yytext;
int yylex();
void yyerror(const char *s);

//%define api.value.type {AExpr}

%}

%start ast


%union {
  int   num;
  char* loc;
  struct {struct AExpr *l; struct AExpr *r;} infix;
  struct AExpr *expression;
}

%token LPAR RPAR RCURLY LCURLY

%token TRUE FALSE

%token <num> NUMBER
%token <loc> VAR

%left <mul> MUL
%left <infix> ADD SUB

%token EOL

%type <expression> ast
%type <expression> exp factor term


%%
ast: { $$ = NULL; }
  | exp EOL { $$ = $1; ast = $$; }
  ;

exp: factor
   | exp ADD factor { $$ = new_infix(m, AExpr_add, $1, $3); }
   | exp SUB factor { $$ = new_infix(m, AExpr_sub, $1, $3); }
   // | exp SUB factor { MkExprBin(sub); $$ = e; }
   ;

factor: term
   | factor MUL term { $$ = new_infix(m, AExpr_mul, $1, $3); }
   ;

term: NUMBER { $$ = new_num(m, $1); }

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
