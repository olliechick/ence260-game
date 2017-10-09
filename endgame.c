#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "demo.c"

void endgame(void)
{

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

int result = get_result();
display_result(result);

}
