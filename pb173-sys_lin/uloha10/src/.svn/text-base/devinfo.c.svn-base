#include <stdio.h>
#include <pcap.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void)
{
	char ebuf[PCAP_ERRBUF_SIZE];
	char str[201];
	pcap_if_t *device;
	pcap_addr_t *paddr;
	struct sockaddr_in *saddr;
	int ret;

	ret = pcap_findalldevs(&device, ebuf);
	if (ret < 0)
		fprintf(stderr, "%s\n", ebuf);

	while (device != NULL) {
		printf("%s, ", device->name);
		if (device->description != NULL)
			printf("%s, ", device->description);
		paddr = device->addresses;
		while (paddr != NULL) {
			saddr = (struct sockaddr_in *) paddr->addr;
			if (saddr->sin_family == AF_INET)
				inet_ntop(AF_INET, (struct in_addr *) &saddr->sin_addr, str, 200);
			else if (saddr->sin_family == AF_INET6)
				inet_ntop(AF_INET, (struct in6_addr *) &saddr->sin_addr, str, 200);
			printf("\t%s\n", str);
			paddr = paddr->next;
		}
		printf("\n");

		device = device->next;
	}

	return 0;
}
