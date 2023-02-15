%{
#include <stdbool.h>
#include "parser.tab.h"
extern void yyerror(const char *message);
%}

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

("~"|"¬")       { return NEG; }
"/\\"     { return AND; }
"="       { return EQUAL; }
("<="|"≤")       { return LEQUAL; }

"skip"    { return SKIP; }
":="      { return ASSIGN; }
";"       { return SEMICOLON; }
"if"      { return IF; }
"then"    { return THEN; }
"else"    { return ELSE; }
"while"   { return WHILE; }
"do"      { return DO; }
"repeat"  { return REPEAT; }

[a-zA-Z]+  { yylval.loc = strdup(yytext); return VARIABLE; }

[\n]        { return EOL; }

%%
