// QUEST 3: Wearable
// TEAM 17: Karle Erf, Thuc Nguyen, Alex Salmi

// Main code for our sensor circuit/data collection. Gets the data from the sensors, converts the data to scientific units, and then sends the data to the node server using UDP sockets. In the response from the server, we get info on which sensors should be active, and controls for the LEDs

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#include "driver/uart.h"
#include "esp_vfs_dev.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "tcpip_adapter.h"
#include "protocol_examples_common.h"

#include "driver/ledc.h"
#include "esp_err.h"

#include "esp_adc_cal.h"
#include <math.h>

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>


#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV4_ADDR

#define PORT CONFIG_EXAMPLE_PORT

static const char *TAG = "data";

/**
 * Brief:
 * This test code shows how to configure gpio and how to use gpio interrupt.
 *
 * GPIO status:
 * GPIO18: output
 * GPIO19: output
 * GPIO4:  input, pulled up, interrupt from rising edge and falling edge
 * GPIO5:  input, pulled up, interrupt from rising edge.
 *
 * Test:
 * Connect GPIO18 with GPIO4
 * Connect GPIO19 with GPIO5
 * Generate pulses on GPIO18/19, that triggers interrupt on GPIO4/5
 *
 */
#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   10          //Multisampling

//thermistor adc channel config
static esp_adc_cal_characteristics_t *adc_chars_therm;
static const adc_channel_t channel_therm = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten_therm = ADC_ATTEN_DB_11;
static const adc_unit_t unit_therm = ADC_UNIT_1;
//battery adc channel config
static esp_adc_cal_characteristics_t *adc_chars_batt;
static const adc_channel_t channel_batt = ADC_CHANNEL_4;     //GPIO32 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten_batt = ADC_ATTEN_DB_11;
static const adc_unit_t unit_batt = ADC_UNIT_1;
// 14-Segment Display
#define SLAVE_ADDR                         0x70 // alphanumeric address
#define OSC                                0x21 // oscillator cmd
#define HT16K33_BLINK_DISPLAYON            0x01 // Display on cmd
#define HT16K33_BLINK_OFF                  0    // Blink off cmd
#define HT16K33_BLINK_CMD                  0x80 // Blink cmd
#define HT16K33_CMD_BRIGHTNESS             0xE0 // Brightness cmd

// Master I2C
#define I2C_EXAMPLE_MASTER_SCL_IO          22   // gpio number for i2c clk
#define I2C_EXAMPLE_MASTER_SDA_IO          23   // gpio number for i2c data
#define I2C_EXAMPLE_MASTER_NUM             I2C_NUM_0  // i2c port
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE  0    // i2c master no buffer needed
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE  0    // i2c master no buffer needed
#define I2C_EXAMPLE_MASTER_FREQ_HZ         100000     // i2c master clock freq
#define WRITE_BIT                          I2C_MASTER_WRITE // i2c master write
#define READ_BIT                           I2C_MASTER_READ  // i2c master read
#define ACK_CHECK_EN                       true // i2c master will check ack
#define ACK_CHECK_DIS                      false// i2c master will not check ack
#define ACK_VAL                            0x00 // i2c ack value
#define NACK_VAL                           0xFF // i2c nack value

#define LEDC_HS_TIMER          LEDC_TIMER_0
#define LEDC_HS_MODE           LEDC_HIGH_SPEED_MODE
#define LEDC_HS_CH0_GPIO       (12)
#define LEDC_HS_CH0_CHANNEL    LEDC_CHANNEL_0
#define LEDC_HS_CH1_GPIO       (19)
#define LEDC_HS_CH1_CHANNEL    LEDC_CHANNEL_1

#define LEDC_LS_TIMER          LEDC_TIMER_1
#define LEDC_LS_MODE           LEDC_LOW_SPEED_MODE
#define LEDC_LS_CH2_GPIO       (4)
#define LEDC_LS_CH2_CHANNEL    LEDC_CHANNEL_2
#define LEDC_LS_CH3_GPIO       (5)
#define LEDC_LS_CH3_CHANNEL    LEDC_CHANNEL_3

#define GPIO_OUTPUT_IO_0    18
#define GPIO_OUTPUT_IO_1    19
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1))
#define GPIO_INPUT_IO_0     4
#define GPIO_INPUT_IO_1     5
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0) | (1ULL<<GPIO_INPUT_IO_1))
#define ESP_INTR_FLAG_DEFAULT 0

#define LEDC_TEST_CH_NUM       4

int tempG;
int stepsG;
int batteryG;
int findMyWatch = 0;

//-----TEMP SENSOR FUNCTIONS--------------
static void check_efuse(void)
{
    //Check TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("eFuse Two Point: NOT supported\n");
    }

    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        printf("eFuse Vref: Supported\n");
    } else {
        printf("eFuse Vref: NOT supported\n");
    }
}

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}
//--------STEP COUNT FUNTIONS-----------
static xQueueHandle gpio_evt_queue = NULL;
int step_count = 0;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task_example(void* arg)
{
    uint32_t io_num;
    int  count = 0;
    int on = 0;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            //printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
            if(count == 100){
                //printf("tapped\n");
                step_count++;
                
                count = 0;
            }
            count++;
        }
    }
}

