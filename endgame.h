

#ifndef ENDGAME_H
#define ENDGAME_H

/**
 * @return The player number who won (0 or 1).
 */
 int get_result(void);
 
 /** Displays the result as a face: either :), :(, or :| */
void display_result(int result);

void endgame(void);


#endif
