%{
#include <stdio.h>
#include <stdbool.h>
#include "ast.h"

extern memory *m;
extern struct CExpr *ast;
extern int yylineno;
extern char* yytext;
int yylex();
void yyerror(const char *s);

//%define api.value.type {AExpr}

%}

%start ast


%union {
  int   num;
  bool  cst;
  const char* loc;
  struct {struct AExpr *l; struct AExpr *r;} infix;
  struct AExpr *arithmetic_expression;
  struct BExpr *boolean_expression;
  struct CExpr *command_expression;
}

%type <command_expression>    ast
%type <arithmetic_expression> aexp
%type <boolean_expression>    bexp
%type <command_expression>    cexp

%token LPAR RPAR RCURLY LCURLY
%left SEMICOLON
%nonassoc ASSIGN
%precedence <boolean_expression> NEG
%left <boolean_expression> AND
%left <boolean_expression> EQUAL LEQUAL

%token <cst> TRUE FALSE

%left IF WHILE REPEAT
%nonassoc THEN ELSE DO

%token <cmd> SKIP

%token <loc> VARIABLE

%token <num> NUMBER

%left <mul> MUL
%left <infix> ADD SUB


%token EOL


%%
ast: { $$ = NULL; }
  | cexp EOL { $$ = $1; ast = $$; }
  ;

aexp:
    aexp ADD aexp { $$ = new_infix(m, AExpr_add, $1, $3); }
  | aexp SUB aexp { $$ = new_infix(m, AExpr_sub, $1, $3); }
  | aexp MUL aexp { $$ = new_infix(m, AExpr_mul, $1, $3); }
  | NUMBER        { $$ = new_num(m, $1); }
  | VARIABLE      { $$ = new_loc(m, $1); }
  | LPAR aexp RPAR  { $$ = $2; }
  ;

bexp:
    TRUE             { $$ = new_bexp_cst(m, true); }
  | FALSE            { $$ = new_bexp_cst(m, false); }
  | NEG bexp         { $$ = new_bexp_unary(m, BExpr_neg, $2); }
  | bexp AND bexp    { $$ = new_bexp_binary(m, BExpr_and, $1, $3); }
  | aexp EQUAL  aexp { $$ = new_bexp_cmp(m, BExpr_eq,  $1, $3); }
  | aexp LEQUAL aexp { $$ = new_bexp_cmp(m, BExpr_le,  $1, $3); }
  | LPAR bexp RPAR   { $$ = $2; }
  ;

cexp:
    LPAR cexp RPAR              { $$ = $2; }
  | cexp SEMICOLON cexp         { $$ = new_cexp_seq(m, $1, $3); }
  | SKIP                        { $$ = new_cexp_skip(m); }
  | VARIABLE ASSIGN aexp        { $$ = new_cexp_assign(m, $1, $3); }
  | IF bexp THEN cexp ELSE cexp { $$ = new_cexp_cond(m, $2, $4, $6); }
  | WHILE  bexp DO    cexp      { $$ = new_cexp_while(m, $2, $4); }
  | REPEAT cexp WHILE bexp      { $$ = new_cexp_repeat(m, $2, $4); }
  ;
%%

void
yyerror(const char *s) {
  fprintf(stderr, "Error %d: %s\n", yylineno, s);
}
