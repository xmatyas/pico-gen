#include "pwm.h"

/*
    RECAP
    Default System Clock = 125MHz also as 8ns
    We need 25kHz for fan PWM.
    25kHz(40 µs) can be achieved by dividing 40 µs(period)/8 ns (period) = 5000 cycles
    We use the cycles as a TOP
    Level = TOP * Duty
    2500 = 5000 * 0.5(50%)
*/

uint calculate_level(uint clock){
    const uint system_clock = 125000000;
    return system_clock/clock;
}

void pwm_setup(uint gpio_num, uint frequency){
    gpio_set_function(gpio_num, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(gpio_num);
    pwm_set_enabled(slice, true);
    pwm_set_wrap(slice, calculate_level(frequency));
}

void pwm_loop(const uint frequency, uint gpio_num){

    //stdio_init_all();
    pwm_setup(gpio_num, frequency);

    const uint count_top = calculate_level(frequency);
    short step = 0; //5% steps
    bool going_up = true;

    while(1){
        if(going_up){
                step = step + 250;
                if(step >= count_top){
                    going_up = false;
                    step = step - 250;  //to prevent 5250
                }
            }
            else{
                step = step - 250;
                if(step < 0){
                    going_up = true;
                    step = 0;
                }
            }
            printf("Step num : %d | Step percentage : %0.f\n",step,((float)step/(float)count_top)*100.0);
            pwm_set_chan_level(pwm_gpio_to_slice_num(gpio_num), PWM_CHAN_A, adc_to_pwm_level(step));
    }
}