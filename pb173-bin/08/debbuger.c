#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <errno.h>

void wsigtrap(int stat)
{
	int ret;

	do {
		ret = wait(&stat);
		if (ret == -1) {
			perror("wait");
			exit(1);
		}
	} while (!WIFSTOPPED(stat));
}

void child_trace(void)
{
	int ret;

	ret = ptrace(PTRACE_TRACEME, 0, NULL, NULL);
	if (ret == -1) {
		perror("ptrace(PTRACE_TRACEME)");
		exit(1);
	}
	ret = execl("./put", "./put", NULL);
	if (ret == -1) {
		perror("execl");
		exit(1);
	}
}

int main()
{
	int ret, stat;
	pid_t pid;
	struct user_regs_struct regs;
	long eip;

	pid = fork();
	if (pid == -1) {
		perror("fork");
		return 1;
	}

	if (pid == 0) {
		child_trace();
	} else {
		wsigtrap(stat);
		ptrace(PTRACE_CONT, pid, NULL, NULL);
		wsigtrap(stat);

		printf("Caught SIGTRAP\n");
		ret = ptrace(PTRACE_GETREGS, pid, 0, &regs);
		if (ret == -1) {
			perror("ptrace(PTRACE_GETREG)");
			return 1;
		}

		errno = 0;
		eip = ptrace(PTRACE_PEEKTEXT, pid, regs.eip-1, 0);
		if (errno) {
			perror("ptrace(PTRACE_PEEKTEXT)");
			return 1;
		}

		if ((eip & 0xff) == 0xcc) {
			eip = (eip & ~0xff) | 0x90;
			printf("Instruction changed\n");
			ptrace(PTRACE_POKETEXT, pid, regs.eip-1, eip);
		}

		ptrace(PTRACE_CONT, pid, NULL, NULL);

		do {
			wait(&stat);
		} while (!WIFEXITED(stat));
	}

	return 0;
}
