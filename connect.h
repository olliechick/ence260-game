/** @file connect.c
    @author Ollie Chick
    @brief Connects to another funkit. Differentiates between sender and receiver.
*/

#ifndef CONNECT_H
#define CONNECT_H

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

/* Connects to another funkit.
   Waits for the delay (ms) first to receive anything.
   If nothing is received, it sends until it gets an acknowledgement.
   @return true if they received a signal immediately,
       false if they didn't receive a signal initially but later got an ack
 */
bool connect(void);

#endif
