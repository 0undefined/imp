%{
#include "parser.tab.h"
extern void yyerror(const char *message);
%}

%option noyywrap
%%

"("       { return LPAR; }
")"       { return RPAR; }
"{"       { return LCURLY; }
"}"       { return RCURLY; }

"true"    { return TRUE; }
"false"   { return FALSE; }

[ \r\t]     {  }

-?[0-9]+    { yylval.num = atoi(yytext); return NUMBER; }

"+"       { return ADD; }
"-"       { return SUB; }
"*"       { return MUL; }

[a-zA-Z]  { yylval.loc = yytext; return VAR; }

\n        { return EOL; }

%%
