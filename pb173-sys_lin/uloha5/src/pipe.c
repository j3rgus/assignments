#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>

#include "debug.h"

#define NUM 20

char **simple_parse(char *src, int *num)
{
	char **res;
	char *ptr;
	int i;

	res = malloc(sizeof(char *) * NUM);

	ptr = strtok(src, " ");
	for (i = 0; ptr != NULL; i++) {
		if (i >= NUM) {
			printf("Too many parameters!\n");
			exit(1);
		}
		res[i] = malloc(strlen(ptr) + 1);
		strcpy(res[i], ptr);
		ptr = strtok(NULL, " ");
	}
	res[i] = NULL;
	*num = i;

	return res;
}

int main(int argc, char *argv[])
{
	char **args1;
	char **args2;
	int pfd[2], n1, n2, i;
	int ret;
	pid_t child, child2;

	if (argc < 3) {
		printf("Need more arguments!\n");
		return 1;
	}

	ret = pipe(pfd);
	printerr_ex((ret == -1), "pipe", 1);

	args1 = simple_parse(argv[1], &n1);
	args2 = simple_parse(argv[2], &n2);

	child = fork();
	printerr_ex((child == -1), "fork (child)", 1);

	if (child == 0) {
		close(pfd[0]);
		dup2(pfd[1], STDOUT_FILENO);
		ret = execvp(args1[0], args1);
		if (ret == -1)
			printf("%s: %s\n", args1[0], strerror(errno));
	} else {
		child2 = fork();
		printerr_ex((child2 == -1), "fork (child2)", 1);
		if (child2 == 0) {
			close(pfd[1]);
			dup2(pfd[0], STDIN_FILENO);
			ret = execvp(args2[0], args2);
			if (ret == -1)
				printf("%s: %s\n", args2[0], strerror(errno));
		}
	}

	for (i = 0; i < n1; i++)
		free(args1[i]);

	for (i = 0; i < n2; i++)
		free(args2[i]);

	free(args1);
	free(args2);

	wait(NULL);
	return 0;
}
