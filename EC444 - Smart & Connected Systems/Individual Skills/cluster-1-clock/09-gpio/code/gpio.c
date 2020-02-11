#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

// Pin numbers for the four LEDs
#define RED_GPIO 15
#define BLUE_GPIO 33
#define YELLOW_GPIO 27
#define GREEN_GPIO 12

void gpio_task(void *pvParameter)
{
    // Configure the IOMUX register for pad GPIO variables
    gpio_pad_select_gpio(RED_GPIO);
    gpio_pad_select_gpio(BLUE_GPIO);
    gpio_pad_select_gpio(YELLOW_GPIO);
    gpio_pad_select_gpio(GREEN_GPIO);

    // Set the GPIOs as a push/pull output
    gpio_set_direction(RED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(BLUE_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(YELLOW_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(GREEN_GPIO, GPIO_MODE_OUTPUT);

    // Initiate counting variable
    int num = 0;
    while(1) {
        // Copy number to be able to manipulate it
        int tempnum = num;
        // Check if bit 0 is pos or neg
        if(tempnum%2 == 1){
          gpio_set_level(GREEN_GPIO, 1);
        }
        else{
          gpio_set_level(GREEN_GPIO, 0);
        }
        // Divide by 2 to get next bit
        tempnum = tempnum / 2;

        // Check if bit 1 is pos or neg
        if(tempnum%2 == 1){
          gpio_set_level(YELLOW_GPIO, 1);
        }
        else{
          gpio_set_level(YELLOW_GPIO, 0);
        }
        tempnum = tempnum / 2;

        // Check if bit 2 is pos or neg
        if(tempnum%2 == 1){
          gpio_set_level(BLUE_GPIO, 1);
        }
        else{
          gpio_set_level(BLUE_GPIO, 0);
        }
        tempnum = tempnum / 2;

        // Check if bit 3 is pos or neg
        if(tempnum%2 == 1){
          gpio_set_level(RED_GPIO, 1);
        }
        else{
          gpio_set_level(RED_GPIO, 0);
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // Increment counting variable, loop back around if 16
        num += 1;
        if(num == 16){
          num = 0;
        }
    }
}

void app_main()
{
    xTaskCreate(&gpio_task, "gpio_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}
