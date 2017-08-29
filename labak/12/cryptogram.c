#include <stdio.h>
#include <string.h>
#include <openssl/des.h>

#define INLINES_NUM		4
#define CHALLENGE_SIZE	8
#define S_KEY_LEN		16
#define DER_DATA_LEN	(CHALLENGE_SIZE * 2)
#define CRYPTG_LEN		8
#define BASE_SIZE		24
#define CARD_CRYPTGRAM	0
#define HOST_CRYPTGRAM	1

char hch_in[CHALLENGE_SIZE];
char cch_in[CHALLENGE_SIZE];
char enc_in[S_KEY_LEN];
char mac_in[S_KEY_LEN];

void print_hex(char *title, char *data, size_t size)
{
	int i;

	printf("%s: ", title);
	for (i = 0; i < size; i++) {
		printf("%02X ", (unsigned char) data[i]);
	}
	printf("\n");
}

void parse_to_array(char *in, char *out)
{
	char *ptr, *err;
	long num, i = 0;

	ptr = strtok(in, " ");
	while (ptr != NULL) {
		num = strtol(ptr, &err, 16);
		out[i++] = num;
		ptr = strtok(NULL, " ");
	}
}

void get_encks_der_file(FILE *fd, DES_key_schedule *enc1,
		DES_key_schedule *enc2, char *der)
{
	char buf[128];
	char *ptr = der;
	char key[S_KEY_LEN / 2] = {0};

	fgets(buf, 128, fd);
	parse_to_array(buf, hch_in);
	fgets(buf, 128, fd);
	parse_to_array(buf, cch_in);
	fgets(buf, 128, fd);
	parse_to_array(buf, enc_in);
	fgets(buf, 128, fd);
	parse_to_array(buf, mac_in);

	strncat(ptr, &cch_in[CHALLENGE_SIZE/2], CHALLENGE_SIZE/2);
	strncat(ptr+4, hch_in, CHALLENGE_SIZE/2);
	strncat(ptr+8, cch_in, CHALLENGE_SIZE/2);
	strncat(ptr+12, &hch_in[CHALLENGE_SIZE/2], CHALLENGE_SIZE/2);

	memcpy(key, enc_in, S_KEY_LEN / 2);
	print_hex("Static ENC Key 1", key, S_KEY_LEN / 2);
	DES_set_key((C_Block *) key, enc1);

	memcpy(key, &enc_in[S_KEY_LEN / 2], S_KEY_LEN / 2);
	print_hex("Static ENC Key 2", key, S_KEY_LEN / 2);
	DES_set_key((C_Block *) key, enc2);
}

void make_session_key(DES_key_schedule *k1, DES_key_schedule *k2,
		char *der, DES_key_schedule *skey1, DES_key_schedule *skey2)
{
	char key1[S_KEY_LEN / 2] = {0};
	char key2[S_KEY_LEN / 2] = {0};
	int i;

	DES_ecb2_encrypt((const_DES_cblock *) der, (C_Block *) key1, k1, k2, DES_ENCRYPT);
	DES_ecb2_encrypt((const_DES_cblock *) (der + 8), (C_Block *) key2, k1, k2, DES_ENCRYPT);

	for (i = 0; i < (S_KEY_LEN / 2); i++) {
		key1[i] = (((unsigned char)key1[i] % 2) == 1) ? 0x2d : 0xca;
		key2[i] = (((unsigned char)key2[i] % 2) == 1) ? 0x2d : 0xca;
	}

	print_hex("Sesion Key 1", key1, S_KEY_LEN / 2);
	DES_set_key((C_Block *) key1, skey1);

	print_hex("Sesion Key 2", key2, S_KEY_LEN / 2);
	DES_set_key((C_Block *) key2, skey2);
}

void get_cryptogram(char *out, DES_key_schedule *k1,
		DES_key_schedule *k2, int type)
{
	DES_cblock ivsetup = {0,0,0,0,0,0,0,0};
	DES_cblock ivec;	
	char base[BASE_SIZE] = {0};

	if (type == CARD_CRYPTGRAM) {
		strncat(base, hch_in, CHALLENGE_SIZE);
		strncat(&base[CHALLENGE_SIZE], cch_in, CHALLENGE_SIZE);
	} else if (type == HOST_CRYPTGRAM) {
		strncat(base, cch_in, CHALLENGE_SIZE);
		strncat(&base[CHALLENGE_SIZE], hch_in, CHALLENGE_SIZE);
	} else
		return;
	strncat(&base[2*CHALLENGE_SIZE] - 1, "\x80", 1);

	print_hex("Base", base, BASE_SIZE);

	memcpy(ivec, ivsetup, sizeof(ivsetup));
	DES_ede2_cbc_encrypt((unsigned char *) base, (unsigned char *) out, CRYPTG_LEN,
			k1, k2, &ivec, DES_ENCRYPT);
}

int main(int argc, char *argv[])
{
	FILE *fd;
	char ccrgram[CRYPTG_LEN];
	char hcrgram[CRYPTG_LEN];
	char derdat[DER_DATA_LEN] = {0};
	DES_key_schedule enc1, enc2;
	DES_key_schedule s_enc1, s_enc2;

	if (argc < 2) {
		printf("Usage: ./%s <filename>\n", argv[0]);
		printf("Put HC, CC, S ENC key, S MAC key to the file.\n");
		printf("Bytes have to be seperated by white space.\n");
		return 1;
	}

	fd = fopen(argv[1], "r");
	if (fd == NULL) {
		perror("fopen");
		return 1;
	}

	get_encks_der_file(fd, &enc1, &enc2, derdat);
	fclose(fd);

	/* make Session ENC key */
	make_session_key(&enc1, &enc2, derdat, &s_enc1, &s_enc2);

	/* get Card cryptogram */
	get_cryptogram(ccrgram, &s_enc1, &s_enc2, CARD_CRYPTGRAM);
	/* get Host cryptogram */
	get_cryptogram(hcrgram, &s_enc1, &s_enc2, HOST_CRYPTGRAM);

	print_hex("Card Cryptogram", ccrgram, CRYPTG_LEN);
	print_hex("Host Cryptogram", hcrgram, CRYPTG_LEN);

	return 0;
}
