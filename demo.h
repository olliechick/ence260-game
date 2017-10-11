/**
 * @file demo.c
 * @author Ollie Chick
 * @date 10 October 2017
 * @brief Demo module
 */
 
#include <stdbool.h>

#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "tinygl.h"
#include "button.h"

#define PACER_RATE 500 //Hz
#define MILLISECS_IN_A_SEC 1000
#define TIME_BETWEEN_FRAMES 2000 //milliseconds between each frame in demo


/** Plays the demo and returns when the button is pressed. */
void play_demo (void);
