#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define DISPLAY_WIDTH 5
#define DISPLAY_HEIGHT 7

#define SEND_FREQUENCY 100 //Hz
#define MILLISECS_IN_A_SEC 1000
#define DELAY 3000 // wait time in milliseconds to get signal
#define BOARD_SIZE 9

#define PACER_RATE 500 //Hz
#define MILLISECS_IN_A_SEC 1000
#define TIME_BETWEEN_FRAMES 2000 //milliseconds between each frame in demo

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

static uint8_t board[9] = {1};
static uint8_t boardBitmap[5] = {0};

/** Updates board_bitmap based on board */
static void setBoardBitmap()
{
    int i;
    int row;
    for (i = 0; i < BOARD_SIZE; i++) {
        // Choose which row to modify
        if (i < 3) {
            row = 0;
        } else if (i < 6) {
            row = 2;
        } else {
            row = 4;
        }
        
        //Modify it
        if (board[i] == 1) {
            // player 1 is here
            boardBitmap[row] |= 1 << (5 - 2*(i%3)) | 1 << (6 - 2*(i%3));
        } else if (board[i] == 2) {
            // player 2 is here
            boardBitmap[row] |= 1 << (6 - 2*(i%3));
        }
    }
}


int main(void)
{
    board[0] = 1;
    board[1] = 2;
    board[2] = 1;
    board[3] = 0;
    board[4] = 1;
    board[5] = 0;
    
    setBoardBitmap();
    
    for (int i = 0; i < 9; i++) {
        printf("%d ", board[i]);
    }
    
    printf("\n");
    for (int i = 0; i < 5; i++) {
        printf(" "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(boardBitmap[i]));
    }
    /*
static const uint8_t bitmap[] =
{
    4, 14, 21, 4, 4
    // |631    |
    // |4268421|
    // |....@..|
    // |...@@@.|
    // |..@.@.@|
    // |....@..|
    // |....@..|
};
    int i, j;
    for (i = 0; i < DISPLAY_WIDTH; i++) {
        for (j = 0; j < DISPLAY_HEIGHT; j++) {
            bool val = bitmap[i] & (1 << j);
            printf("%d\n", val);
        }
    }
    
    uint8_t maxval = 500/(1000/500);
    printf("%d", maxval);*//*
    
    uint32_t ticks_before_scene_change = (PACER_RATE * TIME_BETWEEN_FRAMES)/MILLISECS_IN_A_SEC;
    printf("%d", ticks_before_scene_change);*/
    
    
}
