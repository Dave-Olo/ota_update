#include <string.h>
#include <stdio.h>
#include "tb_client.h"
#include "mqtt_client.h"
#include "ota_manager.h"
#include "esp_log.h"

#define TB_TOKEN "5itnsdtlveutd8yn4sdl"
#define TB_HOST "mqtt.thingsboard.cloud"

static const char *TAG = "TB";

static void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                               int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;

    switch ((esp_mqtt_event_id_t)event_id) {

    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "Connected to ThingsBoard");
        esp_mqtt_client_subscribe(event->client, "v1/devices/me/attributes", 0);
        break;

    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "Received: %.*s", event->data_len, event->data);

        if (strstr(event->data, "fw_url")) {
            char url[256];
            sscanf(event->data, "{\"fw_url\":\"%[^\"]\"}", url);
            ESP_LOGI(TAG, "OTA URL: %s", url);
            ota_start(url);
        }
        break;

    default:
        break;
    }
}

void tb_init(void)
{
    char uri[128];
    sprintf(uri, "mqtt://%s", TB_HOST);

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = uri,
        .credentials.username = TB_TOKEN,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}