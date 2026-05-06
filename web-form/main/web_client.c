/*
	Example using WEB Socket.
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
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "freertos/message_buffer.h"
#include "esp_log.h"
#include "cJSON.h"

static const char *TAG = "CLIENT";

#include "websocket_server.h"

extern MessageBufferHandle_t xMessageBufferTrans;
extern MessageBufferHandle_t xMessageBufferRecv;
extern size_t xItemSize;

void client_task(void* pvParameters) {
	ESP_LOGI(TAG, "Start");
	char messageBuffer[xItemSize];
	while(1) {
		size_t readBytes = xMessageBufferReceive(xMessageBufferTrans, messageBuffer, sizeof(messageBuffer), portMAX_DELAY );
		ESP_LOGD(TAG, "readBytes=%d", readBytes);
		cJSON *root = cJSON_Parse(messageBuffer);
		if (cJSON_GetObjectItem(root, "id")) {
			char *id = cJSON_GetObjectItem(root,"id")->valuestring;
			ESP_LOGI(TAG, "id=%s",id);

			// Do something when the browser started
			if ( strcmp (id, "init") == 0) {
			} // end of init

			if ( strcmp (id, "send-request") == 0) {
				char *payload = cJSON_GetObjectItem(root,"payload")->valuestring;
				xMessageBufferSend(xMessageBufferRecv, payload, strlen(payload), portMAX_DELAY);
			} // end of send-request

			if ( strcmp (id, "recv-request") == 0) {
				char *payload = cJSON_GetObjectItem(root,"payload")->valuestring;
				char out[256];
				char DEL = 0x04;
				int outlen = sprintf(out,"RECEIVE%c%s", DEL, payload);
				ESP_LOGD(TAG,"outlen=%d out-[%s]", outlen, out);
				ws_server_send_text_all(out, outlen);
			} // end of recv-request

		}
		cJSON_Delete(root);
	}
	vTaskDelete(NULL);
}
