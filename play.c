#include <stdbool.h>
#include <stdint.h>

#include "tinygl.h"
#include "pacer.h"
#include "navswitch.h"
#include "led.h"

#define BOARD_SIZE 9
#define ROW_SIZE 3
#define COL_SIZE 3

#define PACER_RATE 500 //Hz
#define P2_FLIP_RATE 2 //Hz
#define CURSOR_FLASH_RATE 5 //Hz

/** The game board */
static uint8_t board[9] = {0};
static uint8_t board_bitmap[5] = {0};

typedef enum {NOT_FINISHED, PLAYER_1_WON, PLAYER_2_WON, TIE} Result;
typedef enum {PLAYER_1, PLAYER_2} Player;

/**
 * @return The result of the game based on the board so far
 */
static Result get_result(void) {
    
    int i; //index used in for loops
    
    // Check for three in a row on the rows (012, 345, 678)
    for (i = 0; i < BOARD_SIZE; i+=3) {
        if (board[i] == 1 && board[i+1] == 1 && board[i+2] == 1) {
            return PLAYER_1_WON;
        } else if (board[i] == 2 && board[i+1] == 2 && board[i+2] == 2) {
            return PLAYER_2_WON;
        }
    }
     
    // Check for three in a row on the cols (036, 147, 258)
    for (i = 0; i < 3; i++) {
        if (board[i] == 1 && board[i+3] == 1 && board[i+6] == 1) {
            return PLAYER_1_WON;
        } else if (board[i] == 2 && board[i+3] == 2 && board[i+6] == 2) {
            return PLAYER_2_WON;
        }
    }
    
    // Check for three in a row on the diagonals (048, 246)
    if (board[0] == 1 && board[4] == 1 && board[8] == 1) {
        return PLAYER_1_WON;
    } else if (board[0] == 2 && board[4] == 2 && board[8] == 2) {
        return PLAYER_2_WON;
    } 
    if (board[2] == 1 && board[4] == 1 && board[6] == 1) {
        return PLAYER_1_WON;
    } else if (board[2] == 2 && board[4] == 2 && board[6] == 2) {
        return PLAYER_2_WON;
    }
    
    // Check for a tie - i.e. the board is full
    bool arrayFull = true; //assume board is full until you find an empty space
    for (i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == 0) {
            //found empty space
            arrayFull = false;
        }
    }
    if (arrayFull) {
        return TIE;
    }
    
    // No result so far
    return NOT_FINISHED;
}

/** Updates board_bitmap based on board */
static void set_board_bitmap(bool flip, uint8_t cursor_position, bool cursor_on)
{
    int i;
    int row;
    for (i = 0; i < BOARD_SIZE; i++) {
        // Choose which row to modify
        if (i < 3) {
            row = 0; // First row
        } else if (i < 6) {
            row = 2; // Second row
        } else {
            row = 4; //Third row
        }
        
        //Modify it
        if (cursor_position == i) {
            // the cursor is here
            if (cursor_on) {
                // flash on
                board_bitmap[row] |= 1 << (5 - 2*(i%3)) | 1 << (6 - 2*(i%3));
            } else {
                //flash off
                board_bitmap[row] &= ~(1 << (5 - 2*(i%3))) & ~(1 << (6 - 2*(i%3)));
            }
        } else if (board[i] == 1) {
            // player 1 is here
            board_bitmap[row] |= 1 << (5 - 2*(i%3)) | 1 << (6 - 2*(i%3));
        } else if (board[i] == 2) {
            // player 2 is here
            if (flip) {
                board_bitmap[row] |= (1 << (5 - 2*(i%3)));
                board_bitmap[row] |= 1 << (6 - 2*(i%3));
            } else {
                board_bitmap[row] &= ~(1 << (6 - 2*(i%3)));
                board_bitmap[row] &= ~(1 << (5 - 2*(i%3)));
            }
        } else {
            //nothing is here
            board_bitmap[row] &= ~(1 << (5 - 2*(i%3))) & ~(1 << (6 - 2*(i%3)));
        }
    }
}


 /** Displays the result as a face: either :), :(, or :| until the button is pushed */
static void display_result(Result result) {
    board[0] = result; //DEBUG so it doesn't complain about unused variables
    //TODO
    return;
}

