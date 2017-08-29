#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "RSAPrivateKey.h"

uint32_t get_num(INTEGER_t *in)
{
	uint32_t num = 0;
	int i;

	for (i = 0; i < in->size; i++) {
		num <<= 8;
		num |= in->buf[i];
	}

	return num;
}

int main(int argc, char **argv)
{
	int fd;
	char *buffer;
	char num[10];
	unsigned int buflen;
	struct stat st;
	RSAPrivateKey_t *rsapk = 0;
	asn_dec_rval_t rval;

	if (argc != 2) {
		fprintf(stderr, "Need RSA private key as an argument\n");
		return 1;
	}

	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror(argv[1]);
		return 1;
	}

	fstat(fd, &st);

	buffer = malloc(st.st_size);
	if (!buffer)
		exit(1);

	buflen = read(fd, buffer, st.st_size);
	close(fd);

	rval = ber_decode(0, &asn_DEF_RSAPrivateKey, (void **) &rsapk, buffer, buflen);
	if (rval.code != RC_OK)
		exit(1);

	printf("Public exponent of %s is %u.\n", argv[1], get_num(&rsapk->publicExponent));

	free(buffer);
	asn_DEF_RSAPrivateKey.free_struct(&asn_DEF_RSAPrivateKey, rsapk, 0);

	return 0;
}
