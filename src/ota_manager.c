// #include "ota_manager.h"
// #include "esp_https_ota.h"
// #include "esp_log.h"

// static const char *TAG = "OTA";

// void ota_start(const char *url)
// {
//     esp_http_client_config_t config = {
//         .url = url,
//     };

//     ESP_LOGI(TAG, "Starting OTA from %s", url);

//     if (esp_https_ota(&config) == ESP_OK) {
//         ESP_LOGI(TAG, "OTA OK, restarting...");
//         esp_restart();
//     } else {
//         ESP_LOGE(TAG, "OTA failed");
//     }
// }

#include "ota_manager.h"
#include "esp_https_ota.h"
#include "esp_log.h"

static const char *TAG = "OTA";

void ota_start(const char *url)
{
    esp_http_client_config_t http_config = {
        .url = url,
    };

    esp_https_ota_config_t ota_config = {
        .http_config = &http_config,
    };

    ESP_LOGI(TAG, "Starting OTA from %s", url);

    esp_err_t ret = esp_https_ota(&ota_config);

    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "OTA OK, restarting...");
        esp_restart();
    } else {
        ESP_LOGE(TAG, "OTA failed");
    }
}