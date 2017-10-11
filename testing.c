#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


#define SEND_FREQUENCY 100 //Hz
#define MILLISECS_IN_A_SEC 1000
#define DELAY 3000 // wait time in milliseconds to get signal
int main(void)
{
    
    uint32_t total_paces = DELAY/(MILLISECS_IN_A_SEC/SEND_FREQUENCY) + 1;
    printf("%d\n", total_paces);
}
