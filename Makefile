default: test

parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: scanner.l parser.tab.h
	flex scanner.l

%.c: %.h

test: parser.tab.h lex.yy.c main.c mem.c ast.c
	cc -g -Og -o $@ parser.tab.c lex.yy.c main.c mem.c ast.c -lfl

clean:
	rm -f lex.yy.c parser.tab.c parser.tab.h test
