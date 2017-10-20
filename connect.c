/** @file connect.c
    @author Ollie Chick (och26)
    @brief Connects to another funkit. Differentiates between sender and receiver.
*/

#include <stdbool.h>

#include "ir_uart.h"
#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"

#define MILLISECS_IN_A_SEC 1000
/* Pacer rate in Hz. */
#define PACER_RATE 500
/* Wait time in milliseconds to try and get a sending signal. */
#define DELAY 5000
/* Pause time in milliseconds before reading IR signal. */
#define PAUSE_TIME 1 //CANDIDATE FOR REMOVAL
/* Character to send to signify sending. */
#define SENDING 's'
/* Character to send to signify acknowledgement. */
#define ACK 'a'

/* Checks if the other kit is sending.
   If it is, send an acknowledgement.
   @return true if the other kit is sending and it send an acknowledgement,
       false if it timed out without getting a signal.
 */
static bool check_if_sending(void) {
    
    tinygl_draw_char('R', tinygl_point(0, 0));
    
    /* Get how many paces it should do, based on SEND_FREQUENCY.
       Note: +1 is to account for any discarded remainder when dividing.
       Limited to 2^16 = 65k paces. */
    uint16_t total_paces = DELAY/(MILLISECS_IN_A_SEC/PACER_RATE) + 1;
    
    /* Try to receive a signal from other kit */
    uint16_t i;
    for (i = 0; i < total_paces; i++)
    {
        pacer_wait();
        tinygl_update();
        
        if (ir_uart_read_ready_p())
        {
            if (ir_uart_getc() == SENDING) {
                /* Other kit was already sending. */
                pacer_wait();
                ir_uart_putc(ACK); //send acknowledgement
                tinygl_clear();
                tinygl_update();
                return true;
            }
        }
    }
    
    /* Waited long enough with no signal. */
    tinygl_clear();
    tinygl_update();
    return false;
}


/* Sends to the other kit until it gets an acknowledgement signal back.
   @return true if the other kit was sending and it send an acknowledgement,
       false if the other kit sent an acknowledgement.
  */
static bool send(void) {
    
    tinygl_draw_char('S', tinygl_point(0, 0));
    
    uint8_t ticker = 1; // initialised to 1 to avoid sending immediately
    
    /* Sending loop */
    while (1)
    {
        pacer_wait();
        tinygl_update();
        
        /* Try and get a response */
        if (ir_uart_read_ready_p())
        {
            char received_char = ir_uart_getc();
            if (received_char == ACK) {
                /* They have accepted the signal. */
                tinygl_clear();
                tinygl_update();
                return false;
            } else if (received_char == SENDING) {
                /* They are trying to send too - let them. */
                check_if_sending();
                return true;
            }
        }
        
        /* Send every 2^8 = 256 loops (to ensure ledmat doesn't flicker) */
        if (ticker == 0) {
            ir_uart_putc(SENDING);
        }
        
        ticker++;
    }
}


/* Connects to another funkit.
   Waits for the delay (ms) first to receive anything.
   If nothing is received, it sends until it gets an acknowledgement.
   @return true if they received a signal immediately,
       false if they didn't receive a signal initially but later got an ack
 */
bool connect(void) {
    //return false; //DEBUG auto-"connect" - we are player 1
    
    pacer_init(PACER_RATE);
    
    if (check_if_sending()) {
        /* Received a signal. */
        return true;
    } else if (send()){
        /* Didn't initially receive a signal, but later did. */
        return true;
    } else {
        /* Didn't initially receive a signal, and later received an acknowledgement. */
        return false;
    }
}
