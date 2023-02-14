#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "parser.tab.h"

memory *m = NULL;
extern FILE* yyin;

struct AExpr *ast;

int main (int argc, char* argv[]) {
  int ret = 0;

  if (argc == 2) {
    if (!(yyin = fopen(argv[1], "r"))) {
      perror(argv[1]);
      exit(EXIT_FAILURE);
    }
  }

  m = memory_new(sizeof(struct AExpr) * 1024);

  ret = yyparse();

  if (ret) {
    perror("failed to parse file");
  } else {
    print_ast(ast);
	printf("\n");
  }

  return 0;
}
