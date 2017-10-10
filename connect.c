/**
 * @file connect.c
 * @author Ollie Chick
 * @date 10 October 2017
 * @brief Connects to another funkit
 */

#include "ir_uart.h"
#include "pacer.h"
       
/**
 * Connects to another funkit.
 * Waits for the delay (ms) first to receive anything.
 * If nothing is received, it sends until it gets an acknowledgement.
 * @param delay how long in ms it waits. -1 flag means forever.
 * @return 0 if they didn't receive something initially but later got an ack,
 *      1 if they received immediately
 */
int connect(int delay) {
    
    ir_uart_init ();
    pacer_init(10); //Initialize pacer to 10Hz
    
    // Get how many paces it should do, based on a freq of 10Hz
    // +1 to account for any discarded remainder when dividing by 100
    uint8_t total_paces = delay/100 + 1;
    if (total_paces == 0) {
        total_paces = 1;
    }
    
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
    
    while (1)
    {
        pacer_wait();
        ir_uart_putc('s'); //send 
        
        //Try and get a response
        if (ir_uart_read_ready_p ())
        {
            char character = ir_uart_getc ();
            if (character == 'a') {
                //They have accepted they are the second player
                return 0;
            }
        }
        
    }
    
    
    
}
