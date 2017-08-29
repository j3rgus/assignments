#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void printip(struct addrinfo *res)
{
	char buf[256];
	void *ptr;

	while (res != NULL) {
		switch (res->ai_family) {
		case AF_INET:
			ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
			break;
		case AF_INET6:
			ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
			break;
		}

		inet_ntop(res->ai_family, ptr, buf, 255);
		printf("\tIPv%d address: %s\n", (res->ai_family == AF_INET6) ? 6 : 4, buf);
		res = res->ai_next;
	}
}

void printhost(struct sockaddr *addr)
{
	char hname[256];
	int ret;

	ret = getnameinfo(addr, sizeof(struct sockaddr_storage), hname, 255, NULL, 0, NI_NAMEREQD);

	if (ret != 0) {
		printf("getnameinfo: %s\n", gai_strerror(ret));
		return;
	}
	printf("\t%s\n", hname);
}

int main(int argc, char **argv)
{
	struct addrinfo hints;
	struct addrinfo *res;
	int ret;

	if (argc < 2) {
		printf("More arguments needed!\n");
		return 1;
	}

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags |= AI_CANONNAME;
	hints.ai_protocol = 0;

	ret = getaddrinfo(argv[1], NULL, &hints, &res);
	if (ret != 0) {
		printf("getaddrinfo: %s\n", gai_strerror(ret));
		return 1;
	}

	printf("Host:\n");
	printhost(res->ai_addr);

	printf("\nIPs:\n");
	printip(res);

	freeaddrinfo(res);
	return 0;
}
