/** @file display_bitmap.c
    @author Ollie Chick & Leo Lloyd
    @brief A module for setting the display to a bitmap.
*/

#ifndef DISPLAY_BITMAP_H
#define DISPLAY_BITMAP_H

#include "system.h"
#include "pio.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"

/* Pacer rate in Hz. */
#define PACER_RATE 500

/* Sets the display (cell by cell) given a bitmap. */
void set_display(const uint8_t bitmap[]);

#endif
