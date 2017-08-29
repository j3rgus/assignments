/**
 *	Jergus Lysy (374217)
 *	Uloha 7.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <openssl/sha.h>

#define OCTET		8
#define RSA_SIZE	2048/OCTET
#define KEY_SIZE	160/OCTET
#define PS_LEN		(RSA_SIZE-KEY_SIZE-3)
#define BT			0x01
#define PS			0xff

unsigned char *get_filehash(char *filename)
{
	int fd;
	unsigned char *buf;
	unsigned char *hash = NULL;
	unsigned int len;
	struct stat s;

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		perror(filename);
		return NULL;
	}

	fstat(fd, &s);
	buf = malloc(s.st_size);
	if (!buf)
		exit(1);

	len = read(fd, buf, s.st_size);
	close(fd);

	hash = SHA1(buf, len, NULL);
	free(buf);

	return hash;
}

unsigned char *get_padblock(unsigned char *hash)
{
	unsigned char *pblock;
	unsigned char *ptr;

	pblock = malloc(RSA_SIZE);
	if (!pblock)
		exit(1);

	ptr = pblock;

/* 00|BT|PS|00|HASH */

	*ptr++ = 0x00;
	*ptr++ = BT;
	memset(ptr, PS, PS_LEN);
	ptr += PS_LEN;
	*ptr++ = 0x00;
	memcpy(ptr, hash, KEY_SIZE);

	return pblock;
}

int main(int argc, char *argv[])
{
	unsigned char *hash;
	unsigned char *pad_block;
	unsigned int i;

	if (argc < 2) {
		printf("File required!\n");
		return 1;
	}

	hash = get_filehash(argv[1]);
	if (!hash)
		return 1;

	pad_block = get_padblock(hash);

	for (i = 0; i < RSA_SIZE; i++) {
		if ((i % 16) == 0)
			printf("\n");
		printf("%02X", pad_block[i]);
	}
	printf("\n\n");

	free(pad_block);
	return 0;
}
