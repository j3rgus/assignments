/*
 *	Jergus Lysy (374217)
 *	Uloha 9
 *
 *	README: Nanestastie, nepodarilo na mi rozbehat funknost Winscard vo Windowse.
 *	Pouzil som LCC-Win32, kt. vsak obsahuje bug vo funkcii (problem s kniznicou)
 *	SCardTransmit a neumoznuje tak spusit binarku. Pevne vsak verim, ze kod robi
 *	aspon z velkej casti to co ma.
 */


#include <Winscard.h>
#include <stdio.h>
#include <string.h>

BYTE sendBuf[260];
BYTE recvBuf[258];

void print_recv_hex(DWORD len)
{
	int i;

	for (i = 0; i < len; i++)
		printf("%02x ", recvBuf[i]);
	printf("\n");
}

void setData(BYTE cla, BYTE ins, BYTE p1, BYTE p2, BYTE lc, LPCBYTE data)
{
	memset(sendBuf, 0, 260);
	sendBuf[0] = cla;
	sendBuf[1] = ins;
	sendBuf[2] = p1;
	sendBuf[3] = p2;
	sendBuf[4] = lc;

	if (data != NULL)
		memcpy(sendBuf+5, data, lc);
}

int main(void)
{
	FILE *fd;
	SCARDHANDLE hCardHandle;
	SCARDCONTEXT hSC;
	DWORD recvLen;
	DWORD dwAP, cch = SCARD_AUTOALLOCATE;
	LONG ret;
	char *readers = NULL;
	unsigned int len, filesz;
	unsigned int index, add;

	ret = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &hSC);
	if (ret != SCARD_S_SUCCESS) {
		printf("SCardEstablishContext failed");
		return 1;
	}

	ret = SCardListReaders(hSC, NULL, (char *) &readers, &cch);
	if (ret != SCARD_S_SUCCESS) {
		printf("SCardListReaders not OK");
		return 1;
	}

	/* pre jednoduchost pouzijem prvu citacku v zozname */
	ret = SCardConnect(hSC, (LPCTSTR) readers, SCARD_SHARE_EXCLUSIVE,
			SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &hCardHandle, &dwAP);
	if (ret != SCARD_S_SUCCESS) {
		printf("SCardConnect failed");
		return 1;
	}

	/* Select AID */
	recvLen = 256;
	setData(0x00, 0xa4, 0x04, 0x0c, 0x07, "\xa0\x00\x00\x02\x47\x10\x01");
	SCardTransmit(hCardHandle, SCARD_PCI_T0, sendBuf, sendBuf[4]+5, NULL, recvBuf, &recvLen);
	print_recv_hex(recvLen);

	/* Select File 01 0E */
	recvLen = 256;
	setData(0x00, 0xa4, 0x02, 0x0c, 0x02, "\x01\x0e");
	SCardTransmit(hCardHandle, SCARD_PCI_T0, sendBuf, sendBuf[4]+5, NULL, recvBuf, &recvLen);
	print_recv_hex(recvLen);

	recvLen = 256;
	printf("Getting file...\n");
	setData(0x00, 0xb0, 0x00, 0x00, 0x06, NULL);
	SCardTransmit(hCardHandle, SCARD_PCI_T0, sendBuf, 5, NULL, recvBuf, &recvLen);

	/* zistim, ci je vyuzity druhy bajt ako tag */
	index = 1;
	add = 2;	/* 1B tag, 1B length */

	if ((recvBuf[0] & 0x1f) == 0x1f) {
		index++;
		add++;	/* zvysim bajt pre dalsi tag */
	}

	/* zistim velkost suboru (predpokladam max. 2B na identifikaciu) */
	if (recvBuf[index] > 127) {
		filesz = recvBuf[index+1];
		if ((recvBuf[index] - 0x80) == 2) {
			filesz <<= 8;
			filesz |= recvBuf[index+2];
			add++;
		}
		add++; /* pricitam dalsi bajt */
	}
	else
		filesz = recvBuf[index];

	filesz += add;

	fd = fopen("karta", "wb");
	if (fd == NULL) {
		perror("karta");
		return 1;
	}

	/* Zapis do suboru, max. po 256B */
	index = 0;
	while (filesz > 0) {
		if (filesz > 256) {
			recvLen = 258;
			len = 0;
		}
		else {
			recvLen = filesz + 2;
			len = filesz;
		}

		setData(0x00, 0xb0, 0x00, index, len, NULL);
		SCardTransmit(hCardHandle, SCARD_PCI_T0, sendBuf, 5, NULL, recvBuf, &recvLen);
		fwrite(sendBuf, 1, recvLen - 2, fd); /* odcitam dva posledne bajty 90 00*/

		index += (recvLen - 2);
		filesz -= (recvLen - 2);
	}

	fclose(fd);

	ret = SCardDisconnect(hCardHandle, SCARD_LEAVE_CARD);
	if (ret != SCARD_S_SUCCESS) {
		printf("SCardDisconnect failed");
		return 1;
	}

	SCardFreeMemory(hSC, readers);

	ret = SCardReleaseContext(hSC);
	if (ret != SCARD_S_SUCCESS) {
		printf("SCardReleaseContext failed");
		return 1;
	}

	return 0;
}
