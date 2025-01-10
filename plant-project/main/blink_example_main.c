#include "freertos/FreeRTOS.h" // FreeRTOS base functionality
#include "freertos/task.h"     // Task creation and delay functions
#include "driver/gpio.h"       // GPIO control and configuration
#include "esp_system.h"        // ESP32 system-related functions
#include "esp_log.h"           // Logging utilities
#include "esp_timer.h"         // High-resolution timer API
#include "sdkconfig.h"         // Project configuration

#define BTN_PIN 2
#define LED_PIN 3

#define ESP_INTR_FLAG_DEFAULT 0

static TaskHandle_t task_handle = NULL;
static const char *TAG = "GPIO_INT";

static void IRAM_ATTR gpio_button_intr(void *arg)
{
    static uint32_t last_isr_time = 0;
    uint32_t current_isr_time = esp_timer_get_time();

    // Debounce: Check if the ISR is triggered within 200ms of the last trigger
    if (current_isr_time - last_isr_time > 200000)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xTaskNotifyFromISR(task_handle, 0, eNoAction, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR();
        }
    }
    last_isr_time = current_isr_time;
}

void led_blink_task(void *arg)
{
    while (1)
    {
        // Wait for notification from ISR
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        ESP_LOGI(TAG, "Button pressed! Starting LED blink.");
        // Perform the LED blinking
        for (int i = 0; i < 5; i++)
        {
            gpio_set_level(LED_PIN, i % 2);
            vTaskDelay(300 / portTICK_PERIOD_MS);
        }
        gpio_set_level(LED_PIN, 0); // Turn off LED after blinking
    }
}

void app_main()
{
    // Configure the GPIO pin as an input
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_NEGEDGE,    // Interrupt on falling edge
        .mode = GPIO_MODE_INPUT,           // Set as input mode
        .pin_bit_mask = (1ULL << BTN_PIN), // Pin bitmask
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE, // Enable pull-up resistor
    };
    gpio_config(&io_conf);

    gpio_config_t led_conf = {
        .intr_type = GPIO_INTR_DISABLE,    // No interrupt for LED
        .mode = GPIO_MODE_OUTPUT,          // Set as output mode
        .pin_bit_mask = (1ULL << LED_PIN), // Pin bitmask for LED
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };
    gpio_config(&led_conf);

    // Install GPIO ISR service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    // Hook the ISR handler for the specific pin
    gpio_isr_handler_add(BTN_PIN, gpio_button_intr, (void *)BTN_PIN);

    xTaskCreate(led_blink_task, "led_blink_task", 2048, NULL, 10, &task_handle);

    ESP_LOGI(TAG, "GPIO interrupt example initialized.");

    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Main loop doing nothing
    }
}
