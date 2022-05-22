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

#define PWM_PIN 22
#define ADC_PIN 26
#define SWITCH_PIN 21

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
int data_parse(uint8_t data[], uint8_t size){
    int result = 0;
    for(uint8_t i = size-1, exp = 0; i >= 0 && exp <= size; i--,exp++){
        result += data[i]*int_pow(10,exp);
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
        uint8_t size = uart_getc(uart);
        char type = uart_getc(uart);
        uint8_t temp_a[size];
        printf("Size : %d, Type : %c\n",size, type);
        for (int i = 0; i < size; i++)
        {
            temp_a[i] = (uint8_t)uart_getc(uart);
            printf("temp_a[%d] = %d\n",i,temp_a[i]);
        }
        int data = data_parse(temp_a, size);
        printf("Data : %d\n",data);
        PicoPacket rx_packet = {size, type, data};
        if(validate_packet(rx_packet) == 0){
            exit(0);
        }
        return rx_packet;
    }
    else{
        exit(0);
    }
}

int main(){

    stdio_init_all();

    //LED INDICATION
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    //PWM SETUP
    //Frequency in Hz
    const uint freq = 25000;
    pwm_setup(PWM_PIN,freq);
    //Slice through which we control the PWM
    const uint slice = pwm_gpio_to_slice_num(PWM_PIN);
    const uint count_top = calculate_level(freq);

    uint16_t fan_duty = 0;

    //ADC SETUP
    adc_setup(ADC_PIN);
    //UART SETUP
    uart_data uart_data_curr = {UART_ID, BAUD_RATE, DATA_BITS, STOP_BITS, PARITY};
    uart_pins uart_pins_curr = {UART_TX_PIN, UART_RX_PIN};
    uart_setup(uart_data_curr, uart_pins_curr);
    //uart_irq_setup(uart_data_curr);
    char inp_c;
    inp_c = uart_getc(UART_ID);

    bool switch_button;
    gpio_init(SWITCH_PIN);
    gpio_set_dir(SWITCH_PIN, GPIO_IN);
    gpio_pull_up(SWITCH_PIN);
    ///////////////
    while(1){
        PicoPacket rx_packet;     
        
        if(uart_is_readable(UART_ID)){
            rx_packet = uart_get_packet(UART_ID);
            if(rx_packet.type == 'r'){
                printf("Size : %d, Type : %c, Data : %d\n",rx_packet.size, rx_packet.type, rx_packet.data);
                pwm_set_chan_level(slice, PWM_CHAN_A, duty_to_pwm_level(rx_packet.data, count_top));
                sleep_ms(1000);
            }
        }
   
        //ADC PART
        const float conversion_factor = 3.3f / (1 << 12);
        uint16_t result = adc_read();
        printf("Result: %d\tVoltage : %fV \tLevel : %d\n", result, result*conversion_factor, adc_to_pwm_level(result, count_top));
        //ADC SET LEVEL
        pwm_set_chan_level(slice, PWM_CHAN_A, adc_to_pwm_level(result, count_top));
        sleep_ms(1000);\

        //BUTTON PART
        if(!gpio_get(SWITCH_PIN)) {
            printf("Button value if: %d\n", gpio_get(SWITCH_PIN));
        }else{
            printf("Button value else : %d\n", gpio_get(SWITCH_PIN));
        }
    }
}