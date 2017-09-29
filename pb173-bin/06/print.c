#include <stdio.h>

extern void *start_data;
extern void *end_data;
extern void *start_rodata;
extern void *end_rodata;

char *hel = "Hello World!";

void dump_hex(char *saddr, char *eaddr)
{
	unsigned char *ptr = saddr;
	unsigned char byte;
	unsigned int i, j, length = eaddr - saddr;

	for (i = 0; i < length; i++) {
		byte = ptr[i];
		printf("%02x ", ptr[i]);
		if (((i % 16) == 15) || (i == length - 1)) {
			for (j = 0; j < (15 - (i % 16)); j++)
				printf("   ");
			printf("| ");
			for (j = (i - (i % 16)); j <= i; j++) {
				byte = ptr[j];
				if ((byte > 31) && (byte < 127))
					printf("%c", byte);
				else
					printf(".");
			}
			printf("\n");
		}
	}
}

int main(void)
{
	printf("\n.rodata address: %p\n", &start_rodata);
	dump_hex((char *) &start_rodata, (char *) &end_rodata);

	printf("\n");

	printf(".data address: %p\n", &start_data);
	dump_hex((char *) &start_data, (char *) &end_data);

	return 0;
}
