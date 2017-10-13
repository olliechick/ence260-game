#include <stdbool.h>

#include "system.h"
#include "led.h"
#include "tinygl.h"
#include "ir_uart.h"
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
    ir_uart_init ();
    
    /* LED */
    led_init();
    led_set (LED1, false); //turn off LED
    
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
        led_set (LED1, true); //LED FOR DEBUG
        play_demo(); //returns when button is pushed
        led_set (LED1, false); //LED FOR DEBUG
        

        //Check what player you are
        Player player; // player number (PLAYER_1 or PLAYER_2)
        if (connect()) {
            // Got an immediate connection, so we are player 2
            player = PLAYER_2;
            led_set (LED1, true); //LED FOR DEBUG
        } else {
            player = PLAYER_1;
            led_set (LED1, false); //LED FOR DEBUG
        }
        
        play(player);
            
    }
    
    return 0;
}
