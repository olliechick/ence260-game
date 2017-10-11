/**
 * @file connect.c
 * @author Ollie Chick
 * @date 10 October 2017
 * @brief Connects to another funkit
 */

#include "ir_uart.h"
#include <stdbool.h>
#include "led.h" //LED FOR DEBUG

#include "system.h"
#include "display.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"

#define SEND_FREQUENCY 500 //Hz
#define MILLISECS_IN_A_SEC 1000
#define DELAY 5000 // wait time in milliseconds to get signal

/**
 * Checks if the other kit is sending
 * @return true if it gets a signal from the other kit
 */
static bool check_if_sending(void) {
    
    tinygl_text("sending");
    
    // Get how many paces it should do, based on SEND_FREQUENCY
    // Note: +1 is to account for any discarded remainder when dividing
    // Limited to 2^16 = 65k paces
    uint16_t total_paces = DELAY/(MILLISECS_IN_A_SEC/SEND_FREQUENCY) + 1;
    
    // Try to receive a signal from other kit
    uint16_t i;
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
                return true;
            }
        }
        tinygl_update ();
    }
    display_clear();
    // Waited long enough with no signal
    return false;
}

/** Sends to the other kit until it gets an acknowledgement signal back */
static void send(void) {
    
    tinygl_text("receiving");
    
    uint8_t ticker = 0;
    
    // We've waited long enough, lets try to send
    while (1)
    {
        pacer_wait();
        tinygl_update ();
        
        //roughly twice a second, send and check for ack
        if (ticker == 0) {
            ir_uart_putc('s'); //send 
            
            //Try and get a response
            if (ir_uart_read_ready_p ())
            {
                char character = ir_uart_getc ();
                if (character == 'a') {
                    //They have accepted the signal
                    return;
                }
            }
            ticker ++;
        }
    }
}

/**
 * Connects to another funkit.
 * Waits for the delay (ms) first to receive anything.
 * If nothing is received, it sends until it gets an acknowledgement.
 * @return true if they received a signal immediately,
 *  false if they didn't receive a signal initially but later got an ack
 *      
 */
bool connect() {
    
    // Initialisation
    ir_uart_init ();
    pacer_init(SEND_FREQUENCY); //Initialize pacer to SEND_FREQUENCY
    
    if (check_if_sending()) {
        // Received a signal
        return true;
    } else {
        led_set (LED1, 1); //LED FOR DEBUG
        send(); //keep sending until they send back an ack
        return false;
    }
}
