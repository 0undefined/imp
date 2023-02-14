%{
#include "bison.tab.h"
%}

%%

"+"       { return ADD; }
"-"       { return SUB; }
"*"       { return MUL; }

"("       { return LPAR; }
")"       { return RPAR; }

-?[0-9]+    { yylval = atoi(yytext); return NUMBER; }

"true"    { return TRUE; }
"false"   { return FALSE; }

[a-zA-Z]  { yylval = atoi(yytext); return NUMBER; }

[ \t]     {  }
\n        { return EOL; }

%%
