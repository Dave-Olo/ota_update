#include "ota_manager.h"
#include "esp_https_ota.h"
#include "esp_log.h"
#include "esp_system.h"
#include "mqtt_client.h"      // For publishing status

static const char *TAG = "OTA";

// We will declare client as extern so we can use it
extern esp_mqtt_client_handle_t client;   // From tb_client.c

void ota_start(const char *url)
{
    esp_http_client_config_t http_config = {
        .url = url,
        .timeout_ms = 20000,
    };

    esp_https_ota_config_t ota_config = {
        .http_config = &http_config,
    };

    ESP_LOGI(TAG, "Starting OTA download from: %s", url);

    esp_err_t ret = esp_https_ota(&ota_config);

    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "OTA Success! Restarting...");
        if (client) {
            esp_mqtt_client_publish(client, "v1/devices/me/telemetry", 
                                  "{\"fw_state\":\"UPDATED\"}", 0, 1, 0);
        }
        vTaskDelay(pdMS_TO_TICKS(1500));
        esp_restart();
    } else {
        ESP_LOGE(TAG, "OTA Failed: %s", esp_err_to_name(ret));
        if (client) {
            esp_mqtt_client_publish(client, "v1/devices/me/telemetry", 
                                  "{\"fw_state\":\"FAILED\"}", 0, 1, 0);
        }
    }
}