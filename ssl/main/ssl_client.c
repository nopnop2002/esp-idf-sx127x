/*
	SSL client Example using plain Mbed TLS sockets

	Contacts the https server via TLS v1.2 and reads a JSON response.
	Adapted from the ssl_client1 example in Mbed TLS.

	This example code is in the Public Domain (or CC0 licensed, at your option.)

	Unless required by applicable law or agreed to in writing, this
	software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
	CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/message_buffer.h"
#include "esp_log.h"

#include "mbedtls/platform.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/esp_debug.h"
#include "mbedtls/ssl.h"
#if ESP_IDF_VERSION_MAJOR == 5
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#endif
#include "mbedtls/error.h"
#ifdef CONFIG_MBEDTLS_SSL_PROTO_TLS1_3
#include "psa/crypto.h"
#endif
#include "esp_crt_bundle.h"

extern MessageBufferHandle_t xMessageBufferTrans;
extern MessageBufferHandle_t xMessageBufferRecv;
extern size_t xItemSize;

extern const uint8_t server_crt_start[] asm("_binary_server_crt_start");
extern const uint8_t server_crt_end[]	asm("_binary_server_crt_end");

/* Constants that aren't configurable in menuconfig */
#define SERVER_HOST CONFIG_SSL_SERVER_HOST
#define SERVER_PORT CONFIG_SSL_SERVER_PORT

static const char *TAG = "SSL_CLIENT";

int ssl_handshake(mbedtls_ssl_context * ssl) {
	int ret;
	while ((ret = mbedtls_ssl_handshake(ssl)) != 0) {
		ESP_LOGW(TAG, "mbedtls_ssl_handshake ret=%d", ret);
		if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
		{
			ESP_LOGE(TAG, "mbedtls_ssl_handshake returned -0x%x", -ret);
			return ret;
		}
	}
	return 0;
}

int ssl_write(mbedtls_ssl_context * ssl, const unsigned char * buf, size_t len) { 
	int ret;
	size_t written_bytes = 0;
	while(written_bytes < len) {
		ret = mbedtls_ssl_write(ssl, buf + written_bytes, len- written_bytes);
		if (ret >= 0) {
			ESP_LOGI(TAG, "%d bytes written", ret);
			written_bytes += ret;
		} else if (ret != MBEDTLS_ERR_SSL_WANT_WRITE && ret != MBEDTLS_ERR_SSL_WANT_READ) {
			ESP_LOGE(TAG, "mbedtls_ssl_write returned -0x%x", -ret);
			return ret;
		}
	}
	return 0;
}

int ssl_read(mbedtls_ssl_context * ssl, unsigned char * buf, size_t len, size_t *readed) {
	int pos = 0;
	*readed = 0;
	int ret;
	while(1) {
		ESP_LOGI(TAG, "mbedtls_ssl_read pos=%d", pos);
		ret = mbedtls_ssl_read(ssl, buf+pos, len-pos);

#if CONFIG_MBEDTLS_SSL_PROTO_TLS1_3 && CONFIG_MBEDTLS_CLIENT_SSL_SESSION_TICKETS
		if (ret == MBEDTLS_ERR_SSL_RECEIVED_NEW_SESSION_TICKET) {
			ESP_LOGD(TAG, "got session ticket in TLS 1.3 connection, retry read");
			continue;
		}
#endif // CONFIG_MBEDTLS_SSL_PROTO_TLS1_3 && CONFIG_MBEDTLS_CLIENT_SSL_SESSION_TICKETS

		if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE) {
			continue;
		}

		if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) {
			ESP_LOGI(TAG, "MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY");
			*readed = pos;
			ret = 0;
			break;
		}

		if (ret < 0) {
			ESP_LOGE(TAG, "mbedtls_ssl_read returned -0x%x", -ret);
			break;
		}

		if (ret == 0) {
			ESP_LOGI(TAG, "connection closed");
			break;
		}
		pos = pos + ret;
		if (pos >= len) {
			*readed = pos;
			ret = 0;
			break;
		}

	}
	return ret;
}

void ssl_terminate(mbedtls_ssl_context * ssl, mbedtls_net_context * server_fd, int ret) {
	char error_buf[100];
	mbedtls_ssl_session_reset(ssl);
	mbedtls_net_free(server_fd);
	if (ret != 0) {
		mbedtls_strerror(ret, error_buf, sizeof(error_buf));
		ESP_LOGE(TAG, "Last error was: -0x%x - %s", -ret, error_buf);
	}
}

void ssl_client(void *pvParameters)
{
	int ret;

	ESP_LOGI(TAG, "Start");
#if ESP_IDF_VERSION_MAJOR == 5
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
#endif
	mbedtls_ssl_context ssl;
	mbedtls_x509_crt cacert;
	mbedtls_ssl_config conf;
	mbedtls_net_context server_fd;

#ifdef CONFIG_MBEDTLS_SSL_PROTO_TLS1_3
	psa_status_t status = psa_crypto_init();
	if (status != PSA_SUCCESS) {
		ESP_LOGE(TAG, "Failed to initialize PSA crypto, returned %d", (int) status);
		return;
	}
#endif

	mbedtls_ssl_init(&ssl);
	mbedtls_x509_crt_init(&cacert);
#if ESP_IDF_VERSION_MAJOR == 5
	mbedtls_ctr_drbg_init(&ctr_drbg);
	ESP_LOGI(TAG, "Seeding the random number generator");
#endif

	mbedtls_ssl_config_init(&conf);

#if ESP_IDF_VERSION_MAJOR == 5
	mbedtls_entropy_init(&entropy);
	if((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0)) != 0)
	{
		ESP_LOGE(TAG, "mbedtls_ctr_drbg_seed returned %d", ret);
		abort();
	}
