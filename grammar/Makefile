# makefile for osh interpreter

osh:	oshGrammar.l parser.y
	   bison -d parser.y
		flex oshGrammar.l
		gcc -o $@ parser.tab.c lex.yy.c -ll
