#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <readline/readline.h>

#define MAX_LINE 1024	/* The maximum length of cmd */
#define MAX_ARGS 80		/* length of each argument */
#define MAX_FNAME 512	/* size of file name */
#define MAX_UNAME 100	/* size of user name */
#define MAX_PATH 512		/* path to home directory */
#define ws " \t\n\v"
#define HOME "HOME"
#define USER "USER"
#define HISTORY "/.osh_history"

/* helper functions for shell */
char *escapechars(char *str)
{
	return str;
}

/* Built-in function declarations for shell commands */
int osh_exit(char **args);
int osh_cd(char **args);
int osh_history(char **args);

/* List of built-in commands */
char *builtin_cmd[] = {
	"exit",
	"cd",
	"history"
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

		strcpy(path,getenv(HOME));
		strcat(path,getenv(USER));
		chdir(path);
	}
	else if(chdir(args[1]) == -1) {
		perror("osh");
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
		fprintf(fp,"%s",cmd);
		fclose(fp);
	}

	return 0;
}

/*  Split commands into arguments */
char **osh_split(char *cmd)
{
	char **args;
	args = malloc(MAX_ARGS * sizeof(char*));
	int argc = 0;

	args[argc] = strtok(cmd,ws);

	while (args[argc] != NULL) {
		argc += 1;
		args[argc] = strtok(NULL,ws);
	}

	return args;
}

/* Execute system commands */
int osh_sys(char **args)
{
	pid_t pid;
	pid = fork();

	/* execute command in child process */
	if(pid == 0) {
		if(execvp(args[0],args) == -1) {
         /* if exec fails, try cd */
         perror("osh");
         exit(0);
		}
	}
	else if(pid < 0) {
		/* fork failed */
		perror("osh");
	}
	else {
      wait(NULL);
	}
	return 1;
}

/* Execute builtin shell commands */
int osh_exec(char **args)
{
	if(args[0] == NULL) {
		// no command
		return 1;
	}

	for(int i=0;i<builtin_num;i++) {
		if(strcmp(args[0],builtin_cmd[i]) == 0) {
			return (*builtin_func[i])(args);
		}
	}

	return osh_sys(args);
}

/* Initiate shell */
int osh_init()
{
	char *cmd;
	char **args;
	int status;

	do {
		cmd = osh_read();
		osh_save(cmd);
		args = osh_split(cmd);
		status = osh_exec(args);

		free(cmd);
		free(args);
	} while(status);

	return 1;
}

int main(void)
{
	/* initiate shell */
	osh_init();
	return 0;
}
