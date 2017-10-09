/**
 * @file demo.c
 * @author Ollie Chick
 * @date 10 October 2017
 * @brief Demo module
 */

#include "system.h"
#include "pio.h"
#include "pacer.h"


static uint8_t last_column = 7;

/** Define PIO pins driving LED matrix rows.  */
static const pio_t rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO, 
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/** Define PIO pins driving LED matrix columns.  */
static const pio_t cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};


/** Bitmap of the arrow pointing to the button. */
static const uint8_t arrow[] =
{
    4, 14, 31, 4, 4
    // |631    |
    // |4268421|
    // |....@..|
    // |...@@@.|
    // |..@@@@@|
    // |....@..|
    // |....@..|
};


/**  Displays the row pattern in the column passed in. */
static void display_column (uint8_t row_pattern, uint8_t current_column)
{
    pio_output_high(cols[last_column]); //turn off last column
    
    uint8_t current_row;
    for (current_row = 0; current_row < 7; current_row++) {
        //Check if the point at the current column and row should be on
        if ((row_pattern >> current_row) & 1) {
            // yes
            pio_output_low(rows[current_row]);
        } else {
            // no
            pio_output_high(rows[current_row]);
        }
    }
    pio_output_low(cols[current_column]); //turn on this column
    last_column = current_column; // increment last column
}


/** Plays the demo and returns when the button is pressed. NOTE: CURRENTLY DOESN'T RETURN*/
void play_demo (void)
{
    uint8_t current_column = 0;
  
    system_init ();
    pacer_init (500);
    
    /* Initialise LED matrix pins.  */
    pio_config_set(LEDMAT_ROW1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW3_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW4_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW5_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW6_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW7_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL3_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL4_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL5_PIO, PIO_OUTPUT_HIGH);
    

    while (1)
    {
        pacer_wait ();
        
        // If button is pushed, return (skip the demo).
        if(pio_input_get(BUTTON1_PIO))
        {
			return;
		}
        
        // Demo animation
        
        
        
        
        display_column (arrow[current_column], current_column);
    
        current_column++;
    
        if (current_column > (LEDMAT_COLS_NUM - 1))
        {
            current_column = 0;
        }  
             
    }
}
