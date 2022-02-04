#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "lora.h"

#if CONFIG_SENDER
void task_tx(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(NULL), "Start");
	uint8_t buf[256]; // Maximum Payload size of SX1276/77/78/79 is 255
	while(1) {
		TickType_t nowTick = xTaskGetTickCount();
		int send_len = sprintf((char *)buf,"Hello World!! %d",nowTick);
		lora_send_packet(buf, send_len);
		ESP_LOGI(pcTaskGetName(NULL), "%d byte packet sent...", send_len);
		vTaskDelay(pdMS_TO_TICKS(5000));
	} // end while
}
#endif // CONFIG_SENDER

#if CONFIG_RECEIVER
void task_rx(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(NULL), "Start");
	uint8_t buf[256]; // Maximum Payload size of SX1276/77/78/79 is 255
	while(1) {
		lora_receive(); // put into receive mode
		if (lora_received()) {
			int receive_len = lora_receive_packet(buf, sizeof(buf));
			ESP_LOGI(pcTaskGetName(NULL), "%d byte packet received:[%.*s]", receive_len, receive_len, buf);
		} // end if
		vTaskDelay(1); // Avoid WatchDog alerts
	} // end while
}
#endif // CONFIG_RECEIVER

void app_main()
{
	lora_init();

#if CONFIG_169MHZ
	ESP_LOGI(pcTaskGetName(NULL), "Frequency is 169MHz");
	lora_set_frequency(169e6); // 169MHz
#elif CONFIG_433MHZ
	ESP_LOGI(pcTaskGetName(NULL), "Frequency is 433MHz");
	lora_set_frequency(433e6); // 433MHz
#elif CONFIG_470MHZ
	ESP_LOGI(pcTaskGetName(NULL), "Frequency is 470MHz");
	lora_set_frequency(470e6); // 470MHz
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


	lora_set_bandwidth(CONFIG_BANDWIDTH);
	int bw = lora_get_bandwidth();
	ESP_LOGI(pcTaskGetName(NULL), "bandwidth=%d", bw);

#if CONFIG_SENDER
	xTaskCreate(&task_tx, "task_tx", 1024*2, NULL, 5, NULL);
#endif
#if CONFIG_RECEIVER
	xTaskCreate(&task_rx, "task_rx", 1024*2, NULL, 5, NULL);
#endif
}
