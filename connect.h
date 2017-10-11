/**
 * @file connect.c
 * @author Ollie Chick
 * @date 10 October 2017
 * @brief Connects to another funkit
 */
 
#ifndef CONNECT_H
#define CONNECT_H

#include "ir_uart.h"
#include "pacer.h"
#include <stdbool.h>
#include "led.h" //LED FOR DEBUG

#define SEND_FREQUENCY 100 //Hz
#define MILLISECS_IN_A_SEC 1000
#define DELAY 2600 // wait time in milliseconds to get signal
   
/**
 * Connects to another funkit.
 * Waits for the delay (ms) first to receive anything.
 * If nothing is received, it sends until it gets an acknowledgement.
 * @return true if they received a signal immediately,
 *  false if they didn't receive a signal initially but later got an ack
 *      
 */
bool connect();


#endif
