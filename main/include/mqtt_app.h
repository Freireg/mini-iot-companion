#ifndef E8E5A977_3839_42DC_9C10_1B107876F570
#define E8E5A977_3839_42DC_9C10_1B107876F570

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "freertos/portmacro.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

#define WIFI_SSID "Tasp_Giardino_Ap1503_2G"
#define WIFI_PASS "ann240ur"
#define CONFIG_BROKER_URI "mqtt://10.0.0.102:1883"

void mqtt_main(void);

typedef struct {
    char *topic;
    char *payload;
    uint8_t payload_len;
} mqttMessage_t;

#endif /* E8E5A977_3839_42DC_9C10_1B107876F570 */