#endif

	ESP_LOGI(TAG, "Loading the CA root certificate...");

	ret = mbedtls_x509_crt_parse(&cacert, server_crt_start, server_crt_end - server_crt_start);
	if(ret < 0)
	{
		ESP_LOGE(TAG, "mbedtls_x509_crt_parse returned -0x%x", -ret);
		abort();
	}


	ESP_LOGI(TAG, "Setting hostname for TLS session...");

	 /* Hostname set here should match CN in server certificate */
	if((ret = mbedtls_ssl_set_hostname(&ssl, SERVER_HOST)) != 0)
	{
		ESP_LOGE(TAG, "mbedtls_ssl_set_hostname returned -0x%x", -ret);
		abort();
	}

	ESP_LOGI(TAG, "Setting up the SSL/TLS structure...");

	if((ret = mbedtls_ssl_config_defaults(&conf,
		MBEDTLS_SSL_IS_CLIENT,
		MBEDTLS_SSL_TRANSPORT_STREAM,
		MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
	{
		ESP_LOGE(TAG, "mbedtls_ssl_config_defaults returned %d", ret);
		ssl_terminate(&ssl, &server_fd, ret);
		vTaskDelete(NULL);
	}

	mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_REQUIRED);
	mbedtls_ssl_conf_ca_chain(&conf, &cacert, NULL);
#if ESP_IDF_VERSION_MAJOR == 5
	mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
#endif
#ifdef CONFIG_MBEDTLS_DEBUG
	mbedtls_esp_enable_debug_log(&conf, CONFIG_MBEDTLS_DEBUG_LEVEL);
#endif

	if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0)
	{
		ESP_LOGE(TAG, "mbedtls_ssl_setup returned -0x%x", -ret);
		ssl_terminate(&ssl, &server_fd, ret);
		vTaskDelete(NULL);
	}

	char buffer[xItemSize];
	char work[512];
	while(1) {
		size_t received = xMessageBufferReceive(xMessageBufferTrans, buffer, sizeof(buffer), portMAX_DELAY);
		ESP_LOGI(TAG, "xMessageBufferReceive received=%d", received);
		if (received <= 0) {
			ESP_LOGE(TAG, "xMessageBufferReceive fail");
			ssl_terminate(&ssl, &server_fd, 0);
			break;
		}

		mbedtls_net_init(&server_fd);

		char server_port[16];
		snprintf(server_port, sizeof(server_port), "%d", SERVER_PORT);

		ESP_LOGI(TAG, "Connecting to %s:%s...", SERVER_HOST, server_port);

		if ((ret = mbedtls_net_connect(&server_fd, SERVER_HOST, server_port, MBEDTLS_NET_PROTO_TCP)) != 0)
		{
			ESP_LOGE(TAG, "mbedtls_net_connect returned -%x", -ret);
			ssl_terminate(&ssl, &server_fd, ret);
			continue;
		}

		ESP_LOGI(TAG, "Connected.");

		mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);

		ESP_LOGI(TAG, "Performing the SSL/TLS handshake...");
		ret = ssl_handshake(&ssl);
		if (ret != 0) {
			ESP_LOGE(TAG, "ssl_handshake returned -%x", -ret);
			ssl_terminate(&ssl, &server_fd, ret);
			break;
		}

		ESP_LOGI(TAG, "Verifying peer X.509 certificate...");

		int flags;
		if ((flags = mbedtls_ssl_get_verify_result(&ssl)) != 0)
		{
			/* In real life, we probably want to close connection if ret != 0 */
			ESP_LOGW(TAG, "Failed to verify peer certificate!");
			bzero(work, sizeof(work));
			mbedtls_x509_crt_verify_info(work, sizeof(work), "	! ", flags);
			ESP_LOGW(TAG, "verification info: %s", work);
		}
		else {
			ESP_LOGI(TAG, "Certificate verified.");
		}

		ESP_LOGI(TAG, "Cipher suite is %s", mbedtls_ssl_get_ciphersuite(&ssl));

		ESP_LOGI(TAG, "Writing...");
		ret = ssl_write(&ssl, (const unsigned char *)buffer, received);
		if (ret != 0) {
			ESP_LOGE(TAG, "ssl_write returned -%x", -ret);
			ssl_terminate(&ssl, &server_fd, ret);
			break;
		}

		ESP_LOGI(TAG, "Reading...");
		size_t readed;
		bzero(buffer, xItemSize);
		ret = ssl_read(&ssl, (unsigned char *)buffer, xItemSize, &readed);
		if (ret != 0) {
			ESP_LOGE(TAG, "ssl_read returned -%x", -ret);
			ssl_terminate(&ssl, &server_fd, ret);
			break;
		}
		ESP_LOGI(TAG, "%d bytes read", readed);
		ESP_LOG_BUFFER_HEXDUMP(TAG, buffer, readed, ESP_LOG_DEBUG);

#if 0
		size_t spacesAvailable = xMessageBufferSpacesAvailable( xMessageBufferRecv );
		ESP_LOGI(TAG, "spacesAvailable=%d", spacesAvailable);
		size_t sended = xMessageBufferSend(xMessageBufferRecv, buffer, readed, 100);
		if (sended != readed) {
			ESP_LOGE(TAG, "xMessageBufferSend fail readed=%d sended=%d", readed, sended);
			ssl_terminate(&ssl, &server_fd, 0);
			break;
		}
#endif

		mbedtls_ssl_close_notify(&ssl);

		mbedtls_ssl_session_reset(&ssl);
		mbedtls_net_free(&server_fd);
	} // end while

	vTaskDelete(NULL);
}
