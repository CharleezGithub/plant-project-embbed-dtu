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

static void IRAM_ATTR gpio_button_intr(void *arg)
{
    // Post a notification to a task instead of delaying in the ISR
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTaskNotifyFromISR(task_handle, 0, eNoAction, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken)
    {
        portYIELD_FROM_ISR();
    }
}

void led_blink_task(void *arg)
{
    while (1)
    {
        // Wait for notification from ISR
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Perform the LED blinking
        for (int i = 0; i < 5; i++)
        {
            gpio_set_level(LED_PIN, i % 2);
            vTaskDelay(300 / portTICK_PERIOD_MS);
        }
    }
}

void app_main()
{
    // Configure the GPIO pin as an input
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_NEGEDGE,        // Interrupt on falling edge
        .mode = GPIO_MODE_INPUT,               // Set as input mode
        .pin_bit_mask = (1ULL << BTN_PIN),     // Pin bitmask
        .pull_down_en = GPIO_PULLDOWN_DISABLE, // Disable pull-down resistor
        .pull_up_en = GPIO_PULLUP_ENABLE,      // Enable pull-up resistor
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

    printf("GPIO interrupt example initialized.\n");

    int flip = 0;

    // Loop to simulate main application
    while (1)
    {
        printf("Test Setting led on or off");
        if (flip)
        {
            gpio_set_level(LED_PIN, 1);
            flip = 0;
        }
        else
        {
            gpio_set_level(LED_PIN, 0);
            flip = 1;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        printf("Waiting for interrupt...\n");
    }
}
