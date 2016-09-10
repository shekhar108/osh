#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_LINE 1024	/* The maximum length of cmd */
#define MAX_ARGS 80		/* length of each argument */
#define ws " \t\n\v"
#define HOME "HOME"
#define USER "USER"

/* Built-in function declarations for shell commands */
int osh_exit(char **args);
int osh_cd(char **args);
int osh_history(char **args);

/* List of built-in commands */
char *builtin_cmd[] = {
	"exit",
	"cd"
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
		chdir(getenv(HOME));
	}
	else if(chdir(args[1]) == -1) {
		perror("osh");
	}
	return 1;
}

int osh_history(char **args)
{
	return 1;
}

/*  Read line from stdin */
char *osh_read(void)
{
	char *cmd;	/* command read */

	cmd = malloc(MAX_LINE * sizeof(char));
	fgets(cmd,MAX_LINE,stdin);
	return cmd;
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
	char *user;

	user = getenv(USER);

	do {
		printf("%s:osh$ ",user);

		cmd = osh_read();
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