void count_steps(){
    gpio_pad_select_gpio(13);
    gpio_set_direction(13,GPIO_MODE_OUTPUT);
    gpio_set_level(13,0);

    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    //interrupt of rising edge
    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode    
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    //change gpio intrrupt type for one pin
    gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_ANYEDGE);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1);

    //remove isr handler for gpio number.
    gpio_isr_handler_remove(GPIO_INPUT_IO_0);
    //hook isr handler for specific gpio pin again
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);

    int cnt = 0;
    while(1) {
        printf("step_count: %d\n", step_count);
        stepsG = step_count;
        vTaskDelay(1000 / portTICK_RATE_MS);
        gpio_set_level(GPIO_OUTPUT_IO_0, cnt % 2);
        gpio_set_level(GPIO_OUTPUT_IO_1, cnt % 2);
    }
}

void water_LED(){

    gpio_pad_select_gpio(12);
    gpio_set_direction(12, GPIO_MODE_OUTPUT);

    while(1){
        gpio_set_level(12,1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        //printf("LED on\n");
        gpio_set_level(12,0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        //printf("LED  off\n");
    } 
}

void check_temp(){
    //Check if Two Point or Vref are burned into eFuse
    check_efuse();
  

    //Configure ADC
    if (unit_therm == ADC_UNIT_1) {
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(channel_therm, atten_therm);
    } else {
        adc2_config_channel_atten((adc2_channel_t)channel_therm, atten_therm);
    }

    //Characterize ADC
    adc_chars_therm = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit_therm, atten_therm, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars_therm);
    print_char_val_type(val_type);

    //Continuously sample ADC1
    while (1) {
        uint32_t adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            if (unit_therm == ADC_UNIT_1) {
                adc_reading += adc1_get_raw((adc1_channel_t)channel_therm);
            } else {
                int raw;
                adc2_get_raw((adc2_channel_t)channel_therm, ADC_WIDTH_BIT_12, &raw);
                adc_reading += raw;
            }
        }
        adc_reading /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars_therm);
        //printf("Raw: %d\tVoltage: %dmV         ", adc_reading, voltage);
        double volt = (double)voltage;
        double tmp = (1/((1/298.15) + (1/3435.0) * log((5000/volt) - 1))) - 276.15;
        int temp = (int)tmp;
        printf("Temperature is: %d\n",temp);
        tempG = temp;
        vTaskDelay(pdMS_TO_TICKS(1000));

    }
}

void  check_batt(){
        //Check if Two Point or Vref are burned into eFuse
    check_efuse();
    //i2c_example_master_init();
    //i2c_scanner();

    //Configure ADC
    if (unit_batt == ADC_UNIT_1) {
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(channel_batt, atten_batt);
    } else {
        adc2_config_channel_atten((adc2_channel_t)channel_batt, atten_batt);
    }

    //Characterize ADC
    adc_chars_batt = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit_batt, atten_batt, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars_batt);
    print_char_val_type(val_type);

    //Continuously sample ADC1
    while (1) {
        uint32_t adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            if (unit_batt == ADC_UNIT_1) {
                adc_reading += adc1_get_raw((adc1_channel_t)channel_batt);
            } else {
                int raw;
                adc2_get_raw((adc2_channel_t)channel_batt, ADC_WIDTH_BIT_12, &raw);
                adc_reading += raw;
            }
        }
        adc_reading /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars_batt);
        printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
        batteryG = voltage;
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void sendData(){
    char rx_buffer[128];
    char addr_str[128];
    int addr_family;
    int ip_protocol;

    char tempStr[10];
    char stepStr[10];
    char battStr[10];
    char output[33];

    gpio_pad_select_gpio(15);
    gpio_set_direction(15, GPIO_MODE_OUTPUT);
    gpio_set_level(15,0);

    while (1) {
        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
        inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);

        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket created, sending to %s:%d", HOST_IP_ADDR, PORT);

        while (1) {
            itoa(tempG, tempStr, 10);
            itoa(stepsG, stepStr, 10);
            itoa(batteryG, battStr, 10);

            strcpy(output, tempStr);
            strcat(output, "|");
            strcat(output, stepStr);
            strcat(output, "|");
            strcat(output, battStr);

            int err = sendto(sock, output, strlen(output), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            if (err < 0) {
                ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                break;
            }
            ESP_LOGI(TAG, "Message sent");

            struct sockaddr_in source_addr; // Large enough for both IPv4 or IPv6
            socklen_t socklen = sizeof(source_addr);
            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

            // Error occurred during receiving
            if (len < 0) {
                ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
                break;
            }
            // Data received
            else {
                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
                ESP_LOGI(TAG, "%s", rx_buffer);

                if(rx_buffer[0] == 't'){
                    gpio_set_level(15,1);
                    vTaskDelay(1000 / portTICK_PERIOD_MS);
                    //printf("LED on\n");
                    gpio_set_level(15,0);
                }
            }

            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    xTaskCreate(count_steps,"counting_steps", 4096, NULL, 1, NULL);    
    xTaskCreate(water_LED,"water_reminder", 4096, NULL, 2, NULL);
    xTaskCreate(check_temp,"temperature", 4096, NULL, 3, NULL);
    xTaskCreate(check_batt,"battery",4096,NULL,4,NULL);
    xTaskCreate(sendData,"data",4096,NULL,5,NULL);
}

