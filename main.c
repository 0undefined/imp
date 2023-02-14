#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "parser.h"

extern FILE* yyin;

struct value_type *ast;

int main (int argc, char* argv[]) {
  int ret = 0;

  if (argc == 2) {
    if (!(yyin = fopen(argv[1], "r"))) {
      perror(argv[1]);
      exit(EXIT_FAILURE);
    }
  }
  ret = yyparse();

  if (ret) {
    perror("failed to parse file");
  } else {
    print_ast(ast);
  }

  return 0;
}
