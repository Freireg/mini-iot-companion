#include "mqtt_app.h"

static const char *TAG = "MQTT_APP";

extern QueueHandle_t mqttQueue;
extern EventGroupHandle_t mqttEvent;

uint32_t MQTT_CONNECTED = 0;
static esp_mqtt_client_handle_t client = NULL;


static void startMQTTApp(void);

/**
 * @brief Event handler for MQTT events.
 * 
 * This function is called whenever an MQTT event occurs.
 * It is responsible for handling the event and performing
 * the necessary actions.
 * 
 * @param handler_args 
 * @param base 
 * @param event_id 
 * @param event_data 
 */
static void mqttEventHandler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    BaseType_t xHigherPriorityTaskWoken, xResult;

    xHigherPriorityTaskWoken = pdFALSE;

    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32, base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        // ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        xEventGroupSetBitsFromISR(mqttEvent, (1 << 0), &xHigherPriorityTaskWoken);

        msg_id = esp_mqtt_client_subscribe(client, "sensor/temperature", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        // ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        // ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        // msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        // ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        // ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        // ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        // ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            ESP_LOGI(TAG, "Last error code reported from esp-tls: 0x%x", event->error_handle->esp_tls_last_esp_err);
            ESP_LOGI(TAG, "Last tls stack error number: 0x%x", event->error_handle->esp_tls_stack_err);
            ESP_LOGI(TAG, "Last captured errno : %d (%s)",  event->error_handle->esp_transport_sock_errno,
                     strerror(event->error_handle->esp_transport_sock_errno));
        } else if (event->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED) {
            ESP_LOGI(TAG, "Connection refused error: 0x%x", event->error_handle->connect_return_code);
        } else {
            ESP_LOGW(TAG, "Unknown error type: 0x%x", event->error_handle->error_type);
        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

/**
 * @brief Starts the MQTT application.
 * 
 * This function is responsible for starting the MQTT application.
 * It performs the necessary initialization and sets up the required
 * configurations for the MQTT communication.
 * 
 * @note This function should be called once at the beginning of the program.
 */
static void startMQTTApp(void)
{/*Depending on your website or cloud there could be more parameters in mqtt_cfg.*/
  const esp_mqtt_client_config_t mqtt_cfg= {
        .broker = {
            .address.uri = CONFIG_BROKER_URI,
        },
        .credentials = {
          .username = "homeassistant",
          .authentication.password = "pac3ohkah1vi7rohPaeth0raeNg8oob8thae8chi1ooNeesh1bah2Kea9eikoafi",
        }
    };
//   ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqttEventHandler, NULL);
    esp_mqtt_client_start(client);

}

static void mqttTask(void *pvParameters)
{
    ESP_LOGI(TAG, "mqttTask started");
    int msgId;
    mqttMessage_t receivedMessage;
    EventBits_t connectionEvents;

;

    while (1) 
    {
        connectionEvents = xEventGroupWaitBits(mqttEvent, (1 << 1), pdTRUE, pdFALSE, 10);
        if(connectionEvents & (1 << 1))
        {
            ESP_LOGI(TAG, "Requesting MQTT connection");
            /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
            * Read "Establishing Wi-Fi or Ethernet Connection" section in
            * examples/protocols/README.md for more information about this function.
            */
            ESP_ERROR_CHECK_WITHOUT_ABORT(example_connect());
            // startMQTTApp()   
        }
        if((xQueueReceive(mqttQueue, &receivedMessage, 10) == pdTRUE))
        {
            ESP_LOGI(TAG, "Sending message to MQTT topic=%s, payload=%s", receivedMessage.topic, receivedMessage.payload);
            // mqtt_publish(client, receivedMessage.topic, receivedMessage.payload, sizeof(receivedMessage.payload), 0, 0);
            msgId = esp_mqtt_client_publish(client, receivedMessage.topic, receivedMessage.payload, receivedMessage.payload_len, 0, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msgId);
            vTaskDelay(10/portTICK_PERIOD_MS);
        }
    }
}

void mqtt_main(void)
{
    xTaskCreate(&mqttTask, "mqttTask", 8192, NULL, 5, NULL);
    
}