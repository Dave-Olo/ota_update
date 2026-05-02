// #include "tb_client.h"
// #include "ota_manager.h"
// #include "mqtt_client.h"
// #include "cJSON.h"
// #include "esp_log.h"
// #include <string.h>

// #define TB_TOKEN "5itnsdtlveutd8yn4sdl"
// #define TB_HOST  "mqtt.thingsboard.cloud"

// static const char *TAG = "TB_CLIENT";

// esp_mqtt_client_handle_t client = NULL;

// static void report_current_firmware(void)
// {
//     const char *msg = "{\"current_fw_title\":\"firmware2\",\"current_fw_version\":\"1.0.0\",\"fw_state\":\"UPDATED\"}";
//     esp_mqtt_client_publish(client, "v1/devices/me/attributes", msg, 0, 1, 0);
//     ESP_LOGI(TAG, "Reported current firmware v1.0.0");
// }

// static void mqtt_event_handler(void *handler_args, esp_event_base_t base,
//                                int32_t event_id, void *event_data)
// {
//     esp_mqtt_event_handle_t event = event_data;

//     switch (event_id) {
//         case MQTT_EVENT_CONNECTED:
//             ESP_LOGI(TAG, "Connected to ThingsBoard MQTT");
//             esp_mqtt_client_subscribe(client, "v1/devices/me/attributes", 1);
//             report_current_firmware();
//             break;

//         case MQTT_EVENT_DATA:
//             ESP_LOGI(TAG, "=== RAW DATA RECEIVED ===");
//             ESP_LOGI(TAG, "Topic: %.*s", event->topic_len, event->topic);
//             ESP_LOGI(TAG, "Data : %.*s", event->data_len, event->data);

//             if (strstr(event->topic, "attributes") != NULL) {
//                 cJSON *root = cJSON_ParseWithLength(event->data, event->data_len);
//                 if (root) {
//                     cJSON *item = NULL;
//                     cJSON_ArrayForEach(item, root) {
//                         if (cJSON_IsString(item)) {
//                             ESP_LOGI(TAG, "Key: %s = %s", item->string, item->valuestring);
                            
//                             if (strcmp(item->string, "fw_url") == 0) {
//                                 ESP_LOGI(TAG, ">>> OTA URL FOUND: %s", item->valuestring);
//                                 esp_mqtt_client_publish(client, "v1/devices/me/telemetry", 
//                                                       "{\"fw_state\":\"DOWNLOADING\"}", 0, 1, 0);
//                                 ota_start(item->valuestring);
//                             }
//                         }
//                     }
//                     cJSON_Delete(root);
//                 } else {
//                     ESP_LOGW(TAG, "Failed to parse JSON");
//                 }
//             }
//             break;

//         default:
//             break;
//     }
// }

// void tb_init(void)
// {
//     char uri[128];
//     sprintf(uri, "mqtt://%s", TB_HOST);

//     esp_mqtt_client_config_t mqtt_cfg = {
//         .broker.address.uri = uri,
//         .credentials.username = TB_TOKEN,
//     };

//     client = esp_mqtt_client_init(&mqtt_cfg);
//     esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
//     esp_mqtt_client_start(client);

//     ESP_LOGI(TAG, "ThingsBoard client started");
// }

// #include "tb_client.h"
// #include "ota_manager.h"
// #include "mqtt_client.h"
// #include "cJSON.h"
// #include "esp_log.h"
// #include <string.h>

// #define TB_TOKEN "5itnsdtlveutd8yn4sdl"
// #define TB_HOST  "mqtt.thingsboard.cloud"

// static const char *TAG = "TB_CLIENT";
//  esp_mqtt_client_handle_t client = NULL;

// static void mqtt_event_handler(void *handler_args, esp_event_base_t base,
//                                int32_t event_id, void *event_data)
// {
//     esp_mqtt_event_handle_t event = event_data;

//     switch (event_id) {
//         case MQTT_EVENT_CONNECTED:
//             ESP_LOGI(TAG, "Connected to ThingsBoard");
//             esp_mqtt_client_subscribe(client, "v1/devices/me/rpc/request/+", 1);   // RPC
//             esp_mqtt_client_subscribe(client, "v1/devices/me/attributes", 1);
//             ESP_LOGI(TAG, "Subscribed to RPC and Attributes");
//             break;

//         case MQTT_EVENT_DATA:
//             ESP_LOGI(TAG, "Received - Topic: %.*s | Data: %.*s", 
//                      event->topic_len, event->topic, event->data_len, event->data);

//             // Handle RPC command
//             if (strstr(event->topic, "rpc/request") != NULL) {
//                 cJSON *root = cJSON_ParseWithLength(event->data, event->data_len);
//                 if (root) {
//                     cJSON *method = cJSON_GetObjectItem(root, "method");
//                     if (cJSON_IsString(method) && strcmp(method->valuestring, "startOTA") == 0) {
//                         ESP_LOGI(TAG, ">>> RPC OTA Command Received!");
//                         ota_start("https://raw.githubusercontent.com/Dave-Olo/ota_update/main/firmware2.bin");
//                     }
//                     cJSON_Delete(root);
//                 }
//             }
//             break;

//         default:
//             break;
//     }
// }

// void tb_init(void)
// {
//     char uri[128];
//     sprintf(uri, "mqtt://%s", TB_HOST);

//     esp_mqtt_client_config_t mqtt_cfg = {
//         .broker.address.uri = uri,
//         .credentials.username = TB_TOKEN,
//     };

//     client = esp_mqtt_client_init(&mqtt_cfg);
//     esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
//     esp_mqtt_client_start(client);
// }

#include "tb_client.h"
#include "ota_manager.h"
#include "mqtt_client.h"
#include "cJSON.h"
#include "esp_log.h"
#include <string.h>

#define TB_TOKEN "5itnsdtlveutd8yn4sdl"
#define TB_HOST  "mqtt.thingsboard.cloud"

static const char *TAG = "TB_CLIENT";
esp_mqtt_client_handle_t client = NULL;

static void report_current_firmware(void)
{
    const char *msg = "{\"current_fw_title\":\"firmware2\",\"current_fw_version\":\"1.0.0\",\"fw_state\":\"UPDATED\"}";
    esp_mqtt_client_publish(client, "v1/devices/me/attributes", msg, 0, 1, 0);
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                               int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;

    switch (event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Connected to ThingsBoard");
            esp_mqtt_client_subscribe(client, "v1/devices/me/attributes", 1);
            report_current_firmware();
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "Received Topic: %.*s", event->topic_len, event->topic);
            ESP_LOGI(TAG, "Data: %.*s", event->data_len, event->data);

            if (strstr(event->topic, "attributes") != NULL) {
                cJSON *root = cJSON_ParseWithLength(event->data, event->data_len);
                if (root) {
                    // Case 1: Direct fw_url
                    cJSON *fw_url = cJSON_GetObjectItem(root, "fw_url");
                    if (cJSON_IsString(fw_url) && fw_url->valuestring) {
                        ESP_LOGI(TAG, ">>> OTA URL FOUND: %s", fw_url->valuestring);
                        ota_start(fw_url->valuestring);
                        cJSON_Delete(root);
                        return;
                    }

                    // Case 2: Nested in object
                    cJSON *item = root->child;
                    while (item) {
                        if (strcmp(item->string, "fw_url") == 0 && cJSON_IsString(item)) {
                            ESP_LOGI(TAG, ">>> OTA URL FOUND: %s", item->valuestring);
                            ota_start(item->valuestring);
                            break;
                        }
                        item = item->next;
                    }
                    cJSON_Delete(root);
                }
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

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}