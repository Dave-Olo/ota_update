#pragma once
#include "mqtt_client.h"     // This is the important line

void tb_init(void);

extern esp_mqtt_client_handle_t client;   // Global client for OTA status