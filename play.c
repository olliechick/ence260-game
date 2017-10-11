#include <stdbool.h>
#include <stdint.h>

#define BOARD_SIZE 9

/** The game board */
static uint8_t board[9] = {0};

typedef enum {NOT_FINISHED, PLAYER_1_WON, PLAYER_2_WON, TIE} Result;

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
 
 /** Displays the result as a face: either :), :(, or :| until the button is pushed */
static void display_result(Result result) {
    board[0] = result; //DEBUG so it doesn't complain about unused variables
    //TODO
    return;
}

/**
 * Current player having their turn.
 * @return the current result of the game
 */
static Result your_turn (void) {
    //TODO
    return get_result();
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
void play(uint8_t player_number) {
    
    Result result = NOT_FINISHED;
    bool currentPlayersTurn;
    
    // If this is player 1, it is your turn. Otherwise it's not.
    currentPlayersTurn = (player_number == 1);
    
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
