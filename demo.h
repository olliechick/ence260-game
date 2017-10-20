/** @file demo.c
    @author Ollie Chick (och26) and Leo Lloyd (lll28)
    @brief Demo module - used to display information before the game starts.
*/

#ifndef DEMO_H
#define DEMO_H

#include "system.h"

/* Time in seconds before the arrow is displayed. */
#define TIME_BEFORE_ARROW 15

/* Bitmap of the arrow pointing to the button. */
static const uint8_t arrow[] =
{
    4, 14, 21, 4, 4
    // |631    |
    // |4268421|
    // |....@..|
    // |...@@@.|
    // |..@.@.@|
    // |....@..|
    // |....@..|
};

/** Plays the demo and returns when the button is pressed. */
void play_demo (void);

#endif
