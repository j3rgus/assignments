#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/types.h>

int main(void)
{
	int pid;
	char *const args[] = {"ps", "aux", NULL};

	pid = fork();
	if (pid < 0) {
		printf("Can't fork\n");
		exit(1);
	}
	if (!pid) {
		execv("/bin/ps", args);
	} else {
		waitpid(pid, NULL, WUNTRACED);
		printf("I am parent, pid %d ", getpid());
		printf("Child %d\n", pid);
		exit(0);
	}

	return 0;
}
