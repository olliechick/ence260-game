#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define DISPLAY_WIDTH 5
#define DISPLAY_HEIGHT 7

#define SEND_FREQUENCY 100 //Hz
#define MILLISECS_IN_A_SEC 1000
#define DELAY 3000 // wait time in milliseconds to get signal

#define PACER_RATE 500 //Hz
#define MILLISECS_IN_A_SEC 1000
#define TIME_BETWEEN_FRAMES 2000 //milliseconds between each frame in demo
int main(void)
{
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
    printf("%d", maxval);*/
    
    uint32_t ticks_before_scene_change = (PACER_RATE * TIME_BETWEEN_FRAMES)/MILLISECS_IN_A_SEC;
    printf("%d", ticks_before_scene_change);
}
