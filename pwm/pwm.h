#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

uint calculate_level(uint clock);
void pwm_setup(uint gpio_num, uint frequency);
void pwm_loop();