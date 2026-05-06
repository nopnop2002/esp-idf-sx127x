/*
	HTTPS POST Example using ESP-TLS component

	Contacts the https server via TLS v1.2 and reads a JSON response.
	Adapted from the ssl_client1 example in Mbed TLS.

	This example code is in the Public Domain (or CC0 licensed, at your option.)

	Unless required by applicable law or agreed to in writing, this
	software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
	CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/message_buffer.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_tls.h"

static const char *TAG = "HTTPS_CLIENT";

extern MessageBufferHandle_t xMessageBufferTrans;
extern size_t xItemSize;

extern const uint8_t server_crt_start[] asm("_binary_server_crt_start");
extern const uint8_t server_crt_end[]	asm("_binary_server_crt_end");

static esp_err_t https_request(esp_tls_cfg_t cfg, char *url, char *request)
{
	esp_tls_t *tls = esp_tls_init();
	if (!tls) {
		ESP_LOGE(TAG, "Failed to allocate esp_tls handle!");
		return ESP_FAIL;
	}

	if (esp_tls_conn_http_new_sync(url, &cfg, tls) == 1) {
		ESP_LOGI(TAG, "Connection established...");
	} else {
		ESP_LOGE(TAG, "Connection failed...");
		int esp_tls_code = 0, esp_tls_flags = 0;
		esp_tls_error_handle_t tls_e = NULL;
		esp_tls_get_error_handle(tls, &tls_e);
		/* Try to get TLS stack level error and certificate failure flags, if any */
		esp_err_t ret = esp_tls_get_and_clear_last_error(tls_e, &esp_tls_code, &esp_tls_flags);
		if (ret == ESP_OK) {
			ESP_LOGE(TAG, "TLS error = -0x%x, TLS flags = -0x%x", esp_tls_code, esp_tls_flags);
		}
		esp_tls_conn_destroy(tls);
		return ESP_FAIL;
	}

#ifdef CONFIG_EXAMPLE_CLIENT_SESSION_TICKETS
	/* The TLS session is successfully established, now saving the session ctx for reuse */
	if (save_client_session) {
		esp_tls_free_client_session(tls_client_session);
		tls_client_session = esp_tls_get_client_session(tls);
	}
#endif

	ESP_LOGI(TAG, "Writing HTTP request...");
	size_t written_bytes = 0;
	ssize_t ssize;
	do {
		ssize = esp_tls_conn_write(tls, request + written_bytes, strlen(request) - written_bytes);
		if (ssize >= 0) {
			ESP_LOGI(TAG, "%d bytes written", ssize);
			written_bytes += ssize;
		} else if (ssize != ESP_TLS_ERR_SSL_WANT_READ  && ssize != ESP_TLS_ERR_SSL_WANT_WRITE) {
			ESP_LOGE(TAG, "esp_tls_conn_write  returned: [0x%02X](%s)", ssize, esp_err_to_name(ssize));
			esp_tls_conn_destroy(tls);
			return ESP_FAIL;
		}
	} while (written_bytes < strlen(request));

	ESP_LOGI(TAG, "Reading HTTP response...");
	char buf[512];
	do {
		int len = sizeof(buf) - 1;
		memset(buf, 0x00, sizeof(buf));
		ssize = esp_tls_conn_read(tls, (char *)buf, len);

		if (ssize == ESP_TLS_ERR_SSL_WANT_WRITE  || ssize == ESP_TLS_ERR_SSL_WANT_READ) {
			continue;
		} else if (ssize < 0) {
			ESP_LOGE(TAG, "esp_tls_conn_read  returned [-0x%02X](%s)", -ssize, esp_err_to_name(ssize));
			break;
		} else if (ssize == 0) {
			ESP_LOGI(TAG, "connection closed");
			break;
		}

		ESP_LOGD(TAG, "%d bytes read", ssize);
		/* Print response directly to stdout as it is read */
		for (int i = 0; i < ssize; i++) {
			putchar(buf[i]);
		}
		putchar('\n'); // JSON output doesn't have a newline at end
	} while (1);

	esp_tls_conn_destroy(tls);
	return ESP_OK;
}

static void makePostRequest(char * request, char * host, int port, char * payload, int length)
{
	char wk[64];
	strcpy(request, "POST / HTTP/1.1\n");
	//strcat(request, "User-Agent: ESP32 HTTPS Client/1.0\n");
	sprintf(wk, "User-Agent: ESP-IDF/%d.%d\n", ESP_IDF_VERSION_MAJOR, ESP_IDF_VERSION_MINOR);
	strcat(request, wk);
	//strcat(request, "Host: 192.168.0.40:8080\n");
	sprintf(wk, "Host: %s:%d\n", host, port);
	strcat(request, wk);
	strcat(request, "Content-Type: application/json\n");
	//strcat(request, "Content-Length: 18\n\n");
	sprintf(wk, "Content-Length: %d\n\n", length);
	strcat(request, wk);
	strcat(request, payload);
}

esp_err_t query_mdns_host(const char * host_name, char *ip);
void convert_mdns_host(char * from, char * to);

void https_client(void *pvParameters)
{
	ESP_LOGI(TAG, "Start HTTPS_SERVER_HOST:%s HTTPS_SERVER_PORT:%d", CONFIG_HTTPS_SERVER_HOST, CONFIG_HTTPS_SERVER_PORT);

	// Resolve mDNS host name
	char ip[128];
	ESP_LOGI(TAG, "CONFIG_HTTPS_SERVER_HOST=[%s]", CONFIG_HTTPS_SERVER_HOST);
	convert_mdns_host(CONFIG_HTTPS_SERVER_HOST, ip);
	ESP_LOGI(TAG, "ip=[%s]", ip);
	char url[142];
	sprintf(url, "https://%s:%d", ip, CONFIG_HTTPS_SERVER_PORT);
	ESP_LOGI(TAG, "url=[%s]", url);

	char buffer[xItemSize];
	while (1) {
		size_t received = xMessageBufferReceive(xMessageBufferTrans, buffer, sizeof(buffer), portMAX_DELAY);
		ESP_LOGI(TAG, "xMessageBufferReceive received=%d", received);
		if (received > 0) {
			ESP_LOGI(TAG, "xMessageBufferReceive buffer=[%.*s]",received, buffer);

			ESP_LOGI(TAG, "https_request using server.crt");
			esp_tls_cfg_t cfg = {
				.cacert_buf = (const unsigned char *) server_crt_start,
				.cacert_bytes = server_crt_end - server_crt_start,
			};

			char request[512];
			makePostRequest(request, ip, CONFIG_HTTPS_SERVER_PORT, buffer, received);
	
			//https_request(cfg, "https://192.168.0.40:8000", request);
			https_request(cfg, url, request);

		} else {
			ESP_LOGE(TAG, "xMessageBufferReceive fail");
			break;
		}
	} // end while

	// Stop connection
	vTaskDelete(NULL);
}
