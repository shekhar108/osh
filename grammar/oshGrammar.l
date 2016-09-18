/* OSH GRAMMAR */

/* Token */
%{
#include "parser.tab.h"
%}

%%
[ \t]								{ return BLANK; }
[dD][oO]							{ return DO; }
[dD][oO][nN][eE]				{ return DONE; }
[iI][fF]							{ return IF; }
[tT][hH][eE][nN]				{ return THEN; }
[eE][lL][iI][fF]				{ return ELIF; }
[eE][lL][sS][eE]				{ return ELSE; }
[fF][iI]							{ return FI; }
[fF][oO][rR]					{ return FOR; }
[fF][uU][nN][cC][tT][iI][oO][nN]	{return FUNC; }
[wW][hH][iI][lL][eE]			{ return WHILE; }
"{"								{ return LBRACE;}
"}"								{ return RBRACE; }
[a-zA-Z_][a-zA-Z0-9_]*		{ return ID; }
[ \t\n;()<>|&]					{ return META; }
["||"!"&&"&;";;"|"|&"()]	{ return OP; }
.									{ printf("TF Mytery character %c\n",*yytext); }
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
		printf("%d\n",tok);
	}
}
*/
