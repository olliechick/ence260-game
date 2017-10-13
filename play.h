

#ifndef ENDGAME_H
#define ENDGAME_H

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

typedef enum {NOT_FINISHED, PLAYER_1_WON, PLAYER_2_WON, TIE} Result;
typedef enum {PLAYER_1 = 1, PLAYER_2 = 2} Player;

/** Main game loop (and endgame). */ 
void play(Player thisPlayer);

#endif
