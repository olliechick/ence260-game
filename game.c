#include "system.h"
#include "led.h"
#include "display.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"

#include "demo.h"
#include "connect.h"
#include "play.h"

#define PACER_RATE 500
#define LOOP_RATE 500
#define MESSAGE_RATE 10

/** Performs initilisation steps */
void initialise(void) {
    
    system_init();
    display_init();
    
    /* LED */
    led_init();
    led_set (LED1, 0); //turn off LED
    
    /* TinyGL */
    tinygl_init(LOOP_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
}


/** The main loop of the game. */
int main (void) {
    
    initialise();
    
    while (1) {
        play_demo(); //returns when button is pushed
        

        //Check what player you are
        uint8_t player_number; // player number (1 or 2)
        if (connect()) {
            player_number = 1;
            led_set (LED1, 1); //LED FOR DEBUG
        } else {
            player_number = 2;
            led_set (LED1, 0); //LED FOR DEBUG
        }
        
        play(player_number);
            
    }
    
    return 0;
}
