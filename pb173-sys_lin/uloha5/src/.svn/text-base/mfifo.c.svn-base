#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <errno.h>

#include "debug.h"

#define NUM 20
#define PIPE_PATH "named_pipe"

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
	int pipe, fi, fo;
	int ret, n1, n2, i;
	pid_t child, child2;

	if (argc < 3) {
		printf("Need more arguments!\n");
		return 1;
	}

	args1 = simple_parse(argv[1], &n1);
	args2 = simple_parse(argv[2], &n2);

	errno = 0;
	pipe = mkfifo(PIPE_PATH, S_IWUSR | S_IRUSR);
	printerr_ex((pipe == -1) && !(errno & EEXIST), "mkfifo", 1);

	child = fork();
	printerr_ex((child == -1), "fork (child)", 1);

	if (child == 0) {
		fo = open(PIPE_PATH, O_WRONLY);
		dup2(fo, STDOUT_FILENO);
		ret = execvp(args1[0], args1);
		if (ret == -1)
			printf("%s: %s\n", args1[0], strerror(errno));
	} else {
		child2 = fork();
		printerr_ex((child2 == -1), "fork (child2)", 1);
		if (child2 == 0) {
			fi = open(PIPE_PATH, O_RDONLY);
			dup2(fi, STDIN_FILENO);
			ret = execvp(args2[0], args2);
			if (ret == -1)
				printf("%s: %s\n", args2[0], strerror(errno));
		}
	}

	wait(NULL);

	for (i = 0; i < n1; i++)
		free(args1[i]);

	for (i = 0; i < n2; i++)
		free(args2[i]);

	free(args1);
	free(args2);

	unlink(PIPE_PATH);
	return 0;
}
