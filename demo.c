/**
 * @file demo.c
 * @author Ollie Chick
 * @date 10 October 2017
 * @brief Demo module
 */
 
#include <stdbool.h>

#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "tinygl.h"
#include "button.h"

#include "../fonts/font5x7_1.h"

#define PACER_RATE 500 //Hz
#define MILLISECS_IN_A_SEC 1000
#define TIME_BETWEEN_FRAMES 2000 //milliseconds between each frame in demo


/** Bitmap of the arrow pointing to the button. */
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

// Empty screen bitmap (see arrow for orientation, it points towards button):
    //0, 0, 0, 0, 0
    // |631    |
    // |4268421|
    // |.......|
    // |.......|
    // |.......|
    // |.......|
    // |.......|

/**
static const uint8_t scene0[] = 
{
    72, 73, 120, 73, 73
    // |631    |
    // |4268421|
    // |@..@...|
    // |@..@..@|
    // |@@@@...|
    // |@..@..@|
    // |@..@..@|
};
**/

/** 
 * Sets the display (cell by cell) given a bitmap.
 * 
 */
static void set_display(const uint8_t bitmap[])
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

/** Plays the demo and returns when the button is pressed. */
void play_demo (void)
{
    uint32_t ticks_before_scene_change = 4500;//(PACER_RATE * TIME_BETWEEN_FRAMES)/MILLISECS_IN_A_SEC;
    
    //set_display(scene0);
    tinygl_text("  Tic-tac-toe!    ");

    pacer_init (PACER_RATE);
    uint16_t ticker = 0; 
    uint8_t scene = 0;
    
    while (1) {
        pacer_wait();
        tinygl_update();
        
        button_update(); //poll button
        
        // If button is pushed, return (skip the demo).
        if(button_push_event_p (BUTTON1))
        {
            tinygl_clear();
            tinygl_update();
			return;
		}
        
        //Demo animation
        if (ticker == ticks_before_scene_change) {
            switch (scene) {
                case 0:
                    tinygl_clear();
                    set_display(arrow);
                    break;
                default:
                    //gone through all the scenes
                    break;
            }
            scene++;
            ticker = 0;
        }
        
        
        
        tinygl_update(); //refresh screen
        ticker++;
        
    }
}
