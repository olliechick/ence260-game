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
    
    total_paces = delay/100;
    if (total_paces = 0) {
        total_paces = 1;
    }
    
    pacer_init(10)
    
    while (1)
    {
        pacer_wait();
        
        if (ir_uart_read_ready_p ())
        {
            char ch;
            
            ch = ir_uart_getc ();
            
            if (ch == 's') {
                // other kit was already sending
                return 1;
            }
        }
    }
}
