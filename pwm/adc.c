#include "adc.h"

int adc_to_pwm_level(uint16_t result, uint count_top)
{
    int level = 0;
    const uint8_t border = 50;
    // TODO: Fix this static conversion rate.
    const float conversion_rate = 1.221;
    level = result * conversion_rate;
    return (level <= border) ? 0 : (level >= (count_top-border)) ? count_top : level;
}

void adc_setup(uint gpio_num){
    adc_init();
    adc_gpio_init(gpio_num);
    adc_select_input(0 + (gpio_num - 26));  //0...3 are GPIOs 26...29 respectively. Input 4 is the onboard temperature sensor.
}