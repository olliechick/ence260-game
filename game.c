#include "system.h"
#include "led.h" //LED FOR DEBUG
#include "demo.h"
#include "connect.h"
#include "play.h"

/**
 * The main loop of the game.
 */
int main (void)
{
    int delay = 1000; //wait time in milliseconds to get signal
    
    // Initialisation
    system_init();
    led_init(); //LED FOR DEBUG
    led_set (LED1, 0); //turn off led LED FOR DEBUG
    
    while (1) {
        play_demo(); //returns when button is pushed

        //Check what player you are
        uint8_t player_number; // player number (1 or 2)
        if (connect(delay)) {
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
