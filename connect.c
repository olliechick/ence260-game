/**
 * @file connect.c
 * @author Ollie Chick
 * @date 10 October 2017
 * @brief Connects to another funkit
 */

#include "ir_uart.h"
#include "pacer.h"
#include "led.h" //LED FOR DEBUG

#define SEND_FREQUENCY 100 //Hz
#define MILLISECS_IN_A_SEC 1000
       
/**
 * Connects to another funkit.
 * Waits for the delay (ms) first to receive anything.
 * If nothing is received, it sends until it gets an acknowledgement.
 * @param delay how long in ms it waits. -1 flag means forever.
 * @return 0 if they didn't receive something initially but later got an ack,
 *      1 if they received immediately
 */
int connect(int delay) {
    
    // Initialisation
    ir_uart_init ();
    pacer_init(SEND_FREQUENCY); //Initialize pacer to SEND_FREQUENCY
    led_init(); //Initialize led LED FOR DEBUG
    led_set (LED1, 0); //turn off led LED FOR DEBUG
    
    // Get how many paces it should do, based on SEND_FREQUENCY
    // Note: +1 is to account for any discarded remainder when dividing
    uint8_t total_paces = delay/(MILLISECS_IN_A_SEC/SEND_FREQUENCY) + 1;
    
    // Try to receive a signal from other kit
    uint8_t i;
    for (i = 0; i < total_paces; i++)
    {
        pacer_wait();
        
        if (ir_uart_read_ready_p ())
        {
            char ch;
            ch = ir_uart_getc ();
            if (ch == 's') {
                // other kit was already sending
                ir_uart_putc('a'); //send acknowledgement
                return 1;
            }
        }
    }
    
    // We've waited long enough, lets try to send
    led_set (LED1, 1); //LED FOR DEBUG
    while (1)
    {
        pacer_wait();
        ir_uart_putc('s'); //send 
        
        //Try and get a response
        if (ir_uart_read_ready_p ())
        {
            char character = ir_uart_getc ();
            if (character == 'a') {
                //They have accepted the signal
                return 0;
            }
        }
        
    }
    
    
    
}
