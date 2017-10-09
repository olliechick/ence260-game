#include "system.h"
#include "demo.h"
#include "connect.h"

/** The game board */
static uint8_t board[9] = {0};
static uint8_t player_number;

/**
 * One player having their turn.
 * @return 1 if the game is finished, 0 if not.
 */
int play (void) {
    return 0;
}

/**
 * Waits for a signal that the other player has had their turn.
 * It will then place their symbol in that position on the board.
 */
void other_player_play (void) {
    return;
}

/**
 * @return The player number who won (0 or 1).
 */
 int get_result(void) {
     return board[0]; //DEBUG just so board is not unused
 }
 
 /** Displays the result as a face: either :), :(, or :| */
void display_result(int result) {
    board[0] = result; //DEBUG just so result is not unused
    return;
}

/**
 * The main loop of the game.
 */
int main (void)
{
    int finished;
    
    while (1) {
        system_init();
        play_demo(); //returns when button is pushed

        //Check what player you are
        if (connect()) {
            player_number = 0;
            finished = play();
        } else {
            player_number = 1;
            finished = 0;
        }

        //Main game loop
        while (!finished) {
            other_player_play(); //wait for other player to play
            finished = play();
        }
        //game has now finished
        int result = get_result();
        display_result(result);
            
    }
}
