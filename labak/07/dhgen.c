/**
 *	Jergus Lysy (374217)
 *	Uloha 7.3
 */

#include <stdio.h>
#include <openssl/dh.h>
#include <openssl/err.h>

#define GENERATOR	2
#define PRIME_LEN	2048
#define FILENAME	"params.der"

int main(void)
{
	FILE *fd;
	int ret;
	DH *dh;

	ERR_load_crypto_strings();

	dh = DH_new();
	if (!dh) {
		ERR_print_errors_fp(stderr);
		return 1;
	}

	ret = DH_generate_parameters_ex(dh, PRIME_LEN, GENERATOR, NULL);
	if (!ret) {
		ERR_print_errors_fp(stderr);
		return 1;
	}

	fd = fopen(FILENAME, "w");
	ret = i2d_DHparams_fp(fd, dh);
	if (!ret) {
		ERR_print_errors_fp(stderr);
		return 1;
	}

	fclose(fd);
	DH_free(dh);
	ERR_free_strings();

	return 0;
}
