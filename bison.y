%{
#include <stdio.h>
%}

%token NUMBER
%token ABS
%token MUL DIV
%token ADD SUB
%token EOL

%%
calclist: /* nothing */
  | calclist exp EOL
  ;

exp: factor
   | exp ADD factor { $$ = $1 + $3; }
   | exp SUB factor { $$ = $1 - $3; }
   ;

factor: term
   | factor MUL term { $$ = $1 * $3; }
   | factor DIV term { $$ = $1 / $3; }
   ;

term: NUMBER
   | ABS term  { $$ = $2 >= 0 ? $2 : - $2; }
   ;

%%

int main(int argc, char **argv) {
  yyparse();
}

void yyerror(char *s) {
fprintf(stderr, "Error: %s\n", s);
}
