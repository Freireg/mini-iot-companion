#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "led_strip.h"
#include "mqtt_app.h"
#include "display_app.h"


#ifdef CONFIG_IDF_TARGET_ESP32C6
    #define BLINK_GPIO 8
    static uint8_t s_led_state = 0;

    static led_strip_handle_t led_strip;

static void blink_led(void)
{
    /* If the addressable LED is enabled */
    if (s_led_state) {
        /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
        led_strip_set_pixel(led_strip, 0, 16, 10, 0);
        /* Refresh the strip to send data */
        led_strip_refresh(led_strip);
    } else {
        /* Set all LED off to clear all pixels */
        led_strip_clear(led_strip);
    }
}

static void configure_led(void)
{
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = 1, // at least one LED on board
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    /* Set all LED off to clear all pixels */
    led_strip_clear(led_strip);
}

#endif

QueueHandle_t mqttQueue;
EventGroupHandle_t mqttEvent;

static const char *TAG = "MAIN_APP";

void systemTask(void *pvParameter)
{
    mqttMessage_t systemMessage;
    systemMessage.topic = "/topic/system";
    systemMessage.payload = "Hello from system task!";
    systemMessage.payload_len = strlen(systemMessage.payload);

    #ifdef CONFIG_IDF_TARGET_ESP32C6
        configure_led();
    #endif	


    ESP_LOGI(TAG, "System task started");

    while (1) {
        ESP_LOGI("SystemTask", "Sending system message...");
        // xQueueSend(mqttQueue, ( void * )&systemMessage, 0);

        #ifdef CONFIG_IDF_TARGET_ESP32C6
            blink_led();
            /* Toggle the LED state */
            s_led_state = !s_led_state;
        #endif

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_BASE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());


    mqttQueue = xQueueCreate(5, sizeof(mqttMessage_t));
    mqttEvent = xEventGroupCreate();

    mqtt_main();
    display_main();

    xTaskCreate(&systemTask, "systemTask", 2048, NULL, 5, NULL);

    
    
}