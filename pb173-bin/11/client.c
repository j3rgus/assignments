#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define setop(num, op, flg) { sops.sem_num = num; sops.sem_op = op;\
							sops.sem_flg = flg; }
#define MSGSZ 128

struct mymsg {
	long mtype;
	char mtext[MSGSZ];
};

int main(void)
{
	int msgid;
	int ret;
	key_t key;
	struct mymsg mes;

	key = ftok("..", 'x');
	if (key == -1) {
		perror("ftok");
		return 1;
	}

	msgid = msgget(key, IPC_CREAT | 0660);
	if (msgid < 0) {
		perror("semget");
		return 1;
	}

	mes.mtype = 1;
	strcpy(mes.mtext, "Test strana");

	ret = msgsnd(msgid, &mes, strlen(mes.mtext) + 1, 0);
	if (ret < 0) {
		perror("msgsnd");
		return 1;
	}

	return 0;
}
