/* OSH GRAMMAR */

/* Token */
%{
#include "parser.tab.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
%}

%%
[a-zA-Z_][a-zA-Z0-9_]*		{ yylval.strval = strdup(yytext); return ID; }
[0-9]+                     { yylval.intval = atoi(yytext); return INT; }
"\n"                       { return EOL; }
[ \t]							   {}
.									{ printf("TFM character %c\n",*yytext); }
%%
/*
int main(int argc, char *argv[])
{
	// skip program name
	++argv, --argc;

	if(argc > 0) {
		yyin = fopen(argv[0],"r");
	}
	else {
		yyin = stdin;
	}

	int tok;

	while((tok = yylex())) {
		//printf("%d\n",tok);
	}
}
*/
