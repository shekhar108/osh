/* Grammar for osh */

/* definitions */

BLANK	" "|"\t"
ID		[a-zA-Z_][a-zA-Z_0-9]*
OP		"\n"|"&&"

%%
{BLANK}	{ ECHO; }
{ID}		{ ECHO; }
{OP}		{ ECHO; }
.			{ printf("unknown char!\n"); }
%%

int main(int argc, char *argv[])
{
	/* skip program name */
	++argv, --argc;

	if(argc > 0) {
		yyin = fopen(argv[0],"r");
	}
	else {
		yyin = stdin;
	}
	yylex();

}
