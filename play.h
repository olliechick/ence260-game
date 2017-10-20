/**
 * @file play.c
 * @author Ollie Chick (och26) and Leo Lloyd (lll28)
 * @brief The main gameplay of Tic-tac-toe.
 */

#ifndef ENDGAME_H
#define ENDGAME_H

#include "system.h"

#define ROW_SIZE 3
#define COL_SIZE 3
#define BOARD_SIZE (ROW_SIZE * COL_SIZE)

/* Distance between LED rows of tic-tac-toe rows.*/
#define DISTANCE_BETWEEN_ROWS 2

/* The rate that player 2's pieces flash, in Hz. */
#define P2_FLASH_RATE 2
/* The rate that the cursor flashes, in Hz. */
#define CURSOR_FLASH_RATE 5

/* The result of a game. */
typedef enum {NOT_FINISHED, PLAYER_1_WON, PLAYER_2_WON, TIE} Result;

/* The players - how they are stored in the board. */
typedef enum {PLAYER_1 = 1, PLAYER_2 = 2} Player;

/* Main game loop (and endgame). */ 
void play(Player thisPlayer);

#endif
