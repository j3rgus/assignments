/**
 *	Jergus Lysy (374217)
 *	Zadanie 2
 */

#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

#define SIZE_BITS	2048
#define EXP			17

int main(void)
{
	RSA *rsa;

	ERR_load_crypto_strings();

	rsa = RSA_generate_key(SIZE_BITS, EXP, NULL, NULL);
	if (rsa == NULL) {
		ERR_print_errors_fp(stderr);
		return 1;
	}

	if (!RSA_print_fp(stdout, rsa, 0))
		fprintf(stderr, "Couldn't print key!\n");

	RSA_free(rsa);
	ERR_free_strings();

	return 0;
}
