#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

int adc_to_pwm_level(uint16_t result, uint count_top);
void adc_setup(uint gpio_num);