/**
 * @file demo.h
 * @author Ollie Chick
 * @date 10 October 2017
 * @brief Demo module
 */

#ifndef DEMO_H
#define DEMO_H

#include <stdbool.h>

#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "tinygl.h"
#include "button.h"

#define PACER_RATE 500

/** Plays the demo and returns when the button is pressed. */
void play_demo (void);

#endif
