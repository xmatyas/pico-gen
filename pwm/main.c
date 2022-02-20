// DEFAULT C LIBRARIES
#include <stdio.h>
// PICO LIBRARIES/SDK
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
// OWN LIBRARIES
#include "adc.h"
#include "pwm.h"
#include "uart.h"

#define PWM_PIN 0
#define ADC_PIN 26

int main(){

    stdio_init_all();

    //LED INDICATION
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    //PWM SETUP
    const uint freq = 25000;
    pwm_setup(PWM_PIN,freq);
    const uint slice = pwm_gpio_to_slice_num(PWM_PIN);
    const uint count_top = calculate_level(freq);
    uint16_t fan_rpm = 200;
    char fan_rpm_str[17];
    sprintf(fan_rpm_str,"%hu",fan_rpm);
    //ADC SETUP
    adc_setup(ADC_PIN);
    //UART SETUP
    uart_data uart_data_curr = {UART_ID, BAUD_RATE, DATA_BITS, STOP_BITS, PARITY};
    uart_pins uart_pins_curr = {UART_TX_PIN, UART_RX_PIN};
    uart_setup(uart_data_curr, uart_pins_curr);
    //uart_irq_setup(uart_data_curr);
    char inp_c;
    uint16_t rpm = 0;
    ///////////////
    while(1){
        //uart_puts(UART_ID, fan_rpm_str);
        if(uart_is_readable(UART_ID)){
            inp_c = uart_getc(UART_ID);        //BLOCKED AT THIS STAGE
            printf("Char %c and Int %d and CharToInt %d\n",inp_c,inp_c, inp_c -'0');
            gpio_put(LED_PIN, 1);
            if(uart_is_writable(UART_ID)){
                uart_putc_raw(UART_ID, inp_c);
            }
        }
        
        
        
        /*
        //ADC PART
        const float conversion_factor = 3.3f / (1 << 12);
        uint16_t result = adc_read();
        //printf("Measured raw value : 0x%03x, Voltage : %fV \n", result, result*conversion_factor);
        printf("Voltage : %fV \tLevel : %d\n",result*conversion_factor,adc_to_pwm_level(result, count_top));
        pwm_set_chan_level(slice, PWM_CHAN_A, adc_to_pwm_level(result, count_top));
        //printf("Conversion_rate : %f, Level : %f \n",conversion_rate,result*conversion_rate)
        */
    }
}