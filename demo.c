/** @file demo.c
    @author Ollie Chick and Leo Lloyd
    @brief Demo module - used to display information before the game starts.
*/
 
#include <stdbool.h>

#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "display_bitmap.h"

#include "tinygl.h"
#include "button.h"
#include "../fonts/font5x7_1.h"

/* Time in seconds before arrow is displayed. */
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
void play_demo (void)
{
    pacer_init (PACER_RATE);
    tinygl_text("Tic-tac-toe! ");
    
    uint16_t ticks_before_scene_change = (PACER_RATE * TIME_BEFORE_ARROW);
    uint16_t ticker = 0;
    
    while (1) {
        pacer_wait();
        
        button_update(); //poll button
        tinygl_update(); //refresh screen
        
        /* If button is pushed, return (skip the demo). */
        if(button_push_event_p (BUTTON1))
        {
            tinygl_clear();
            tinygl_update();
			return;
		}
        
        /* After the preset time, display the arrow. */
        if (ticker == ticks_before_scene_change) {
            tinygl_clear();
            set_display(arrow);
        }
        
        ticker++;
        
    }
}
