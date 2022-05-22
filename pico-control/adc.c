#include "adc.h"

#define ADC_RESOLUTION 12   //12 bit resolution

int adc_to_pwm_level(uint16_t result, uint count_top)
{
    int level = 0;
    //1% border
    const uint16_t border = count_top*0.01;
    //TOP/Resolution of ADC
    const float conversion_rate = count_top/(1 << ADC_RESOLUTION);
    level = result * conversion_rate;
    return (level <= border) ? 0 : (level >= (count_top-border)) ? count_top : level;
}

void adc_setup(uint gpio_num){
    adc_init();
    adc_gpio_init(gpio_num);
    adc_select_input(0 + (gpio_num - 26));  //0...3 are GPIOs 26...29 respectively. Input 4 is the onboard temperature sensor.
}