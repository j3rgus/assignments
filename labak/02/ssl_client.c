/**
 *	Jergus Lysy (374217)
 *	Zadanie 3
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#define POP3_PORT 995

int main(int argc, char *argv[])
{
	char buf[256];
	int sockfd;
	int err;
	SSL *ssl;
	SSL_CTX *ctx;
	struct hostent *srv_info;
	struct sockaddr_in srv_addr;

	if (argc != 2) {
		fprintf(stderr, "Usage: ./ssl_client <address>\n");
		return 1;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	srv_info = gethostbyname(argv[1]);
	if (srv_info == NULL) {
		fprintf(stderr, "No such host!\n");
		return 1;
	}

	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(POP3_PORT);
	srv_addr.sin_addr = *(struct in_addr *)srv_info->h_addr;

	err = connect(sockfd, (struct sockaddr *) &srv_addr, sizeof(srv_addr));
	if (err == -1) {
		perror("connect");
		return 1;
	}

	SSL_load_error_strings();
	SSL_library_init();

	ctx = SSL_CTX_new(SSLv23_client_method());
	if (ctx == NULL) {
		fprintf(stderr, "Error while making ctx!\n");
		return 1;
	}

	ssl = SSL_new(ctx);
	if (ssl == NULL) {
		ERR_print_errors_fp(stderr);
		return 1;
	}

	err = SSL_set_fd(ssl, sockfd);
	if (!err) {
		ERR_print_errors_fp(stderr);
		return 1;
	}

	err = SSL_connect(ssl);
	if (err <= 0) {
		fprintf(stderr, "SSL_connect ends with code: %d\n", SSL_get_error(ssl, err));
		return 1;
	}

	err = SSL_read(ssl, buf, sizeof(buf) - 1);
	if (err <= 0) {
		fprintf(stderr, "SSL_read ends with code: %d\n", SSL_get_error(ssl, err));
		return 1;
	}

	printf("%s\n", buf);

	SSL_shutdown(ssl);
	close(sockfd);

	ERR_free_strings();

	return 0;
}
