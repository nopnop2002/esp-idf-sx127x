#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <openssl/evp.h>
#include <openssl/ssl.h>

#define SERVER_PORT "8080"
#define SERVER_CERT_FILE "server.crt"
#define SERVER_PRIVATE_KEY_FILE "server.key"

int main()
{
	SSL_library_init();
	SSL_CTX *ctx = SSL_CTX_new(SSLv23_server_method());

	SSL_CTX_use_certificate_file(ctx, SERVER_CERT_FILE, SSL_FILETYPE_PEM);
	SSL_CTX_use_PrivateKey_file(ctx, SERVER_PRIVATE_KEY_FILE, SSL_FILETYPE_PEM);

	struct addrinfo hints;
	struct addrinfo *ai = NULL;

	memset(&hints, sizeof(hints), 0);
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(NULL, SERVER_PORT, &hints, &ai);

	int server_fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	bind(server_fd, ai->ai_addr, ai->ai_addrlen);
	freeaddrinfo(ai);
	listen(server_fd, 10);

	while (1) {
		printf("Waiting for a connect from client\n");
		int session_fd = accept(server_fd, NULL, NULL);
		printf("accept\n");

		SSL *ssl = NULL;
		ssl = SSL_new(ctx);
		if (ssl == NULL) {
			printf("SSL_new fail\n");
			continue;
		}

		int ret;
		ret = SSL_set_fd(ssl, session_fd);
		if (ret == 0) {
			printf("SSL_set_fd fail\n");
			continue;
		}

		ret = SSL_accept(ssl);
		if (ret <= 0) {
			printf("SSL_accept fail\n");
			continue;
		}
		printf("Handshaked\n");

		unsigned char buf[1024];
		size_t nread;
		size_t nwritten;
		size_t total = 0;
		if (SSL_read_ex(ssl, buf, sizeof(buf), &nread) <= 0) {
			printf("SSL_read_ex operation was not successful\n");
			break;
		}
		printf("nread=%ld\n", nread);
		printf("buf=[%.*s]\n", (int)nread, buf);
		strcpy(buf, "Hello, secure world!\n");
		if (SSL_write_ex(ssl, buf, strlen(buf), &nwritten) <= 0) {
			printf("SSL_write_ex operation was not successful\n");
		}

		SSL_shutdown(ssl);
		SSL_free(ssl);

		close(session_fd);
	}

	close(server_fd);
	SSL_CTX_free(ctx);

	return 0;
}

