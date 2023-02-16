//tocando la cucaracha con buzzer
//dado que con pwm podemos generar ondas cuadradas con una cierta frecuencia, si escogemos las frecuencias de las notas musicales, entonces podemos tocar
//melodias simples
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_HIGH_SPEED_MODE
#define LEDC_OUTPUT_IO (13) // Define the output GPIO
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY (4096)                // Set duty to 50%. ((2 ** 13) - 1) * 50% = 4095
#define Do3 261
#define Re3 293
#define Mi3 329
#define Fa3 349
#define Sol3 391
#define La3 440
#define Si3 493
#define Sib3 466
#define Do4 523
#define Re4 587

void delay_ms(int ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}
static void init_pwm(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = 440, // Set output frequency at 5 kHz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LEDC_OUTPUT_IO,
        .duty = 0, // Set duty to 0%
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}
void update_duty(int duty)
{
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}
void app_main(void)
{
    init_pwm();
    int notes[] = {Do3, Do3, Do3, Fa3, La3,
                   Do3, Do3, Do3, Fa3, La3,
                   Fa3, Fa3, Mi3, Mi3, Re3, Re3, Do3,
                   Do3, Do3, Do3, Mi3, Sol3,
                   Do3, Do3, Do3, Mi3, Sol3,
                   Do4, Re4, Do4, Sib3, La3, Sol3, Fa3};
    int size = sizeof(notes) / sizeof(int);
    for (int i = 0; i < size; i++)
    {
        update_duty(LEDC_DUTY);
        ledc_set_freq(LEDC_MODE, LEDC_TIMER, notes[i]);
        delay_ms(250);
        update_duty(0);
        delay_ms(50);
    }
}
