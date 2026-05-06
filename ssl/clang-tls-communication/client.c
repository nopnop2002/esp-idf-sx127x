#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <openssl/ssl.h>

#define SERVER_PORT "8000"
#define SERVER_HOST "localhost"

int main()
{
	SSL_library_init();
	SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());

	struct addrinfo hints;
	struct addrinfo *res, *ai;

	memset(&hints, sizeof(hints), 0);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo(SERVER_HOST, SERVER_PORT, &hints, &res);

	int sockfd;
	for(ai=res; ai; ai=ai->ai_next) {
		sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
		if(!connect(sockfd, ai->ai_addr, ai->ai_addrlen))
			break;
		close(sockfd);
	}
	freeaddrinfo(res);

	SSL *ssl = SSL_new(ctx);
	SSL_set_options(ssl, SSL_OP_NO_TICKET);
	SSL_set_fd(ssl, sockfd);

	SSL_connect(ssl);

	while (1) {
		char buf[512];
		sprintf(buf, "%s", "hello world");
		size_t nwritten;
		if (SSL_write_ex(ssl, buf, strlen(buf), &nwritten) <= 0) {
			printf("SSL_write_ex operation was not successful\n");
			break;
		}
		printf("nwritten=%ld\n", nwritten);

		size_t nread;
		if (SSL_read_ex(ssl, buf, sizeof(buf), &nread) <= 0) {
			printf("SSL_read_ex operation was not successful\n");
			break;
		}
		printf("nread=%ld\n", nread);
		printf("buf=[%.*s]\n", (int)nread, buf);
		break;
	}

	SSL_shutdown(ssl);
	SSL_free(ssl);

	close(sockfd);
	SSL_CTX_free(ctx);

	return 0;
}
