idf_component_register(SRCS "main.c" 
                            "sensor_tasks/display.c"
                            "libs/ssd1306/ssd1306.c"
                            "sensor_tasks/display.c"  # Add all source files here
                    PRIV_REQUIRES spi_flash driver
                    INCLUDE_DIRS "." "libs" "sensor_tasks")
