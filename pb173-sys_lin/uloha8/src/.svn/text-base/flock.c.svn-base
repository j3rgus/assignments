#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>

int main(void)
{
	int fd;
	int ret;

	fd = open("locked", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR | S_IROTH | S_IRGRP);
	if (fd == -1) {
		perror("locked");
		return 1;
	} else 
		printf("File 'locked' opened\n");

	ret = flock(fd, LOCK_EX);
	if (ret == -1)
		perror("flock");

	(void) getchar();

	ret = flock(fd, LOCK_UN);
	if (ret == -1)
		perror("flock");

	close(fd);
	return 0;
}
