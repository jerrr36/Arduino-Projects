#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_system.h"

void hello(void *parameter) {
    

    while (1) {
        printf("Hello world\n");
        
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
    
}

void blink (void *parameter) {

    int led = 2;

    // setting pin as gpio
    gpio_pad_select_gpio(led);

    // setting gpio pin as output
    gpio_set_direction(led, GPIO_MODE_OUTPUT);

    

    while(1) {

        // setting low
        gpio_set_level(led, 0);

        vTaskDelay(500 / portTICK_PERIOD_MS);

        // setting high
        gpio_set_level(led, 1);

        vTaskDelay(500 / portTICK_PERIOD_MS);

    }
}

void app_main() {
    printf("Basic usage of freertos\n");
    
    // creating task on core 0 
    xTaskCreatePinnedToCore(&hello, "hello task", 2048, NULL, 3, NULL, 0);
    xTaskCreatePinnedToCore(&blink, "blink led", 1028, NULL, 2, NULL, 1);

}