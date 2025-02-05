#include "freertos/FreeRTOS.h" // FreeRTOS base functionality
#include "freertos/task.h"     // Task creation and delay functions
#include "driver/gpio.h"       // GPIO control and configuration
#include "esp_system.h"        // ESP32 system-related functions
#include "esp_log.h"           // Logging utilities
#include "esp_timer.h"         // High-resolution timer API
#include "sdkconfig.h"         // Project configuration
#include "sensor_tasks/display.c"
#include "libs/ssd1306/ssd1306.h"

void app_main()
{
    initialize_display();

    printf("Display Initialized!\n");
}
