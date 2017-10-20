/**
 * @file play.c
 * @author Ollie Chick (och26) and Leo Lloyd (lll28)
 * @brief The main gameplay of Tic-tac-toe.
 */

#include <stdbool.h>
#include <stdint.h>

#include "tinygl.h"
#include "pacer.h"
#include "navswitch.h"
#include "led.h"
#include "ir_uart.h"
#include "../fonts/font5x7_1.h"
#include "system.h"
#include "pio.h"
#include "button.h"

#include "display_bitmap.h"

#define ROW_SIZE 3
#define COL_SIZE 3
#define BOARD_SIZE (ROW_SIZE * COL_SIZE)

/* Distance between LED rows of tic-tac-toe rows.*/
#define DISTANCE_BETWEEN_ROWS 2

/* The rate that player 2's pieces flash, in Hz. */
#define P2_FLASH_RATE 2
/* The rate that the cursor flashes, in Hz. */
#define CURSOR_FLASH_RATE 5

/* The game board */
static uint8_t board[9] = {0};
/* The bitmap of the board. */
static uint8_t board_bitmap[5] = {0};
/* The rows of LEDs used in each row of the game. */
static uint8_t rows[ROW_SIZE];
/* Position of cursor on the board. */
static uint8_t cursor_position = 0;
/* Ticker - used to control flashing. Note this will overflow if PACER_RATE > 2^16-1 = 65k */
static uint16_t ticker = 1;
/* Bools to control flashing of player 2 and the cursor - on or off. */
static bool p2_on = false;
static bool cursor_on = false;

/* The result of a game. */
typedef enum {NOT_FINISHED, PLAYER_1_WON, PLAYER_2_WON, TIE} Result;

/* The players - how they are stored in the board. */
typedef enum {PLAYER_1 = 1, PLAYER_2 = 2} Player;

/* The current player. */
static Player player;

/* @param player the player who won
   @return the result where the player passed in wins
*/
static Result toWinner(Player player) {
    if (player == PLAYER_1) {
        return PLAYER_1_WON;
    } else if (player == PLAYER_2) {
        return PLAYER_2_WON;
    } else {
        /* Code flow should not reach here. */
        return NOT_FINISHED;
    }
}


/* @return The result of the game based on the board so far
 */
static Result get_result(void) {

    /* Index used in for-loops. */
    int i;
    /* Index used in nested for-loops. */
    int j;
    /* The value of the first cell in a row/col/diagonal. */
    int first_cell;
    /* True if all the cells in a row/col/diagonal are the same. */
    bool all_the_same;
    
    /* Iterate through the rows, checking for a whole row of the same player. */
    for (i = 0; i < BOARD_SIZE; i += ROW_SIZE) {
        all_the_same = true; // assume true until proven wrong
        first_cell = board[i];
        
        /* Iterate through the cells in a row, checking that they equal the first cell.
           Note that it skips the first cell, as it will (by definition) equal itself. */
        for (j = i + 1; j % ROW_SIZE != 0; j++) {
            if (first_cell != board[j]) {
                all_the_same = false;
            }
        }
                
        if (all_the_same) {
            /* Someone has won - let's figure out who and return the appropriate result.
               Or the whole row is empty, in which case nothing happens. */
            return toWinner(first_cell);
        }
    }
    
    /* Iterate through the cols, checking for a whole col of the same player. */
    for (i = 0; i < ROW_SIZE; i++) {
        all_the_same = true; // assume true until proven wrong
        first_cell = board[i];
        
        /* Iterate through the cells in a col, checking that they equal the first cell.
           Note that it skips the first cell, as it will (by definition) equal itself. */
        for (j = i + ROW_SIZE; j < BOARD_SIZE; j += ROW_SIZE) {
            if (first_cell != board[j]) {
                all_the_same = false;
            }
        }
                
        if (all_the_same) {
            /* Someone has won - let's figure out who and return the appropriate result.
               Or the whole col is empty, in which case nothing happens. */
            return toWinner(first_cell);
        }
    }

    /* Check for a whole diagonal from top-left to bottom-right.
       Iterate through the cells in the diagonal, checking that they equal the first cell.
       Note that it skips the first cell, as it will (by definition) equal itself. */
    all_the_same = true; // assume true until proven wrong
    first_cell = board[0];
    
    for (i = ROW_SIZE + 1; i < BOARD_SIZE; i += ROW_SIZE + 1) {
        if (first_cell != board[i]) {
            all_the_same = false;
        }
    }
            
    if (all_the_same) {
        /* Someone has won - let's figure out who and return the appropriate result.
           Or the whole col is empty, in which case nothing happens. */
        return toWinner(first_cell);
    }

    /* Check for a whole diagonal from top-right to bottom-left.
       Iterate through the cells in the diagonal, checking that they equal the first cell.
       Note that it skips the first cell, as it will (by definition) equal itself. */
    all_the_same = true; // assume true until proven wrong
    first_cell = board[ROW_SIZE - 1];
    
    for (i = 2 * (ROW_SIZE - 1); i < BOARD_SIZE; i += ROW_SIZE - 1) {
        if (first_cell != board[i]) {
            all_the_same = false;
        }
    }
            
    if (all_the_same) {
        /* Someone has won - let's figure out who and return the appropriate result.
           Or the whole col is empty, in which case nothing happens. */
        return toWinner(first_cell);
    }
    
    /* Check for a tie - i.e. the board is full. */
    bool arrayFull = true; // assume board is full until you find an empty space
    for (i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == 0) {
            /* Found an empty space. */
            arrayFull = false;
        }
    }
    if (arrayFull) {
        return TIE;
    }
    
    /* Nothing so far has returned, so there is no result so far. */
    return NOT_FINISHED;
}