/** 
 * Sets the display (cell by cell) given a bitmap.
 * COPIED FROM demo.c - refactor into new module?
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

/**
 * @return the initial position of the cursor on the board
 */
static uint8_t find_init_cursor_position(void) {
    int i = 0;
    // Iterate through places on board
    for (i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == 0) {
            // This place has nothing in it
            return i;
        }
    }
    // If we find ourselves here, the board is full
    return BOARD_SIZE;
}

/**
 * Current player having their turn.
 * @return the current result of the game
 */
static Result your_turn (void)
{   
    bool flip = false;
    bool cursor_on = false;
    uint16_t ticker = 1; //Note this will overflow if PACER_RATE > 2^16 = 65536
    pacer_init(PACER_RATE);
    
    // TEST configuration of board - empty board
    board[0] = 0;
    board[1] = 0;
    board[2] = 0;
    board[3] = 0;
    board[4] = 0;
    board[5] = 0;
    board[6] = 0;
    board[7] = 0;
    board[8] = 0;
    // ENDOF TEST configuration
        
    // TEST configuration of board
    board[0] = 1;
    board[1] = 2;
    board[2] = 0;
    board[3] = 0;
    board[4] = 1;
    board[5] = 1;
    board[6] = 2;
    board[7] = 0;
    board[8] = 2;
    // ENDOF TEST configuration*/
    
    uint8_t cursor_position = find_init_cursor_position();
    
    set_board_bitmap(flip, cursor_position, cursor_on);
    set_display(board_bitmap);
    
    while (1) {
        pacer_wait();
        
        navswitch_update ();
        
        if (navswitch_push_event_p(NAVSWITCH_WEST)) {
            // up a row (if it won't go off the board)
            if (cursor_position >= ROW_SIZE) {
                cursor_position -= ROW_SIZE;
            }
        } else if (navswitch_push_event_p(NAVSWITCH_EAST)) {
            // down a row (if it won't go off the board)
            if (cursor_position < BOARD_SIZE - ROW_SIZE) {
                cursor_position += ROW_SIZE;
            }
        } else if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
            // right a column (if it won't go off the board)
            if (cursor_position%COL_SIZE != COL_SIZE - 1) {
                cursor_position++;
            }
        } else if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
            // left a column (if it won't go off the board)
            if (cursor_position%COL_SIZE != COL_SIZE) {
                cursor_position--;
            }
        } else if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            if (board[cursor_position] == 0) {
                //valid position
                led_set (LED1, false); //LED FOR DEBUG
                board[cursor_position] == PLAYER_1; //TODO set to current player
                return get_result();
            } else {
                //invalid position
                led_set (LED1, true); //LED FOR DEBUG
            }
        }
            
        
        if (ticker % (PACER_RATE/P2_FLIP_RATE) == 0) {
            flip = !flip;
            set_board_bitmap(flip, cursor_position, cursor_on);
            set_display(board_bitmap);
        }
        if (ticker % (PACER_RATE/CURSOR_FLASH_RATE) == 0) {
            //Flash the cursor at 5Hz
            cursor_on = !cursor_on;
            set_board_bitmap(flip, cursor_position, cursor_on);
            set_display(board_bitmap);
        }
        if (ticker % PACER_RATE == 0) {
            //Reset ticker every 1Hz to prevent overflow
            ticker = 1;
        }
        
        tinygl_update();
        ticker++;
    }
}

/**
 * Waits for a signal that the other player has had their turn.
 * It will then place their symbol in that position on the board.
 * @return the current result of the game
 */
static Result other_players_turn (void) {
    //TODO
    return get_result();
}


/** Main game loop (and endgame). */ 
void play(Player player) {
    
    Result result = NOT_FINISHED;
    bool currentPlayersTurn;
    
    // If this is player 1, it is your turn. Otherwise it's not.
    currentPlayersTurn = (player == PLAYER_1);
    
    // Loop until finished
    while (result == NOT_FINISHED) {
        if (currentPlayersTurn) {
            result = your_turn(); // have your turn
        } else {
            result = other_players_turn(); // wait for other player to have their turn
        }
        currentPlayersTurn = !currentPlayersTurn; // invert flag
    }
    
    //game has now finished
    result = get_result();
    display_result(result);
    return;
    
}
