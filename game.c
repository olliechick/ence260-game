#include "system.h"
#include "demo.h"
#include "connect.h"
#include "endgame.h"

/** The game board */
uint8_t board[9] = {0};

/** The player number (0 or 1) */
static uint8_t player_number;

/**
 * One player having their turn.
 * @return 1 if the game is finished, 0 if not.
 */
int play (void) {
    return 1;
}

/**
 * Waits for a signal that the other player has had their turn.
 * It will then place their symbol in that position on the board.
 */
void other_player_play (void) {
    return;
}

/**
 * The main loop of the game.
 */
int main (void)
{
    int finished;
    int delay = 500; //wait time in milliseconds to get signal
    
    system_init();
    
    while (1) {
        play_demo(); //returns when button is pushed

        //Check what player you are
        if (connect(delay)) {
            player_number = 0;
            finished = play();
        } else {
            player_number = 1;
            finished = 0;
        }
    
        board[0] = player_number; //DEBUG so it doesn't complain about unused variables
        player_number = board[1]; //DEBUG so it doesn't complain about unused variables

        //Main game loop
        while (!finished) {
            other_player_play(); //wait for other player to play
            finished = play();
        }
        
        //game has now finished
        //endgame();
            
    }
    
    return 0;
}