/* Sets LED in `board_bitmap` in position given by (row, col) to state. */
static void set_board_bitmap_led(uint8_t row, uint8_t col, bool state)
{
    if (state) {
        board_bitmap[row] |= 1 << (ROW_SIZE - 1 - col);
    } else {
        board_bitmap[row] &= ~(1 << (DISPLAY_HEIGHT - 1 - col));
    }
}


/* Sets `board_bitmap` based on cell of tic-tac-toe board. */
static void set_board_bitmap_cell (uint8_t cell, bool state)
{    
    /* Choose which row and columns to modify. */
    int row = rows[cell/COL_SIZE];
    int col1 = 2*(cell%3) - 6;
    int col2 = 2*(cell%3) - 7;
    
    /* Modify them. */
    set_board_bitmap_led(row, col1, state);
    set_board_bitmap_led(row, col2, state);
}
    

/* Updates the variable `board_bitmap` based on the board, the position of the cursor,
   and whether flashing elements are on or off.
   @param p2_on true if player 2's LEDs should be turned on.
   @param cursor_position the position (as an index of board) of the cursor.
   @param cursor_on true if the cursor should be turned on.
 */
static void set_board_bitmap(bool p2_on, uint8_t cursor_position, bool cursor_on)
{
    int i;

    /* Iterate through each cell, modifying the board based on what is in it. */
    for (i = 0; i < BOARD_SIZE; i++) {
        if (cursor_position == i) {
            /* The cursor is here. */
            set_board_bitmap_cell(i, cursor_on);
        } else if (board[i] == PLAYER_1) {
            /* Player 1 is here. */
            set_board_bitmap_cell(i, true);
        } else if (board[i] == PLAYER_2) {
            /* Player 2 is here. */
            set_board_bitmap_cell(i, p2_on);
        } else {
            /* Nothing is here. */
            set_board_bitmap_cell(i, false);
        }
    }
}


/*
 * Turns on the LED if the cursor is in an occupied slot,
 * otherwise turns it off.
 * @param cursor_position the position (as an index of board) of the cursor
 */
static void update_led(uint8_t cursor_position) {
    if (board[cursor_position]) {
        led_set (LED1, true);
    } else {
        led_set (LED1, false);
    }
}


/*
 * @return the initial position of the cursor on the board
 */
static uint8_t find_init_cursor_position(void) {
    int i = 0;
    /* Iterate through places on board. */
    for (i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == 0) {
            /* This place has nothing in it. */
            return i;
        }
    }
    
    /* If the code flow reaches here, the board is full. */
    return BOARD_SIZE;
}


/*
 * Current player having their turn.
 * @return the current result of the game
 */
