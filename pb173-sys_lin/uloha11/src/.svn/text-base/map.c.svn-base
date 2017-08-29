#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "debug.h"

int main(int argc, char **argv)
{
	char *mem;
	struct stat st;
	int fd;
	int ret, id;

	if (argc < 2)
		exit(1);

	id = atoi(argv[1]);

	if (id == 0) {
		fd = open("myfile", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
		write(fd, "Ahoj", 4);
	} else {
		fd = open("myfile", O_RDONLY);
	}

	printerr_ex((fd == -1), "open", 1);

	ret = fstat(fd, &st);
	printerr_ex((ret == -1), "fstat", 1);

	if (id == 0)
		mem = mmap(0, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	else
		mem = mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);

	printerr_ex((mem == (void *) -1), "mmap", 1);

	if (id == 0) {
		sleep(10);
		strcpy(mem, "Cau");
		ret = msync(mem, st.st_size, MS_SYNC);
		printerr_ex((ret == -1), "msync", 1);
	} else {
		while (id > 0) {
			printf("%s\n", mem);
			sleep(1);
			id--;
		}
	}

	ret = munmap(mem, st.st_size);
	printerr_ex((ret == -1), "munmap", 1);

	close(fd);

	return 0;
}
