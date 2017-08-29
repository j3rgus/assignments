#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <openssl/sha.h>

#include "LDSSecurityObject.h"

#define FPATH	"dg/"
#define FNUM	16

uint32_t fmap;
char fname[32];

/*
 *	Funkcia bere poradove cislo suboru
 *	a hash, s ktorym je testovany hash suboru.
 *
 *	Vrati 0 v pripade uspechu, 1 ak subor neexistuje
 *	a -1 ak je hash rozny. Pre hash=NULL vrati iba
 *	0 (subor existuje) a 1 (subor neexistuje).
*/

int check_filehash(int nr, char *hash)
{
	char *buf;
	unsigned char *fhash;
	struct stat s;
	FILE *fd;
	int buflen;

	sprintf(fname, "%sdg%d.bin", FPATH, nr);

	fd = fopen(fname, "rb");
	if (!fd)
		return 1;

	if (!hash) {
		fclose(fd);
		return 0;
	}

	fstat(fileno(fd), &s);

	buf = malloc(s.st_size);
	if (!buf)
		exit(1);

	buflen = fread(buf, 1, s.st_size, fd);
	fclose(fd);

	fhash = SHA1(buf, buflen, NULL);
	free(buf);

	if (strcmp(fhash, hash))
		return -1;

	return 0;
}

int main(void)
{
	unsigned char *buffer;
	int bufflen;
	LDSSecurityObject_t *lds;
	FILE *f;
	struct stat s;
	asn_dec_rval_t rval;
	int i, j, ret;

	lds = (LDSSecurityObject_t*) calloc(1, sizeof (*lds));
	if (!lds)
		exit(1);

	f = fopen("../lds.bin", "rb");
	if (!f) {
		perror("../lds.bin");
		return 1;
	}

	fstat(fileno(f), &s);

	buffer = malloc(s.st_size);
	if (!buffer)
		exit(1);

	bufflen=fread(buffer,1,s.st_size,f);
	fclose(f);

	rval = ber_decode(0, &asn_DEF_LDSSecurityObject, (void**) &lds, buffer, bufflen);
	if (rval.code != RC_OK)
		exit(1);

	free(buffer);

	printf("LDS version: %i\n\n",lds->version);
	for (i = 0; i < lds->dataGroupHashValues.list.count; i++) {
			DataGroupHash_t *dgh = lds->dataGroupHashValues.list.array[i];

			printf("Hash of DataGroup %i: \n",dgh->dataGroupNumber);
			for (j = 0; j < dgh->dataGroupHashValue.size; j++)
				printf("%02X",dgh->dataGroupHashValue.buf[j]);

			ret = check_filehash(dgh->dataGroupNumber, dgh->dataGroupHashValue.buf);

			if (ret <= 0) {
				fmap |= (1 << dgh->dataGroupNumber);
				(ret == -1) ? printf(" NOK") : printf(" OK");
			} else
				printf(" %sdg%d.bin missing!", FPATH, dgh->dataGroupNumber);

			printf("\n\n");
	}

	printf("List of unused files:\n");
	for (i = 1; i <= FNUM; i++) {
		ret = check_filehash(i, NULL);
		if (!ret && !((1 << i) & fmap))
			printf("%sdg%d.bin, ", FPATH, i);
	}
	printf("\n\n");

	return 0;
}
