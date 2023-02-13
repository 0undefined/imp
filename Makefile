test: bison.y test.l
	bison -d bison.y
	flex test.l
	cc -o $@ bison.tab.c lex.yy.c -lfl
