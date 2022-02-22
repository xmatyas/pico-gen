// DEFAULT C LIBRARIES
#include <stdio.h>
#include <stdlib.h>
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

typedef struct PicoPacket{
    uint8_t size;
    char type;
    int data;
} PicoPacket;

int int_pow(int base, int exponent){
    int result = 1;
    for(exponent; exponent>0; exponent--){
        result = result * base;
    }
    return result;
}

int data_parse(uint8_t data[], int size){
    int result = 0;
    for(int i = size; i >= 0; i--){
        result += data[i]*int_pow(10,i);
    }
    return result;
}

int validate_packet(PicoPacket rx_packet){
    if (rx_packet.type == 'r')
    {
        return 1;
    }
    else if(rx_packet.type == 's')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

PicoPacket uart_get_packet(uart_inst_t *uart){
    if(uart_is_readable(uart)){
        PicoPacket rx_packet;
        rx_packet.size = (uint8_t)uart_getc(uart);
        rx_packet.type = uart_getc(uart);
        uint8_t temp_a[rx_packet.size];
        for (size_t i = rx_packet.size - 1; i >= 0; i--)
        {
            temp_a[i] = (uint8_t)uart_getc(uart);
        }
        rx_packet.data = data_parse(temp_a, rx_packet.size);
        if(!validate_packet(rx_packet)){
            exit(0);
        }
        return rx_packet;
    }
    else{
        exit(0); //Fix return value for faulty uart read
    }
}




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
        PicoPacket rx_packet = uart_get_packet(UART_ID);
        printf("Uart package received");
        char helper_buffer[100];
        int cx = snprintf(helper_buffer, 100, "Size : %d, Type : %c, Data : %d",rx_packet.size, rx_packet.type, rx_packet.data);
        if(cx>=0 && cx<100){
            uart_puts(UART_ID, helper_buffer);
        }
        /*
        if(uart_is_readable(UART_ID)){
            inp_c = uart_getc(UART_ID);        //BLOCKED AT THIS STAGE
            printf("Char %c and Int %d and CharToInt %d\n",inp_c,inp_c, inp_c -'0');
            gpio_put(LED_PIN, 1);
            if(uart_is_writable(UART_ID)){
                uart_putc_raw(UART_ID, inp_c);
            }
        }
        
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