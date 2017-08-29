#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <unistd.h>

#include "debug.h"

int main(int argc, char **argv)
{
	struct shmid_ds shm;
	char *mem;
	key_t shmkey;
	int shmid, id;
	int ret;

	if (argc < 2)
		exit(1);

	id = atoi(argv[1]);

	shmkey = ftok("myfile", 1);
	printerr_ex((shmkey == -1), "ftok", 1);

	if (id == 0) {
		shmid = shmget(shmkey, getpagesize(), IPC_CREAT | S_IRUSR | S_IWUSR);
		printerr_ex((shmid == -1), "shmget", 1);
	} else {
		shmid = shmget(shmkey, getpagesize(), S_IRUSR | S_IWUSR);
		printerr_ex((shmid == -1), "shmget", 1);
	}

	mem = shmat(shmid, 0, 0);
	printerr_ex((mem == (void *) -1), "shmat", 1);

	if (id == 0) {
		strcpy(mem, "ahoj");
		sleep(10);
		strcpy(mem, "Cau");
	} else {
		while (id > 0) {
			printf("%s\n", mem);
			sleep(1);
			id--;
		}
	}

	shmctl(shmid, IPC_STAT, &shm);

	if (shmctl(shmid, IPC_STAT, &shm) == -1 ||
		shmctl(shmid, IPC_RMID, &shm) == -1)
	perror("shmctl FAILURE");

	ret = shmdt((void *) mem);
	printerr_ex((ret == -1), "shmdt", 1);

	return 0;
}
