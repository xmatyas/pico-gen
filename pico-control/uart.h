#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#define UART_ID     uart0
#define BAUD_RATE   115200
#define DATA_BITS   8
#define STOP_BITS   1
#define PARITY      UART_PARITY_NONE

#define UART_TX_PIN 0
#define UART_RX_PIN 1

typedef struct uart_data{
    uart_inst_t *id;
    uint baud_rate;
    uint data_bits;
    uint stop_bits;
    uart_parity_t parity;
}uart_data;

typedef struct uart_pins{ 
    uint tx_pin;
    uint rx_pin;
}uart_pins;

int duty_to_pwm_level(int duty, uint count_top);
void on_uart_rx();
void uart_setup(uart_data info, uart_pins pins);
void uart_irq_setup(uart_data info);
int main();
