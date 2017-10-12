#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

int main(void)
{
    uint8_t cursor_position = 0;
    printf("%d ", cursor_position);
    cursor_position--;
    printf("\n%d ", cursor_position);
    printf("%d ", cursor_position%3);
    cursor_position++;
    printf("\n%d ", cursor_position);
    printf("%d ", cursor_position%3);
    
    
}
