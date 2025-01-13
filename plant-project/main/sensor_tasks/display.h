#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h> // For bool type

// Forward declaration of SSD1306_t struct
typedef struct
{
    int _address;
    int _width;
    int _height;
    int _pages;
    int _dc;
    void *_SPIHandle; // Use appropriate type if not void*
    bool _scEnable;
    int _scStart;
    int _scEnd;
    int _scDirection;
    void *_page; // Replace with actual page structure if defined
    bool _flip;
} SSD1306_t;

// Display Initialization
void initialize_display();

// Update moisture text on the display
void set_moisture_text(char text[], int text_len);

// Update temperature text on the display
void set_temp_text(char text[], int text_len);

// Set up the moisture text to update dynamically
void set_updater_moisture_sensor(char filePath[], int path_len);

// Set up the temperature text to update dynamically
void set_updater_temp_sensor(char filePath[], int path_len);

// Task for dynamically updating temperature text
void temp_updater(void *param);

// Task for dynamically updating moisture text
void moisture_updater(void *param);

#endif // DISPLAY_H
