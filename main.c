#include "lcd.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "inttypes.h"

// pin declarations
const uint ADC_PIN = 26;

const uint DIR_PIN = 16;
const uint STEP_PIN = 18;
const uint steps_per_revolution = 200;

const uint BREAKBEAM_PIN = 22;

// breakbeam counter variable
volatile uint32_t item_count = 0;
bool data_to_show = false;

// breakbeam debounce 
unsigned long time = 0;
const int delayTime = 300;

// breakbeam interrupt
void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BREAKBEAM_PIN)
    {
        if ((to_ms_since_boot(get_absolute_time())-time)>delayTime) {
            time = to_ms_since_boot(get_absolute_time());
            item_count++;
            data_to_show = true;
        }
    }
}

bool motor_speed_update(struct repeating_timer *t) {
    uint32_t result = adc_read();
    uint slice_num = pwm_gpio_to_slice_num(STEP_PIN);
    result += 2000;
    pwm_set_wrap(slice_num, result);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, result/2);
    return true;
}

int main() {
//  prepare ADC
    stdio_init_all();
    adc_init();
    
    gpio_set_function(ADC_PIN, GPIO_FUNC_SIO);
    gpio_disable_pulls(ADC_PIN);
    gpio_set_input_enabled(ADC_PIN, false);

//  prepare LCD display
    lcd_init();
    char disp_buf[16] = {};
    data_to_show = true;
    item_count = 0;

//  prepare stepper motor
    gpio_init(DIR_PIN);
    gpio_set_dir(DIR_PIN, GPIO_OUT);
    gpio_put(DIR_PIN, 0); // Set motor direction counter-clockwise

    gpio_init(STEP_PIN);
    gpio_set_dir(STEP_PIN, GPIO_OUT);
    gpio_set_function(STEP_PIN, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(STEP_PIN);
    pwm_set_clkdiv(slice_num, 32.0f);
    pwm_set_wrap(slice_num, 6000);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 6000/2);
    pwm_set_enabled(slice_num, true);

    struct repeating_timer timer;
    add_repeating_timer_ms(500, motor_speed_update, NULL, &timer);

//  breakbeam
    gpio_init(BREAKBEAM_PIN);
    gpio_set_dir(BREAKBEAM_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(BREAKBEAM_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    while (1)
    {
        if(data_to_show)
        {
            lcd_clear();
            lcd_string(disp_buf);
            sprintf(disp_buf, "%u sztuk", item_count);
            data_to_show = false;
        }
    }
}

