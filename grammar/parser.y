/* Parse rules for osh */

%{
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_LINE 1024	/* The maximum length of cmd */
#define MAX_ARGS 80		/* length of each argument */
#define MAX_FNAME 512	/* size of file name */
#define MAX_UNAME 100	/* size of user name */
#define ws " \t\n\v"
#define HOME "HOME"
#define USER "USER"
#define HISTORY "/.osh_history"

void yyerror(char *);
int osh_exec(char *);
int yylex();

%}

/* declare tokens */
%token BLANK 258
%token ID 259
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
command			:	ID argument_list META { printf("Sup !!\n"); }
argument_list	:	{ /*empty argument list */ }
				   | argument_list BLANK argument;
argument			:	ID;
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
