/* Parse rules for osh */

%{
#include <stdio.h>
void yyerror(char *);
int yylex();
%}

%union {
	int intval;
	char *strval;
	float floatval;
}

/* declare tokens */
%token BLANK 258
%token <strval> ID 259
%token META 260
%token OP 261
%token DO 262
%token DONE 263
%token IF 264
%token THEN 265
%token ELIF 266
%token ELSE 267
%token FI 268
%token FOR 269
%token FUNC 270
%token WHILE 271
%token LBRACE 272
%token RBRACE 273

%%
command			: simple_command '\n'
					| '\n' {printf("no command\n");}
					;
simple_command	: ID '\n'
					| simple_command ID '\n'
					;
%%

int main(int argc, char *argv[])
{
	if(yyparse()) {
		printf("error\n");
		return 0;
	}
	return 0;
}

void yyerror(char *s)
{
	fprintf(stderr,"error: %s\n",s);
}
