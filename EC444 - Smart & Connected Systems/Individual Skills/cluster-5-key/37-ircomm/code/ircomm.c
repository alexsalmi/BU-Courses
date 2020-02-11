/* UART asynchronous example, that uses separate RX and TX tasks

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
   test commitasdfasdf
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"
#include "driver/rmt.h"
#include "driver/gpio.h"
static const int RX_BUF_SIZE = 1024;
char * color;
int count = 0;



#define RMT_TX_CHANNEL 1
#define RMT_TX_GPIO 26
#define SAMPLE_CNT  10


#define TXD_PIN 25
#define RXD_PIN 34
#define BLUE_LED 32
#define GREEN_LED 15
#define RED_LED 33
#define RMT_PIN 26
#define BTN_PIN 4

void init(void) {
    printf("Initializing\n");
    const uart_config_t uart_config = {
        .baud_rate = 2400,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_set_line_inverse(UART_NUM_1, UART_INVERSE_RXD);
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    printf("Initialized\n");
}

static void rmt_tx_int(void)
{
    rmt_config_t config;
    config.rmt_mode = 0;
    config.channel = RMT_TX_CHANNEL;
    config.gpio_num = RMT_TX_GPIO;
    config.mem_block_num = 1;
    config.tx_config.loop_en = 0;
    // enable the carrier to be able to hear the Morse sound
    // if the RMT_TX_GPIO is connected to a speaker
    config.tx_config.carrier_en = 1;
    config.tx_config.idle_output_en = 1;
    config.tx_config.idle_level = 1;
    config.tx_config.carrier_duty_percent = 50;
    // set audible career frequency of 611 Hz
    // actually 611 Hz is the minimum, that can be set
    // with current implementation of the RMT API
    config.tx_config.carrier_freq_hz = 38000;
    config.tx_config.carrier_level = 1;
    // set the maximum clock divider to be able to output
    // RMT pulses in range of about one hundred milliseconds
    config.clk_div = 100;

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));
    //ESP_ERROR_CHECK(rmt_translator_init(config.channel, NULL));
}

int sendData(const char* logName, const char* data)
{
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
    return txBytes;
}

static void tx_task(void *arg)
{
    
    static const char *TX_TASK_TAG = "TX_TASK";
    esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
    while (1) {
        //printf("sending...\n");

        //sendData(TX_TASK_TAG, "Hello world");
        //const char* data = "Hellow world";
        if(count % 3 == 0){
            color = "RED";
        }
        else if(count % 3 == 1){
            color = "GREEN";
        }
        else if(count % 3 == 2){
            color = "BLUE";
        }
        const int len = strlen(color);
        const int txBytes = uart_write_bytes(UART_NUM_1, color, len);
        //ESP_LOGI(logName, "Wrote %d bytes", txBytes);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

static void rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    while (1) {
        //printf("Reading...\n");
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 100 / portTICK_RATE_MS);
        //printf("bytes:  %d",rxBytes);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            printf("Read %d bytes: '%s'\n", rxBytes, (char*)data);
            //printf("%s\n",(char*)data);
            if(count % 3 == 0){
                gpio_set_level(RED_LED,1);
                gpio_set_level(BLUE_LED,0);
                gpio_set_level(GREEN_LED,0);
            }
            else if(count % 3 == 1){
                gpio_set_level(RED_LED,0);
                gpio_set_level(BLUE_LED,0);
                gpio_set_level(GREEN_LED,1);
            }
            else if(count % 3 == 2){
                gpio_set_level(RED_LED,0);
                gpio_set_level(BLUE_LED,1);
                gpio_set_level(GREEN_LED,0);
            }
            //ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, (char*)data);
            //ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
        }
    }
    free(data);
}

static void IRAM_ATTR gpio_isr_handler(void* arg){
    count++;		// Toggle state of flag
}

void app_main(void)
{
    rmt_tx_int();
    init();
    
    gpio_pad_select_gpio(BTN_PIN);
    gpio_set_direction(BTN_PIN,GPIO_MODE_INPUT);
    gpio_set_pull_mode(BTN_PIN,GPIO_PULLDOWN_ENABLE);
    gpio_set_intr_type(BTN_PIN,GPIO_INTR_POSEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BTN_PIN,gpio_isr_handler,(void*)BTN_PIN);

    gpio_pad_select_gpio(GREEN_LED);
    gpio_set_direction(GREEN_LED, GPIO_MODE_OUTPUT);

    gpio_pad_select_gpio(RED_LED);
    gpio_set_direction(RED_LED, GPIO_MODE_OUTPUT);

    gpio_pad_select_gpio(BLUE_LED);
    gpio_set_direction(BLUE_LED, GPIO_MODE_OUTPUT);

    xTaskCreate(rx_task, "uart_rx_task", 1024*2, NULL, 2, NULL);
    xTaskCreate(tx_task, "uart_tx_task", 1024*2, NULL, 1, NULL);
}
