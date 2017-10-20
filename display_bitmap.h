/** @file display_bitmap.c
    @author Ollie Chick (och26) and Leo Lloyd (lll28)
    @brief A module for setting the display to a bitmap.
*/

#ifndef DISPLAY_BITMAP_H
#define DISPLAY_BITMAP_H

#include "system.h"

/* Pacer rate in Hz. */
#define PACER_RATE 500

/* Sets the display (cell by cell) given a bitmap. */
void set_display(const uint8_t bitmap[]);

#endif
