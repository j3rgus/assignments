#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/cdrom.h>

int main(void)
{
	int fd;

	fd = open("/dev/sr0", O_RDWR|O_NONBLOCK);
	ioctl(fd, CDROMEJECT, 0);
	close(fd);

	return 0;
}
