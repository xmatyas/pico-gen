#include "uart.h"
#define CONF_NUM 0xFF

// FIX THIS FUNCTION
void on_uart_rx(){
    if(uart_is_readable(UART_ID)) {
        char incoming = uart_getc(UART_ID);
        if (uart_is_writable(UART_ID)) {
            //uart_putc(UART_ID, CONF_NUM);
        }
    }
}
void uart_setup(uart_data info, uart_pins pins){
    uart_init(info.id, info.baud_rate);

    gpio_set_function(pins.tx_pin, GPIO_FUNC_UART);
    gpio_set_function(pins.rx_pin, GPIO_FUNC_UART);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(info.id, false, false);

    // Set our data format
    uart_set_format(info.id, info.data_bits, info.stop_bits, info.parity);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(info.id, false);
}
void uart_irq_setup(uart_data info){
    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = info.id == uart0 ? UART0_IRQ : UART1_IRQ;
    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);
}
/*
int main(){

    uart_setup(uart_data_default, uart_pins_default);
    uart_irq_setup(uart_data_default);
    // OK, all set up.
    // Lets send a basic string out, and then run a loop and wait for RX interrupts
    // The handler will count them, but also reflect the incoming data back with a slight change!
    uart_puts(UART_ID, "\nHello, uart interrupts\n");

    while (1)
        tight_loop_contents();
}
*/