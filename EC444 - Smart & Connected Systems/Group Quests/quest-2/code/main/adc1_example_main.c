// QUEST 2: Sensor Central
// TEAM 17: Karle Erf, Thuc Nguyen, Alex Salmi

// Main code for our sensor circuit/data collection. Gets the data from the sensors, converts the data to scientific units, and then print the data to the serial monitor to be picjed up by the nodejs server

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "driver/adc.h"

#include "esp_adc_cal.h"

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   10          //Multisampling

static esp_adc_cal_characteristics_t *adc_chars_therm;
static esp_adc_cal_characteristics_t *adc_chars_US;
static esp_adc_cal_characteristics_t *adc_chars_IR;
static const adc_channel_t channel_therm = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_channel_t channel_US = ADC_CHANNEL_4;        //GPIO32 if ADC1
static const adc_channel_t channel_IR = ADC_CHANNEL_5;        //GPIO33 if ADC1
static const adc_atten_t atten_therm = ADC_ATTEN_DB_11;         //atten for thermistor and IR
static const adc_atten_t atten_US = ADC_ATTEN_DB_0;           //atten for ultrasonic sensor
//static const adc_atten_t atten_IR = ADC_ATTEN_DB_11;

static const adc_unit_t unit = ADC_UNIT_1;



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

void app_main(void)
{
    //Check if Two Point or Vref are burned into eFuse
    check_efuse();

    //Configure ADC for therm
    if (unit == ADC_UNIT_1) {
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(channel_therm, atten_therm);
        adc1_config_channel_atten(channel_US, atten_US);
        adc1_config_channel_atten(channel_IR, atten_therm);

    } else {
        adc2_config_channel_atten((adc2_channel_t)channel_therm, atten_therm);
        adc2_config_channel_atten((adc2_channel_t)channel_US, atten_US);
        adc2_config_channel_atten((adc2_channel_t)channel_IR, atten_therm);
    }

    //Characterize ADC
    adc_chars_therm = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    adc_chars_US = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    adc_chars_IR = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type_therm = esp_adc_cal_characterize(unit, atten_therm, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars_therm);
    esp_adc_cal_value_t val_type_US = esp_adc_cal_characterize(unit, atten_US, ADC_WIDTH_BIT_10, DEFAULT_VREF, adc_chars_US);
    esp_adc_cal_value_t val_type_IR = esp_adc_cal_characterize(unit, atten_therm, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars_IR);
    print_char_val_type(val_type_therm);
    print_char_val_type(val_type_US);
    print_char_val_type(val_type_IR);

    //Continuously sample ADC1
    while (1) {
        uint32_t adc_reading_therm = 0;
        uint32_t adc_reading_US = 0;
        uint32_t adc_reading_IR = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            if (unit == ADC_UNIT_1) {
                adc_reading_therm += adc1_get_raw((adc1_channel_t)channel_therm);
                adc_reading_US += adc1_get_raw((adc1_channel_t)channel_US);
                adc_reading_IR += adc1_get_raw((adc1_channel_t)channel_IR);
            } else {
                int raw_therm;
                int raw_US;
                int raw_IR;
                adc2_get_raw((adc2_channel_t)channel_therm, ADC_WIDTH_BIT_12, &raw_therm);
                adc2_get_raw((adc2_channel_t)channel_US, ADC_WIDTH_BIT_10, &raw_US);
                adc2_get_raw((adc2_channel_t)channel_IR, ADC_WIDTH_BIT_12, &raw_IR);
                adc_reading_therm += raw_therm;
                adc_reading_US += raw_US;
                adc_reading_IR += raw_IR;
            }
        }
        adc_reading_therm /= NO_OF_SAMPLES;
        adc_reading_US /= NO_OF_SAMPLES;
        adc_reading_IR /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV
        uint32_t voltage_therm = esp_adc_cal_raw_to_voltage(adc_reading_therm, adc_chars_therm);
        uint32_t voltage_US = esp_adc_cal_raw_to_voltage(adc_reading_US, adc_chars_US);
        uint32_t voltage_IR = esp_adc_cal_raw_to_voltage(adc_reading_IR, adc_chars_IR);
        //printf("Raw: %d\tVoltage: %dmV\n", adc_reading_US, voltage_US);
        double volt = (double)voltage_therm;
        double tmp = (1/((1/298.15) + (1/3435.0) * log((5000/volt) - 1))) - 276.15;
        int temp = (int)tmp;


        int dist_US = adc_reading_US/4;


        double slope = (-12/151.0);
        double b = 160.0;
        int dist_IR = slope*voltage_IR + b;
        printf("%d|", temp);
        printf("%d|", dist_US);
        printf("%d\n", dist_IR);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}


