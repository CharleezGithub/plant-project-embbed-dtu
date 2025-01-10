#include "freertos/FreeRTOS.h" // FreeRTOS base functionality
#include "freertos/task.h"     // Task creation and delay functions
#include "driver/gpio.h"       // GPIO control and configuration
#include "esp_system.h"        // ESP32 system-related functions
#include "esp_log.h"           // Logging utilities
#include "esp_timer.h"         // High-resolution timer API
#include "sdkconfig.h"         // Project configuration

#define LED_GPIO_PIN 3

static const char *TAG = "APP";

void blink_led_task(void *pvParameter)
{
    // This task is commented and not run in app_main.
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_GPIO_PIN), // Select the GPIO pin
        .mode = GPIO_MODE_OUTPUT,               // Set as output mode
        .pull_up_en = GPIO_PULLUP_DISABLE,      // Disable pull-up
        .pull_down_en = GPIO_PULLDOWN_DISABLE,  // Disable pull-down
        .intr_type = GPIO_INTR_DISABLE          // Disable interrupts
    };

    if (gpio_config(&io_conf) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to configure GPIO");
        vTaskDelete(NULL);
    }

    while (1)
    {
        ESP_LOGI(TAG, "LED ON");
        gpio_set_level(LED_GPIO_PIN, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        ESP_LOGI(TAG, "LED OFF");
        gpio_set_level(LED_GPIO_PIN, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void sample_task(void *pvParameter)
{
    ESP_LOGI(TAG, "Sample task started, waiting 2 seconds...");
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "Sample task completed.");
}

void app_main()
{
    // Uncomment this line if you want to run the Blink LED Task:
    if (xTaskCreate(&blink_led_task, "Blink LED Task", 4096, NULL, 5, NULL) != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create Blink LED Task");
    }

    // Create the Sample Task
    int64_t start_time = esp_timer_get_time();

    sample_task(NULL);

    int64_t end_time = esp_timer_get_time();
    ESP_LOGI(TAG, "Elapsed time for task creation: %lld microseconds", end_time - start_time);
}
