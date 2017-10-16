/**
 * @file connect.c
 * @author Ollie Chick
 * @date 10 October 2017
 * @brief Connects to another funkit
 */

#include <stdbool.h>

#include "ir_uart.h"
#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"

#define SEND_FREQUENCY 500 //Hz
#define MILLISECS_IN_A_SEC 1000
#define DELAY 5000 // wait time in milliseconds to get signal
#define PAUSE_TIME 1 //ms pause time before reading IR signal

/**
 * Checks if the other kit is sending
 * @return true if it gets a signal 's' from the other kit
 */
static bool check_if_sending(void) {
    
    tinygl_draw_char('R', tinygl_point(0, 0));
    
    // Get how many paces it should do, based on SEND_FREQUENCY
    // Note: +1 is to account for any discarded remainder when dividing
    // Limited to 2^16 = 65k paces
    uint16_t total_paces = DELAY/(MILLISECS_IN_A_SEC/SEND_FREQUENCY) + 1;
    
    // Try to receive a signal from other kit
    uint16_t i;
    for (i = 0; i < total_paces; i++)
    {
        pacer_wait();
        tinygl_update();
        
        if (ir_uart_read_ready_p())
        {
            if (ir_uart_getc() == 's') {
                // other kit was already sending
                pacer_wait(); // wait one pace
                ir_uart_putc('a'); //send acknowledgement
                tinygl_clear();
                tinygl_update();
                return true;
            }
        }
    }
    
    // Waited long enough with no signal
    tinygl_clear();
    tinygl_update();
    return false;
}

/**
 * Sends to the other kit until it gets an acknowledgement signal back
 * @return true if it gets a signal 's' from the other kit
  */
static bool send(void) {
    
    tinygl_draw_char('S', tinygl_point(0, 0));
    
    uint8_t ticker = 1;
    
    // Sending loop
    while (1)
    {
        pacer_wait();
        tinygl_update();
        
        //Try and get a response
        if (ir_uart_read_ready_p())
        {
            char received_char = ir_uart_getc();
            if (received_char == 'a') {
                //They have accepted the signal
                tinygl_clear();
                tinygl_update();
                return false;
            } else if (received_char == 's') {
                //they are trying to send too
                check_if_sending();
                return true;
            }
        }
        
        //Every 2^8 = 256 loops (to ensure ledmat doesn't flicker):
        if (ticker == 0) {
            ir_uart_putc('s'); //send 
        }
        ticker++;
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
bool connect(void) {
    //return false; //DEBUG auto-"connect" - we are player 1
    
    pacer_init(SEND_FREQUENCY); //Initialize pacer to SEND_FREQUENCY
    
    if (check_if_sending()) {
        // Received a signal
        return true;
    } else if (send()){ /*
        // No signal received
        send(); //keep sending until they send back an ack*/
        return true;
    } else {
        return false;
    }
}
