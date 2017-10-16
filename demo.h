/** @file demo.c
    @author Ollie Chick and Leo Lloyd
    @brief Demo module - used to display information before the game starts.
*/

#ifndef DEMO_H
#define DEMO_H
 
#include <stdbool.h>

#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "tinygl.h"
#include "button.h"
#include "../fonts/font5x7_1.h"

/* Pacer rate in Hz. */
#define PACER_RATE 500
/* Time in seconds before arrow is displayed. */
#define TIME_BEFORE_ARROW 14

/** Plays the demo and returns when the button is pressed. */
void play_demo (void);

#endif
