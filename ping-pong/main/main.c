/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "lora.h"

#if CONFIG_PRIMARY

#define TIMEOUT 100

void task_primary(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(NULL), "Start");
	uint8_t buf[256]; // Maximum Payload size of SX1276/77/78/79 is 255
	while(1) {
		TickType_t nowTick = xTaskGetTickCount();
		int send_len = sprintf((char *)buf,"Hello World!! %"PRIu32, nowTick);

#if 0
		// Maximum Payload size of SX1276/77/78/79 is 255
		memset(&buf[send_len], 0x20, 255-send_len);
		send_len = 255;
#endif

		lora_send_packet(buf, send_len);
		ESP_LOGI(pcTaskGetName(NULL), "%d byte packet sent:[%.*s]", send_len, send_len, buf);

		bool waiting = true;
		TickType_t startTick = xTaskGetTickCount();
		while(waiting) {
			lora_receive(); // put into receive mode
			if(lora_received()) {
				int rxLen = lora_receive_packet(buf, sizeof(buf));
				TickType_t currentTick = xTaskGetTickCount();
				TickType_t diffTick = currentTick - startTick;
				ESP_LOGI(pcTaskGetName(NULL), "%d byte packet received:[%.*s]", rxLen, rxLen, buf);
				ESP_LOGI(pcTaskGetName(NULL), "Response time is %"PRIu32" millisecond", diffTick * portTICK_PERIOD_MS);
				waiting = false;
			}
			TickType_t currentTick = xTaskGetTickCount();
			TickType_t diffTick = currentTick - startTick;
			ESP_LOGD(pcTaskGetName(NULL), "diffTick=%"PRIu32, diffTick);
			if (diffTick > TIMEOUT) {
				ESP_LOGW(pcTaskGetName(NULL), "Response timeout");
				waiting = false;
			}
			vTaskDelay(1); // Avoid WatchDog alerts
		} // end waiting
		vTaskDelay(pdMS_TO_TICKS(5000));
	} // end while
}
#endif // CONFIG_PRIMARY

#if CONFIG_SECONDARY
void task_secondary(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(NULL), "Start");
	uint8_t buf[256]; // Maximum Payload size of SX1276/77/78/79 is 255
	while(1) {
		lora_receive(); // put into receive mode
		if(lora_received()) {
			int rxLen = lora_receive_packet(buf, sizeof(buf));
			ESP_LOGI(pcTaskGetName(NULL), "%d byte packet received:[%.*s]", rxLen, rxLen, buf);
			for (int i=0;i<rxLen;i++) {
				if (isupper(buf[i])) {
					buf[i] = tolower(buf[i]);
				} else {
					buf[i] = toupper(buf[i]);
				}
			}
			vTaskDelay(1);
			lora_send_packet(buf, rxLen);
			ESP_LOGI(pcTaskGetName(NULL), "%d byte packet sent back...", rxLen);
		}
		vTaskDelay(1); // Avoid WatchDog alerts
	}
}
#endif // CONFIG_SECONDARY

void app_main()
{
	if (lora_init() == 0) {
		ESP_LOGE(pcTaskGetName(NULL), "Does not recognize the module");
		while(1) {
			vTaskDelay(1);
		}
	}

#if CONFIG_433MHZ
	ESP_LOGI(pcTaskGetName(NULL), "Frequency is 433MHz");
	lora_set_frequency(433e6); // 433MHz
#elif CONFIG_866MHZ
	ESP_LOGI(pcTaskGetName(NULL), "Frequency is 866MHz");
	lora_set_frequency(866e6); // 866MHz
#elif CONFIG_915MHZ
	ESP_LOGI(pcTaskGetName(NULL), "Frequency is 915MHz");
	lora_set_frequency(915e6); // 915MHz
#elif CONFIG_OTHER
	ESP_LOGI(pcTaskGetName(NULL), "Frequency is %dMHz", CONFIG_OTHER_FREQUENCY);
	long frequency = CONFIG_OTHER_FREQUENCY * 1000000;
	lora_set_frequency(frequency);
#endif

	lora_enable_crc();

	int cr = 1;
	int bw = 7;
	int sf = 7;
#if CONFIG_ADVANCED
	cr = CONFIG_CODING_RATE;
	bw = CONFIG_BANDWIDTH;
	sf = CONFIG_SF_RATE;
#endif

	lora_set_coding_rate(cr);
	//lora_set_coding_rate(CONFIG_CODING_RATE);
	//cr = lora_get_coding_rate();
	ESP_LOGI(pcTaskGetName(NULL), "coding_rate=%d", cr);

	lora_set_bandwidth(bw);
	//lora_set_bandwidth(CONFIG_BANDWIDTH);
	//int bw = lora_get_bandwidth();
	ESP_LOGI(pcTaskGetName(NULL), "bandwidth=%d", bw);

	lora_set_spreading_factor(sf);
	//lora_set_spreading_factor(CONFIG_SF_RATE);
	//int sf = lora_get_spreading_factor();
	ESP_LOGI(pcTaskGetName(NULL), "spreading_factor=%d", sf);

#if CONFIG_PRIMARY
	xTaskCreate(&task_primary, "PRIMARY", 1024*3, NULL, 5, NULL);
#endif
#if CONFIG_SECONDARY
	xTaskCreate(&task_secondary, "SECONDARY", 1024*3, NULL, 5, NULL);
#endif
}