static Result your_turn (void)
{   
    pacer_init(PACER_RATE);
    
    /* Initialise turn. */
    cursor_position = find_init_cursor_position();
    set_board_bitmap(p2_on, cursor_position, cursor_on);
    set_display(board_bitmap);
    update_led(cursor_position);
    
    /* Loop to check for navswitch pushes, and move cursor/return result as appropriate. */
    while (1) {
        pacer_wait();
        
        navswitch_update();
        tinygl_update();
        
        if (navswitch_push_event_p(NAVSWITCH_WEST) && (cursor_position >= ROW_SIZE)) {
            /* Move cursor up a row (if it won't go off the board). */
            cursor_position -= ROW_SIZE;
        } else if (navswitch_push_event_p(NAVSWITCH_EAST) && (cursor_position < BOARD_SIZE - ROW_SIZE)) {
            /* Move cursor down a row (if it won't go off the board). */
            cursor_position += ROW_SIZE;
        } else if (navswitch_push_event_p(NAVSWITCH_NORTH) && (cursor_position%COL_SIZE != COL_SIZE - 1)) {
            /* Move cursor right a column (if it won't go off the board). */
            cursor_position++;
        } else if (navswitch_push_event_p(NAVSWITCH_SOUTH) && (cursor_position%COL_SIZE != 0)) {
            /* Move cursor left a column (if it won't go off the board). */
            cursor_position--;
        } else if (navswitch_push_event_p(NAVSWITCH_PUSH) && (board[cursor_position] == 0) {
            /* Place player's mark in the cursor's location (if it is in a valid position). */
            board[cursor_position] = player;
            set_board_bitmap(p2_on, cursor_position, cursor_on);
            set_display(board_bitmap);
            ir_uart_putc(cursor_position); //send result to other player
            return get_result();
        }
        update_led(cursor_position);
            
        /* Flashing LEDs */
        if (ticker % (PACER_RATE/P2_FLASH_RATE) == 0) {
            /* Toggle player 2's LEDs. */
            p2_on = !p2_on;
            set_board_bitmap(p2_on, cursor_position, cursor_on);
            set_display(board_bitmap);
        }
        if (ticker % (PACER_RATE/CURSOR_FLASH_RATE) == 0) {
            /* Toggle the cursor. */
            cursor_on = !cursor_on;
            set_board_bitmap(p2_on, cursor_position, cursor_on);
            set_display(board_bitmap);
        }
        
        /* Reset ticker every second to prevent overflow. */
        if (ticker % PACER_RATE == 0) {
            ticker = 1;
        }
        
        ticker++;
    }
}


/*
 * Waits for a signal that the other player has had their turn.
 * It will then place their symbol in that position on the board.
 * @return the current result of the game
 */
static Result other_players_turn (void) {
    //return get_result(); //DEBUG - just ignore other player's turn
   
    /* Move the cursor off the board and turn it off. */
    cursor_on = false;
    cursor_position = BOARD_SIZE;
    
    while (1) {
        pacer_wait();
        tinygl_update();
        
        /* Try and get a response. */
        if (ir_uart_read_ready_p()) {
            int i;
            int received_position = ir_uart_getc();
            /* Iterate through valid board array indices. */
            for (i = 0; i < BOARD_SIZE; i++) {
                if (received_position == i) {
                    /* Got a valid board array index.
                       This is where the other player went, so put
                       them there on the board. */
                    if (player == PLAYER_1) {
                        board[i] = PLAYER_2;
                    } else {
                        board[i] = PLAYER_1;
                    }
                    return get_result();
                } 
            }
        }

        /* Flashing LEDs */
        if (ticker % (PACER_RATE/P2_FLASH_RATE) == 0) {
            // Toggle player 2's LEDs
            p2_on = !p2_on;
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


/* Displays the result until the button is pushed. */
static void display_result(Result result) {
    
    tinygl_clear();
    
    /* Check for win/loss/tie. */
    if ((result == PLAYER_1_WON && player == PLAYER_1) ||
        (result == PLAYER_2_WON && player == PLAYER_2)) {
        // Won
        tinygl_text("WINNER");
    } else if ((result == PLAYER_1_WON && player == PLAYER_2) ||
        (result == PLAYER_2_WON && player == PLAYER_1)) {
        // Lost
        tinygl_text("LOSER");
    } else if (result == TIE) {
        // Tie
        tinygl_text("TIE");
    }
    
    /* Check for button push (and update tinygl). */
    while (1) {
        pacer_wait();
        
        tinygl_update();
        button_update();
        
        /* Return when the button is pushed. */
        if (button_push_event_p (BUTTON1))
        {
            tinygl_clear();
            tinygl_update();
			return;
		}
    }
}


/* Main game loop (and endgame). */ 
void play(Player thisPlayer) {

    player = thisPlayer;
    Result result = NOT_FINISHED;

    /* True if it is the current player's turn. */
    bool currentPlayersTurn;
    
    /* Clear the board. */
    int i;
    for (i = 0; i < BOARD_SIZE; i++) {
        board[i] = 0;
    }

    /* Initialise the rows of LEDS to use. */
    int row = 0;
    for (i = 0; i < COL_SIZE; i++) {
        rows[i] = row;
        row += DISTANCE_BETWEEN_ROWS;
    }
    
    /* If this is player 1, it is your turn. Otherwise it's not. */
    currentPlayersTurn = (player == PLAYER_1);
    
    // Loop until game is finished - one loop is one player having their turn.
    while (result == NOT_FINISHED) {
        if (currentPlayersTurn) {
            result = your_turn(); // have your turn
        } else {
            result = other_players_turn(); // wait for other player to have their turn
        }
        currentPlayersTurn = !currentPlayersTurn; // swap whose turn it is
    }
    
    /* Game has now finished. */
    result = get_result();
    display_result(result);
    return;
}
