/* OSH GRAMMAR */

/* Token */
%{
#include "parser.tab.h"
#include <string.h>
%}

%option case-insensitive

%%
[a-zA-Z_][a-zA-Z0-9_]*		{ yylval.strval = strdup(yytext); return ID; }
[;|&]								{ return META; }
[ \t\n]							{ }
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
