/** @file display_bitmap.c
    @author Ollie Chick & Leo Lloyd
    @brief A module for setting the display to a bitmap.
*/

#include "system.h"
#include "pio.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"

/* Pacer rate in Hz. */
#define PACER_RATE 500

/* Sets the display (cell by cell) given a bitmap. */
void set_display(const uint8_t bitmap[])
{
    int i;
    int j;
    for (i = 0; i < DISPLAY_WIDTH; i++) {
        for (j = 0; j < DISPLAY_HEIGHT; j++) {
            bool val = bitmap[i] & (1 << j);
            tinygl_draw_point(tinygl_point(i, j), val);
        }
    }
    return;
}

