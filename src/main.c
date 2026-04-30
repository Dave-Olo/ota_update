// #include "wifi_manager.h"
// #include "tb_client.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"

// void app_main(void)
// {
//     wifi_init();

//     vTaskDelay(pdMS_TO_TICKS(5000)); // wait for connection

//     tb_init();
// }

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "wifi_manager.h"
#include "tb_client.h"

#define LED_PIN GPIO_NUM_2

void led_task(void *pv)
{
    while (1) {
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 second

        gpio_set_level(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    wifi_init();

    vTaskDelay(pdMS_TO_TICKS(5000));

    tb_init();

    xTaskCreate(led_task, "led_task", 2048, NULL, 5, NULL);
}