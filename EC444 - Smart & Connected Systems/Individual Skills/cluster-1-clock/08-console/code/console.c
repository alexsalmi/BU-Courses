#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_vfs_dev.h"

#define BUF_SIZE (1024)
#define BLINK_GPIO 13

static void echo_task(void *arg)
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    // set up uart
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE,UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0);
    esp_vfs_dev_uart_use_driver(UART_NUM_0);

    // Configure a temporary buffer for the incoming data
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);

    // Prepare led
    int led_level = 0;
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(BLINK_GPIO, 1);

    // mode variable to select between toggle, echo, and dec to hex
    int mode = 0;
    printf("toggle mode\n");

    // main program loop
    while(1){
        // decide which prompt to print based on which mode is selected
        switch (mode) {
            case 0: // toggle LED
                printf("Read: ");
                break;
            case 1: // echo
                printf("echo: ");
                break;
            case 2: // echo dec to hex
                printf("Enter an integer: \n");
                break;
            default:
                break;
        }

        // loop variable, and string to store input, and index to iterate through string
        bool loop = true;
        char *str = NULL;
        str = (char*)calloc(200, sizeof(char));
        int ind = 0;

        // reading input using uart
        while (loop) {
            // Read data from the UART
            int len = uart_read_bytes(UART_NUM_0, data, BUF_SIZE, 20 / portTICK_RATE_MS);
            // Write data back to the UART
            // uart_write_bytes(UART_NUM_0, (const char *) data, len);
            if(len > 0){
                // if data[0] is 13, user pressed enter, and we exit the loop to move on to logic
                if(data[0] == 13){
                    loop = false;
                }
                // if data[0] is anything else, dd it to the input string, increment current index
                else{
                    str[ind] = data[0];
                    ind++;
                }
            }
        }

        // if the input string is only 's', switch mode
        if(ind == 1 && str[0] == 's'){
            mode = (mode + 1) % 3;
            printf("s\n");
            switch (mode) {
                case 0: // toggle LED
                    printf("toggle mode \n");
                    break;
                case 1: // echo
                    printf("echo mode \n");
                    break;
                case 2: // echo dec to hex
                    printf("echo dec to hex mode \n");
                    break;
            }
        }
        else{
            // perform logic based on current mode
            switch (mode) {
                case 0: // toggle LED
                    // print the input string, and if it is only 't', toggle the LED
                    printf("%s\n", str);
                    if(ind == 1 && str[0] == 't'){
                        gpio_set_level(BLINK_GPIO, led_level);
                        led_level = (led_level + 1)%2;
                    }
                    break;
                case 1: // echo
                    // print the input string
                    printf("%s\n", str);
                    break;
                case 2: // echo dec to hex
                    printf("Hex: ");
                    // convert input string to int, so we can convert to hex
                    int dec = atoi(str);
                    // reinitialize str to stor the hex string
                    free(str);
                    char *str = NULL;
                    str = (char*)calloc(200, sizeof(char));
                    ind = 0;
                    // convert from dec to hex
                    while(dec != 0){
                        int digit = dec % 16;
                        switch (digit) {
                            case 10:
                                str[ind] = 'A';
                                break;
                            case 11:
                                str[ind] = 'B';
                                break;
                            case 12:
                                str[ind] = 'C';
                                break;
                            case 13:
                                str[ind] = 'D';
                                break;
                            case 14:
                                str[ind] = 'E';
                                break;
                            case 15:
                                str[ind] = 'F';
                                break;
                            default:
                                str[ind] = digit + '0';
                                break;
                        }
                        ind++;
                        dec = dec/16;
                    }
                    // print string in reverse, to print the correct hex number
                    for(int i = strlen(str)-1; i>=0; i--){
                        printf("%c", str[i]);
                    }
                    printf("\n");
                    break;
            }
        }
        // free the allocated memory for the input string
        free(str);
    }
}

void app_main(void)
{
    xTaskCreate(echo_task, "uart_echo_task", 1024, NULL, 10, NULL);
}
