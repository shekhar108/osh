/* check history */

#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_LINE 1024	/* The maximum length of cmd */
#define MAX_ARGS 80		/* length of each argument */
#define MAX_FNAME 512	/* size of file name */
#define MAX_UNAME 100	/* size of user name */
#define MAX_PATH 512		/* path to home directory */
#define ws " \t\n\v"
#define HOME "HOME"
#define USER "USER"
#define HISTORY "/.osh_history"

/* command types */
struct cmd_type
{
   char **args; /* command */
   int type;    /* type of command (simple, redirect, pipe etc. ) */
   char **xargs;/* extra arguments */
};

typedef struct cmd_type cmd_type;

/* Built-in function declarations for shell commands */
int osh_exit(char **args);
int osh_cd(char **args);
int osh_history(char **args);

/* List of built-in commands */
char *builtin_cmd[] = {
	"exit",
	"cd",
	"history",
};

/* List of built-in functions */
int (*builtin_func[]) (char **) = {
	&osh_exit,
	&osh_cd,
	&osh_history
};

/*  Number of built-in commands */
int builtin_num = sizeof(builtin_cmd)/sizeof(char*);

/* Implementing built-in functions */
int osh_exit(char **args)
{
	return 0;
}

int osh_cd(char **args)
{
	if(args[1] == NULL) {
		char *path;
		path = malloc(MAX_PATH * sizeof(char));

      if(strcmp(args[0],"cd") == 0) {
	   	strcpy(path,getenv(HOME));
	   	chdir(path);
      }
      else {
         if(chdir(args[0]) == -1) {
            return 0;
         }
      }
	}
	else if(chdir(args[1]) == -1) {
		perror("osh_cd");
      return 0;
	}
	return 1;
}

int osh_history(char **args)
{
	char *cmd;
	cmd = malloc(MAX_LINE * sizeof(char));

	char *fpath;
	fpath = malloc(MAX_FNAME * sizeof(char));
	strcpy(fpath,getenv(HOME));
	strcat(fpath,HISTORY);

	FILE *fp;
	fp = fopen(fpath,"r");
	int i = 0;
	if(fp != NULL) {
		while(fgets(cmd,MAX_LINE,fp) != NULL && ++i) {
			fprintf(stdout,"%d %s",i,cmd);
		}
	}
	fclose(fp);
	free(cmd);
	free(fpath);

	return 1;
}

/*  Read line from stdin */
char *osh_read(void)
{
	char *cmd;	/* command read */
	cmd = malloc(MAX_LINE * sizeof(char));

   char *prompt;
	prompt = malloc(MAX_UNAME * sizeof(char));
	strcpy(prompt,getenv(USER));
   strcat(prompt,":osh$ ");

//	fgets(cmd,MAX_LINE,stdin);
	cmd = readline(prompt);
   return cmd;
}

/* Save history */
int osh_save(char *cmd)
{
  char fpath[MAX_FNAME];

  strcpy(fpath,getenv(HOME));
  strcat(fpath,HISTORY);

  FILE *fp;
  fp = fopen(fpath,"a+");

	if(fp == NULL) {
		perror("osh:");
	}
	else {
      if(strcmp(cmd,"") != 0) {
		   fprintf(fp,"%s\n",cmd);
      }
		fclose(fp);
	}

	return 0;
}

/*  Split commands into arguments */
cmd_type *osh_split(char *cmd)
{
   cmd_type *cargs;

   cargs = malloc(sizeof(cmd_type));
	cargs->type = 0;
   cargs->args = malloc(MAX_ARGS * sizeof(char*));
   cargs->xargs = malloc(MAX_ARGS * sizeof(char*));
   int argc = 0;
   int xargc = 0;
   char *arg;

	arg = strtok(cmd,ws);

	while (cargs->type == 0 && arg != NULL) {

      if(strcmp(arg,">") == 0) {
         cargs->type = 1;
         break;
      }
      if(strcmp(arg,"<") == 0) {
         cargs->type = 2;
         break;
      }
      cargs->args[argc] = arg;
		argc += 1;
		arg = strtok(NULL,ws);
	}

   while(arg != NULL) {
      arg = strtok(NULL,ws);
      cargs->xargs[xargc] = arg;
      xargc += 1;
   }

   return cargs;
}

/* Execute system commands */
int osh_sys(cmd_type *cargs)
{
	pid_t pid;
	pid = fork();

	/* execute command in child process */
	if(pid == 0)
   {
      if(cargs->type == 1)
      {
         FILE *fp = fopen(cargs->xargs[0],"w+");
         if(fp == NULL) {
            perror("osh_sys");
         }
         close(STDOUT_FILENO);
         dup(fileno(fp));
      }
      if(cargs->type == 2)
      {
         FILE *fp = fopen(cargs->xargs[0],"r+");
         if(fp == NULL) {
            perror("osh_sys");
         }
         close(STDIN_FILENO);
         dup(fileno(fp));
      }

      if(execvp(cargs->args[0],cargs->args) == -1) {
         perror("osh_sys");
         exit(0);
		}
	}
	else if(pid < 0) {
		/* fork failed */
		perror("osh_sys");
	}
	else {
      wait(NULL);
	}
	return 1;
}

/* Execute builtin shell commands */
int osh_exec(cmd_type *cargs)
{
	if(cargs->args[0] == NULL) {
		// no command
		return 1;
	}

   /* check builtin commands */
	for(int i=0;i<builtin_num;i++) {
		if(strcmp(cargs->args[0],builtin_cmd[i]) == 0) {
			return (*builtin_func[i])(cargs->args);
		}
	}

   /* try changing directory */
   if(osh_cd(cargs->args) == 1) {
      return 1;
   }

	return osh_sys(cargs);
}

///* support for complex commands eg. redirect */
//int osh_exec2(cmd_type *cargs)
//{
//   /* simple command */
//   if(cargs->type == 0) {
//      return osh_exec(cargs->args);
//   }
//   /* redirect command */
//   if(cargs->type == 1) {
//      osh_redirect(cargs->args,cargs->xargs);
//   }
//   return 0;
//}

/* Initiate shell */
int osh_init()
{
	char *cmd;
	cmd_type *cargs;
	int status;

	do {
		cmd = osh_read();
		osh_save(cmd);
		cargs = osh_split(cmd);
		status = osh_exec(cargs);

		free(cmd);
		free(cargs);
	} while(status);

	return 1;
}

int main(void)
{
	/* initiate shell */
	osh_init();
	return 0;
}
