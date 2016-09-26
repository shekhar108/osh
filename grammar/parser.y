/* Parse rules for osh */

%{
#include <stdio.h>
#include <string.h>
void yyerror(char *);
int yylex();
%}

%union {
	int intval;
	char *strval;
	float floatval;
   char **list;
}

/* declare tokens */
%token BLANK 258
%token <strval> ID 259
%token <intval> INT 260
%token EOL 261
%token ADD 262
%token SUB 263

%type<strval> expr prog

%%
prog  : prog expr EOL { $$ = $2; printf("prog1: %s\n",$2);}
      | expr EOL  { $$ = $1; printf("prog2: %s\n",$1);}
      ;
expr  : ID            { $$ = $1; }
      | expr ID       { $$ = $1; strcat($$,$2); }
      ;
%%

int main(int argc, char *argv[])
{
	yyparse();
	return 0;
}

void yyerror(char *s)
{
	fprintf(stderr,"error: %s\n",s);
}
