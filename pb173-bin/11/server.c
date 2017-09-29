#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>

#define setop(num, op, flg) {sops.sem_num = num; sops.sem_op = op;\
							sops.sem_flg = flg; }
#define MSGSZ 128

union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
} arg;

struct mymsg {
	long mtype;
	char mtext[MSGSZ];
};

int main(void)
{
	struct sembuf sops;
	struct mymsg mes;
	int semid, msgid, pid;
	int ret, val;
	key_t key;

	key = ftok("..", 'x');
	if (key == -1) {
		perror("ftok");
		return 1;
	}

	semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0660);
	if (semid < 0) {
		perror("semget");
		return 1;
	}

	msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0660);
	if (msgid < 0) {
		perror("msgget");
		return 1;
	}

	arg.val = 5;
	ret = semctl(semid, 0, SETVAL, arg);
	if (ret < 0) {
		perror("semctl");
		return 1;
	}

	while (1) {
		ret = msgrcv(msgid, &mes, MSGSZ, 1, 0);
		if (ret < 0) {
			perror("msgrcv");
			return 1;
		}
		pid = fork();
		if (!pid) {
			val = semctl(semid, 0, GETVAL);
			printf("Free printers: %d\n", val);
			setop(0, -1, 0);
			semop(semid, &sops, 1);
			printf("Printing message: %s.\n", mes.mtext);
			sleep(10);
			setop(0, 1, 0);
			semop(semid, &sops, 1);
			printf("%s printed.\n", mes.mtext);
		}
	}

	return 0;
}
