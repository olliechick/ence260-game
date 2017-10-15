#include <stdbool.h>
#include <stdint.h>

#include "tinygl.h"
#include "pacer.h"
#include "navswitch.h"
#include "led.h"
#include "ir_uart.h"

#include "system.h"
#include "pio.h"
#include "button.h"

#include "../fonts/font5x7_1.h"

#define BOARD_SIZE 9
#define ROW_SIZE 3
#define COL_SIZE 3

#define PACER_RATE 500 //Hz
#define P2_FLASH_RATE 2 //Hz
#define CURSOR_FLASH_RATE 5 //Hz

/** The game board */
static uint8_t board[9] = {0};
static uint8_t board_bitmap[5] = {0};

typedef enum {NOT_FINISHED, PLAYER_1_WON, PLAYER_2_WON, TIE} Result;
typedef enum {PLAYER_1 = 1, PLAYER_2 = 2} Player;

static Player player;

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

/** 
 * Updates board_bitmap based on the board, the position of the cursor,
 * and whether flashing elements are on or off.
 * @param p2_on true if player 2's LEDs should be turned on
 * @param cursor_position the position (as an index of board) of the cursor
 * @param cursor_on true if the cursor should be turned on
 */
static void set_board_bitmap(bool p2_on, uint8_t cursor_position, bool cursor_on)
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
            if (p2_on) {
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
 * Turns on the LED if the cursor is in an empty slot,
 * otherwise turns it off
 * @param cursor_position the position (as an index of board) of the cursor
 */
static void update_led(uint8_t cursor_position) {
    if (board[cursor_position] == 0) {
        led_set (LED1, true);
    } else {
        led_set (LED1, false);
    }
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
    bool p2_on = false;
    bool cursor_on = false;
    uint16_t ticker = 1; //Note this will overflow if PACER_RATE > 2^16-1 = 65535
    pacer_init(PACER_RATE);
    
    uint8_t cursor_position = find_init_cursor_position();
    
    set_board_bitmap(p2_on, cursor_position, cursor_on);
    set_display(board_bitmap);
    update_led(cursor_position);
    
    while (1) {
        pacer_wait();
        
        navswitch_update();
        tinygl_update();
        
        // Deal with navswitch pushes
        if (navswitch_push_event_p(NAVSWITCH_WEST)) {
            // up a row (if it won't go off the board)
            if (cursor_position >= ROW_SIZE) {
                cursor_position -= ROW_SIZE;
            }
            update_led(cursor_position);
        } else if (navswitch_push_event_p(NAVSWITCH_EAST)) {
            // down a row (if it won't go off the board)
            if (cursor_position < BOARD_SIZE - ROW_SIZE) {
                cursor_position += ROW_SIZE;
            }
            update_led(cursor_position);
        } else if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
            // right a column (if it won't go off the board)
            if (cursor_position%COL_SIZE != COL_SIZE - 1) {
                cursor_position++;
            }
            update_led(cursor_position);
        } else if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
            // left a column (if it won't go off the board)
            if (cursor_position%COL_SIZE != COL_SIZE) {
                cursor_position--;
            }
            update_led(cursor_position);
        } else if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            if (board[cursor_position] == 0) {
                //valid position
                board[cursor_position] = player;
                set_board_bitmap(p2_on, cursor_position, cursor_on);
                set_display(board_bitmap);
                ir_uart_putc(cursor_position); //send result to other player
                return get_result();
            }
        }
            
        // Flashing LEDs
        if (ticker % (PACER_RATE/P2_FLASH_RATE) == 0) {
            // Flash player 2's LEDs
            p2_on = !p2_on;
            set_board_bitmap(p2_on, cursor_position, cursor_on);
            set_display(board_bitmap);
        }
        if (ticker % (PACER_RATE/CURSOR_FLASH_RATE) == 0) {
            // Flash the cursor
            cursor_on = !cursor_on;
            set_board_bitmap(p2_on, cursor_position, cursor_on);
            set_display(board_bitmap);
        }
        
        //Reset ticker every second to prevent overflow
        if (ticker % PACER_RATE == 0) {
            ticker = 1;
        }
        
        ticker++;
    }
}

/**
 * Waits for a signal that the other player has had their turn.
 * It will then place their symbol in that position on the board.
 * @return the current result of the game
 */
static Result other_players_turn (void) {
    return get_result(); //DEBUG just ignore other player's turn
    while (1) {
        // Try and get a response
        if (ir_uart_read_ready_p()) {
            int i;
            // Iterate through valid board array indices
            for (i = 0; i < BOARD_SIZE; i++) {
                if (ir_uart_getc() == i) {
                    // Got a valid board array index
                    // This is where the other player went, so put
                    // them there on the board.
                    if (player == PLAYER_1) {
                        board[i] = PLAYER_2;
                    } else {
                        board[i] = PLAYER_1;
                    }
                    // TODO try uncommenting the below line if the board doesn't
                    // update (maybe when the other player wins?)
                    //set_board_bitmap(p2_on, cursor_position, cursor_on);
                    return get_result();
                } 
            }
        }
    }
}


 /** Displays the result as a face: either :), :(, or :| until the button is pushed */
static void display_result(Result result) {
    
    tinygl_clear();
    if ((result == PLAYER_1_WON && player == PLAYER_1) ||
        (result == PLAYER_2_WON && player == PLAYER_2)) {
        // Won
        // TODO display :)
        tinygl_text(":) win!");
    } else if ((result == PLAYER_1_WON && player == PLAYER_2) ||
        (result == PLAYER_2_WON && player == PLAYER_1)) {
        // Lost
        // TODO display :(
        tinygl_text(":( lose");
    } else if (result == TIE) {
        // Tie
        // TODO display :|
        tinygl_text(":I tie");
    }
    while (1) {
        //TODO return when the button is pushed
        pacer_wait();
        tinygl_update();

        button_update();
        
        if(button_push_event_p (BUTTON1))
        {
            tinygl_clear();
            tinygl_update();
			return;
		}
    }
}


/** Main game loop (and endgame). */ 
void play(Player thisPlayer) {

    player = thisPlayer; //assign player (globally)
    
    Result result = NOT_FINISHED;
    bool currentPlayersTurn;
    
    // Clear the board
    int i;
    for (i = 0; i < BOARD_SIZE; i++) {
        board[i] = 0;
    } 
    
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
