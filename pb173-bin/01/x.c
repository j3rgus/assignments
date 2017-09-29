#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <sys/stat.h>

int main(void)
{
	int fd;
	int ret;
	void *mem;
	struct stat st;

	fd = open("x.bin-32bit", O_RDONLY);
	if (fd == -1) {
		perror("x.bin-32bit");
		return 1;
	}

	if (fstat(fd, &st) == -1) {
		perror("fstat");
		return 1;
	}

	mem = mmap(NULL, st.st_size, PROT_EXEC | PROT_READ, MAP_PRIVATE, fd, 0);
	if (mem == MAP_FAILED) {
		perror("mmap");
		return 1;
	}

	/* Funkcia spocita faktorial prveho argumentu a k nemu pricita druhy */
	ret = ((int (*) (int, int))mem)(5, 11);		/* 5! + 11 */

	printf("Result of function is: %d\n", ret);

	if (munmap(mem, st.st_size) == -1) {
		perror("munmap");
		return 1;
	}
	close(fd);
	return 0;
}
