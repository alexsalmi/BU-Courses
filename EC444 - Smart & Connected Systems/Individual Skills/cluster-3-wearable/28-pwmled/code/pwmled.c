#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/uart.h"
#include "esp_err.h"
#include "esp_vfs_dev.h"

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

#define LEDC_TEST_CH_NUM       (4)
#define LEDC_TEST_DUTY         (4000)
#define BRIGHTNESS_90          (3600)
#define BRIGHTNESS_80          (3200)
#define BRIGHTNESS_70          (2800)
#define BRIGHTNESS_60          (2400)
#define BRIGHTNESS_50          (2000)
#define BRIGHTNESS_40          (1600)
#define BRIGHTNESS_30          (1200)
#define BRIGHTNESS_20          (800)
#define BRIGHTNESS_10          (400)
#define BRIGHTNESS_00          (0)
#define LEDC_TEST_FADE_TIME    (3000)

#define BUF_SIZE (1024)

void app_main(void)
{
    int ch;

    /*
     * Prepare and set configuration of timers
     * that will be used by LED Controller
     */
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
        .freq_hz = 5000,                      // frequency of PWM signal
        .speed_mode = LEDC_HS_MODE,           // timer mode
        .timer_num = LEDC_HS_TIMER,            // timer index
        .clk_cfg = LEDC_AUTO_CLK,              // Auto select the source clock
    };
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);

    // Prepare and set configuration of timer1 for low speed channels
    ledc_timer.speed_mode = LEDC_LS_MODE;
    ledc_timer.timer_num = LEDC_LS_TIMER;
    ledc_timer_config(&ledc_timer);

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

    /*
     * Prepare individual configuration
     * for each channel of LED Controller
     * by selecting:
     * - controller's channel number
     * - output duty cycle, set initially to 0
     * - GPIO number where LED is connected to
     * - speed mode, either high or low
     * - timer servicing selected channel
     *   Note: if different channels use one timer,
     *         then frequency and bit_num of these channels
     *         will be the same
     */
    ledc_channel_config_t ledc_channel[LEDC_TEST_CH_NUM] = {
        {
            .channel    = LEDC_HS_CH0_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_HS_CH0_GPIO,
            .speed_mode = LEDC_HS_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_HS_TIMER
        },
        {
            .channel    = LEDC_HS_CH1_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_HS_CH1_GPIO,
            .speed_mode = LEDC_HS_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_HS_TIMER
        },
        {
            .channel    = LEDC_LS_CH2_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_LS_CH2_GPIO,
            .speed_mode = LEDC_LS_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_LS_TIMER
        },
        {
            .channel    = LEDC_LS_CH3_CHANNEL,
            .duty       = 0,
            .gpio_num   = LEDC_LS_CH3_GPIO,
            .speed_mode = LEDC_LS_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_LS_TIMER
        },
    };

    // Set LED Controller with previously prepared configuration
    for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
        ledc_channel_config(&ledc_channel[ch]);
    }

    // Initialize fade service.
    ledc_fade_func_install(0);

    char mode = '0';
    int level = 0;

    while (1) {
        int len = uart_read_bytes(UART_NUM_0, data, BUF_SIZE, 20 / portTICK_RATE_MS);

        if(len > 0){
            mode = data[0];
            if(mode == 'u'){
                level = 0;
                printf("Cycling up through intensity levels\n");
            }
            else if(mode == 'd'){
                level = 0;
                printf("Cycling down through intensity levels\n");
            }
            else{
                level = data[0] - '0';
                printf("Setting intensity level to %c\n", mode);
            }
        }

        switch(level){
            case 0:
                for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
                    ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, BRIGHTNESS_00);
                    ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
                }
                break;
            case 1:
                for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
                    ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, BRIGHTNESS_10);
                    ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
                }
                break;
            case 2:
                for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
                    ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, BRIGHTNESS_20);
                    ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
                }
                break;
            case 3:
                for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
                    ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, BRIGHTNESS_30);
                    ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
                }
                break;
            case 4:
                for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
                    ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, BRIGHTNESS_40);
                    ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
                }
                break;
            case 5:
                for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
                    ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, BRIGHTNESS_50);
                    ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
                }
                break;
            case 6:
                for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
                    ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, BRIGHTNESS_60);
                    ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
                }
                break;
            case 7:
                for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
                    ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, BRIGHTNESS_70);
                    ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
                }
                break;
            case 8:
                for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
                    ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, BRIGHTNESS_80);
                    ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
                }
                break;
            case 9:
                for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
                    ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, BRIGHTNESS_90);
                    ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
                }
                break;
            default:
                break;
        }

        if(mode == 'u'){
            level = (level+1)%10;
            vTaskDelay(250 / portTICK_PERIOD_MS);
        }
        else if(mode == 'd'){
            level = level-1;
            if(level == -1)
                level = 9;
            vTaskDelay(250 / portTICK_PERIOD_MS);
        }
    }
}
