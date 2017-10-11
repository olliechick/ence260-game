#include <stdio.h>
#include <stdbool.h>

#define BOARD_SIZE 9


// 2 won
int board[] = {2, 1, 1,
               2, 2, 1,
               1, 2, 2};
       /*        
// tie               
int board[] = {1, 2, 1,
               1, 1, 2,
               2, 1, 2};*/
typedef enum {NOT_FINISHED, PLAYER_1_WON, PLAYER_2_WON, TIE} Result;

/**
 * @return The result of the game based on the board so far
 */
Result get_result(void) {
    // Check for three in a row on the rows (012, 345, 678)
    for (int i = 0; i < 9; i+=3) {
        if (board[i] == 1 && board[i+1] == 1 && board[i+2] == 1) {
            return PLAYER_1_WON;
        } else if (board[i] == 2 && board[i+1] == 2 && board[i+2] == 2) {
            return PLAYER_2_WON;
        }
    }
     
    // Check for three in a row on the cols (036, 147, 258)
    for (int i = 0; i < 3; i++) {
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
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == 0) {
            //found empty space
            arrayFull = false;
        }
    }
    if (arrayFull) {
        return TIE;
    }
    
    return NOT_FINISHED;
}

int main(void)
{
    printf("%d\n", get_result());
}
