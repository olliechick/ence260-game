/**
 * @file game.c
 * @author Ollie Chick & Leo Lloyd
 * @brief Tic-tac-toe game.
 */
#include <stdbool.h>

#include "system.h"
#include "led.h"
#include "tinygl.h"
#include "ir_uart.h"
#include "../fonts/font5x7_1.h"

#include "demo.h"
#include "connect.h"
#include "play.h"

/* Pacer rate in Hz. */
#define PACER_RATE 500
/* How fast text scrolls in characters per 10 seconds. */
#define MESSAGE_RATE 10

/* Perform initialisation steps. */
void initialise(void) {
    
    system_init();
    ir_uart_init ();
    
    /* LED */
    led_init();
    led_set (LED1, false); //turn off LED
    
    /* TinyGL */
    tinygl_init(PACER_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
}


/* The main function of the game. */
int main (void) {
    
    initialise();

    /* Game loop (one loop is one game of tic-tac-toe). */
    while (1) {
        play_demo(); //returns when button is pushed

        /* Check what player you are. */
        Player player;
        if (connect()) {
            /* Got an immediate connection, so we are player 2. */
            player = PLAYER_2;
        } else {
            player = PLAYER_1;
        }
        
        play(player);
    }
}
