#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include <string.h>
#include "esp_log.h"

#include "libs/ssd1306/ssd1306.h"
#include "libs/ssd1306/font8x8_basic.h"
// Driver libraries
#include "driver/i2c.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/adc.h"

#define I2C_MASTER_SDA 6
#define I2C_MASTER_SCL 7

struct SensorText
{
    SSD1306_t dev;
} SensorText;

// Initializes the i2c connection
void initialize_display()
{
    // Initialize i2c
    i2c_master_shared_i2c_init(&SensorText.dev);

    // Initialize display
    ssd1306_init(&SensorText.dev, 128, 64);

    ssd1306_clear_screen(&SensorText.dev, false);
    ssd1306_contrast(&SensorText.dev, 0xff);
    ssd1306_display_text_x3(&SensorText.dev, 0, "Hello", 5, false);
}

// Change the moisture text
void set_moisture_text(char text[], int text_len)
{
    // TODO
}

// Change the temp text
void set_temp_text(char text[], int text_len)
{
    // TODO
}

// Set the moisture text to update when the file in filePath changes/updates
void set_updater_moisture_sensor(char filePath[], int path_len)
{
    // TODO
}

// Set the temp text to update when the file in filePath changes/updates
void set_updater_temp_sensor(char filePath[], int path_len)
{
    // TODO
}

// Updates temp text on file change
void temp_updater(void *)
{
    // TODO
}

// Updates moisture text on file change
void moisture_updater(void *)
{
    // TODO
}