default: test

parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: scanner.l parser.tab.h
	flex scanner.l

test: parser.tab.h lex.yy.c
	cc -o $@ parser.tab.c lex.yy.c main.c mem.c ast.c -lfl
