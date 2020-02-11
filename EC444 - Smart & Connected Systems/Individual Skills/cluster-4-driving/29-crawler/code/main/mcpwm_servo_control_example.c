/* servo motor control example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_attr.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

//You can get these value from the datasheet of servo you use, in general pulse width varies between 1000 to 2000 mocrosecond
#define SERVO_MIN_PULSEWIDTH 1000 //Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH 2000 //Maximum pulse width in microsecond
#define SERVO_MAX_DEGREE 90 //Maximum angle in degree upto which servo can rotate

/*static void mcpwm_example_gpio_initialize(void)
{
    printf("initializing mcpwm servo control gpio......\n");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, 12);    //Set GPIO 18 as PWM0A, to which servo is connected
    //mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, 12);
}*/

/**
 * @brief Use this function to calcute pulse width for per degree rotation
 *
 * @param  degree_of_rotation the angle in degree to which servo has to rotate
 *
 * @return
 *     - calculated pulse width
 */
static uint32_t servo_per_degree_init(uint32_t degree_of_rotation)
{
    uint32_t cal_pulsewidth = 0;
    cal_pulsewidth = (SERVO_MIN_PULSEWIDTH + (((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * (degree_of_rotation)) / (SERVO_MAX_DEGREE)));
    return cal_pulsewidth;
}

/**
 * @brief Configure MCPWM module
 */
void mcpwm_example_servo_control(void *arg)
{
    uint32_t angle, count;
    //1. mcpwm gpio initialization
    //mcpwm_example_gpio_initialize();
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, 12); 

    //2. initial mcpwm configuration
    printf("Configuring Initial Parameters of mcpwm......\n");
    mcpwm_config_t pwm_configB;
    pwm_configB.frequency = 50;    //frequency = 50Hz, i.e. for every servo motor time period should be 20ms
    pwm_configB.cmpr_a = 0;    //duty cycle of PWMxA = 0
    pwm_configB.cmpr_b = 0;    //duty cycle of PWMxb = 0
    pwm_configB.counter_mode = MCPWM_UP_COUNTER;
    pwm_configB.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_configB);    //Configure PWM0A & PWM0B with above settings
    /*while (1) {
        for (count = 0; count < SERVO_MAX_DEGREE; count++) {
            printf("Angle of rotation: %d\n", count);
            angle = servo_per_degree_init(count);
            printf("pulse width: %dus\n", angle);
            mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
            vTaskDelay(10);     //Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation at 5V
        }
    }*/
    while(1){
        angle = servo_per_degree_init(90);
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, angle);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        angle = servo_per_degree_init(0);
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, angle);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        angle = servo_per_degree_init(40);
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, angle);
        vTaskDelay(5000 / portTICK_PERIOD_MS);

    }

    
}

void  pwm_init(){
    // 1. Set GPIO 18 as PWM0A, to which servo control pin is connected
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, 18);    

    // 2. initial mcpwm configuration
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 50;    //frequency = 50Hz, i.e. for every servo motor time period should be 20ms
    pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;    //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);    //Configure PWM0A & PWM0B with above settings

    // 3. Set pulse width (1500 us) or duty cycle (50%)
    //mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 1500);
    //mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 50);
}

void calibrateESC() {
    //mcpwm_example_gpio_initialize();
    //pwm_init();
    //while(1){
      //      mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 2100); // HIGH signal in microseconds

    //}
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, 18); 
    mcpwm_config_t pwm_configA;
    pwm_configA.frequency = 50;    //frequency = 50Hz, i.e. for every servo motor time period should be 20ms
    pwm_configA.cmpr_a = 0;    //duty cycle of PWMxA = 0
    pwm_configA.cmpr_b = 0;    //duty cycle of PWMxb = 0
    pwm_configA.counter_mode = MCPWM_UP_COUNTER;
    pwm_configA.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_configA);    //Configure PWM0A & PWM0B with above settings

    vTaskDelay(3000 / portTICK_PERIOD_MS);  // Give yourself time to turn on crawler
    printf("setting speed to HIGH\n");
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 2100); // HIGH signal in microseconds
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    printf("setting speed to LOW\n");
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 700);  // LOW signal in microseconds
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    printf("setting speed to NEUTRAL\n");
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 1400); // NEUTRAL signal in microseconds
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 1400); // reset the ESC to neutral (non-moving) value
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    //mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 700);  // LOW signal in microseconds


    //vTaskDelay(3000 / portTICK_PERIOD_MS);
    //mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 1400);

    
    
    /*vTaskDelay(1000 / portTICK_PERIOD_MS);
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 2100); // HIGH signal in microseconds
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 1400); // NEUTRAL signal in microseconds
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 700);  // LOW signal in microseconds
    */


}

void forward(){
    /*for(int i  = 1400; i > 700; i = i - 1){
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, i);
        vTaskDelay(25 / portTICK_PERIOD_MS);
    }*/
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 1400);
    for(int i  = 1400; i > 1000; i = i - 5){
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, i);
        vTaskDelay(25 / portTICK_PERIOD_MS);
    }
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 1000);
}

void reverse(){
    /*for(int i  = 1400; i < 2100; i = i + 1){
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, i);
        vTaskDelay(25 / portTICK_PERIOD_MS);
    }*/
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 1400);
    for(int i  = 1400; i < 1800; i = i + 5){
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, i);
        vTaskDelay(25 / portTICK_PERIOD_MS);
    }
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 1800);

}

void neutral(){
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 1400);
}

void test(){
    calibrateESC();
    while(1){
        forward();
        vTaskDelay(500 / portTICK_PERIOD_MS);
        neutral();
        vTaskDelay(500 / portTICK_PERIOD_MS);
        reverse();
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    
}

void app_main(void)
{

    printf("Testing servo motor.......\n");
    xTaskCreate(mcpwm_example_servo_control, "mcpwm_example_servo_control", 4096, NULL, 2, NULL);
    //xTaskCreate(test, "driving", 4096, NULL, 1, NULL);

    //xTaskCreate(calibrateESC, "ESC Calibration", 4096, NULL, 5, NULL);
}
