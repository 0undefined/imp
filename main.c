#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

#define NUMAST 8

#include "ast.h"
#include "parser.tab.h"

void prompt();

memory *m = NULL;
size_t nast = 0;
extern FILE* yyin;

struct CExpr* ast[NUMAST];

int main (int argc, char* argv[]) {
  int ret = 0;

  setlocale(LC_CTYPE, "");

  if (argc == 2) {
    if (!(yyin = fopen(argv[1], "r"))) {
      perror(argv[1]);
      exit(EXIT_FAILURE);
    }
  }

  m = memory_new(sizeof(struct AExpr) * 1024);

  prompt();
  ret = yyparse();

  if (ret) {
    fprintf(stderr, "failed to parse input\n");
  } else {
    wprintf(L"\n -- \n");
    for (int a = 0; a < nast; a++) {
        if (ast[a] == NULL) printf("()");
        else print_ast(ast[a]);
        wprintf(L"\n");
    }
  }

  return 0;
}
