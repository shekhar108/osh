#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_LINE 80	/* The maximum length of cmd */
#define ws " \t\n\v"

int main(void)
{
	int should_run = 1;	/* flag to determine when to exit program */
	char cmd[MAX_LINE];	/* command read */
	char *args[MAX_LINE/2 + 1];	/* cmd line arguments */
	int argc;	/* number of arguments */
	char *arg;	/* buffer store ws separated args */
	pid_t child; /* child process id */
	int WSIG;  /* wait signal */
	char recent[MAX_LINE][10];	/* 10 recent commmands */

	while (should_run) {
		printf("osh> ");
		fflush(stdout);

		argc = 0;
		WSIG = 1;
		fgets(cmd,MAX_LINE,stdin);	/* read cmd */
		if(strcmp(cmd,"exit\n") == 0) {
			should_run = 0;
			continue;
		}
		else if(strcmp(cmd,"\n") == 0) {
			continue;
		}

		/* parse cmd and arguments */
		args[argc] = strtok(cmd,ws);
		while(args[argc] != NULL) {
			argc += 1;
			args[argc] = strtok(NULL,ws);
		}

		if(strcmp(args[argc-1],"&") == 0) {
			WSIG = 0;
			args[argc-1] = NULL;
		}
		else {
			args[argc] = NULL;
		}

		child = fork();
		if (child < 0) {
			/* if fork fails exit gracefully */
			perror("fork() failed\n");
			return 0;
		}
		else if(child == 0) {
			/* child process */
			if(execvp(args[0], args) == -1) {
				printf("-osh: %s: command not found\n",args[0]);
			}
			exit(0);
		}
		else {
			if(WSIG) {
				wait(NULL);
			}
		}
	}

	return 0;
}
