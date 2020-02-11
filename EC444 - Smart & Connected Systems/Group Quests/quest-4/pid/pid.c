#include <stdio.h>
#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"

#define TIMER_DIVIDER         16    //  Hardware timer clock divider
#define TIMER_SCALE           (TIMER_BASE_CLK / TIMER_DIVIDER)  // to seconds
#define TIMER_INTERVAL_SEC   (1)    // Sample test interval for the first timer
#define TEST_WITH_RELOAD      1     // Testing will be done with auto reload

static const int RX_BUF_SIZE = 1024;

#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)

#define RED_GPIO 33
#define BLUE_GPIO 32
#define GREEN_GPIO 15

// A simple structure to pass "events" to main task
typedef struct {
    int flag;     // flag for enabling stuff in main code
} timer_event_t;

// Initialize queue handler for timer-based events
xQueueHandle timer_queue;

// ISR handler
void IRAM_ATTR timer_group0_isr(void *para) {

    // Prepare basic event data, aka set flag
    timer_event_t evt;
    evt.flag = 1;

    // Clear the interrupt, Timer 0 in group 0
    TIMERG0.int_clr_timers.t0 = 1;

    // After the alarm triggers, we need to re-enable it to trigger it next time
    TIMERG0.hw_timer[TIMER_0].config.alarm_en = TIMER_ALARM_EN;

    // Send the event data back to the main program task
    xQueueSendFromISR(timer_queue, &evt, NULL);
}

// Initialize timer 0 in group 0 for 1 sec alarm interval and auto reload
static void alarm_init() {
    /* Select and initialize basic parameters of the timer */
    timer_config_t config;
    config.divider = TIMER_DIVIDER;
    config.counter_dir = TIMER_COUNT_UP;
    config.counter_en = TIMER_PAUSE;
    config.alarm_en = TIMER_ALARM_EN;
    config.intr_type = TIMER_INTR_LEVEL;
    config.auto_reload = TEST_WITH_RELOAD;
    timer_init(TIMER_GROUP_0, TIMER_0, &config);

    // Timer's counter will initially start from value below
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0x00000000ULL);

    // Configure the alarm value and the interrupt on alarm
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, TIMER_INTERVAL_SEC * TIMER_SCALE);
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);
    timer_isr_register(TIMER_GROUP_0, TIMER_0, timer_group0_isr,
        (void *) TIMER_0, ESP_INTR_FLAG_IRAM, NULL);

    // Start timer
    timer_start(TIMER_GROUP_0, TIMER_0);
}

// The main task of this example program
static void timer_evt_task(void *arg) {
    while (1) {
        // Create dummy structure to store structure from queue
        timer_event_t evt;

        // Transfer from queue
        xQueueReceive(timer_queue, &evt, portMAX_DELAY);

        // Do something if triggered!
        if (evt.flag == 1) {
            printf("Action!\n");
        }
    }
}

void microlidar_init(void) {
    printf("Initializing\n");
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    printf("Initialized\n");
}

double distance = 0;

static void rx_task(void *arg)
{
    printf("Initializing Rx\n");
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            int raw = 0;
            double count = 0.0;
            // ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
            for(int i=0; i<rxBytes-2; i++){
                if(data[i] == 0x59 && data[i+1] == 0x59){
                    raw += data[i+2];
                    count += 1;
                }
            }
            distance = raw/count;
            printf("Distance: %f cm\n\n", raw/count);
            // ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
        }
    }
    free(data);
}

static void errorLED_task(void *arg){
    double previous_error = 0.0;
    double integral = 0.0;
    double setpoint = 50.0;
    double dt = 1000.0;

    int Kp = 1;
    int Ki = 0.1;
    int Kd = 1;

    // Configure the IOMUX register for pad GPIO variables
    gpio_pad_select_gpio(RED_GPIO);
    gpio_pad_select_gpio(BLUE_GPIO);
    gpio_pad_select_gpio(GREEN_GPIO);

    // Set the GPIOs as a push/pull output
    gpio_set_direction(RED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(BLUE_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(GREEN_GPIO, GPIO_MODE_OUTPUT);

    gpio_set_level(GREEN_GPIO, 0);
    gpio_set_level(RED_GPIO, 0);
    gpio_set_level(BLUE_GPIO, 0);

    while(1)
    {
        // Create dummy structure to store structure from queue
        timer_event_t evt;

        // Transfer from queue
        xQueueReceive(timer_queue, &evt, portMAX_DELAY);

        // Do something if triggered!
        if (evt.flag == 1) {
            printf("Action!\n");
            double error = setpoint - distance;
            integral = integral + error * dt;
            double derivative = (error - previous_error) / dt;
            double output = Kp * error + Ki * integral + Kd * derivative;
            previous_error = error;
            printf("Error: %f\n", output);

            if(output < 0){
                gpio_set_level(GREEN_GPIO, 0);
                gpio_set_level(RED_GPIO, 1);
                gpio_set_level(BLUE_GPIO, 0);
            }
            else if(output > 0){
                gpio_set_level(GREEN_GPIO, 0);
                gpio_set_level(RED_GPIO, 0);
                gpio_set_level(BLUE_GPIO, 1);
            }
            else{
                gpio_set_level(GREEN_GPIO, 1);
                gpio_set_level(RED_GPIO, 0);
                gpio_set_level(BLUE_GPIO, 0);
            }
        }
    }
}



void app_main(void) {
    // Create a FIFO queue for timer-based
    timer_queue = xQueueCreate(10, sizeof(timer_event_t));

    microlidar_init();

    // Create task to handle timer-based events
    xTaskCreate(errorLED_task, "errorLED_task", 2048, NULL, 5, NULL);
    xTaskCreate(rx_task, "uart_rx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);

    // Initiate alarm using timer API
    alarm_init();
}