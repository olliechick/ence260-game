## Setup

First, you need to install the AVR compilation tools using the command `sudo apt-get install git gcc-avr binutils-avr avr-libc dfu-programmer`.

You then need to use the command `git clone https://eng-git.canterbury.ac.nz/rmc84/ence260-ucfk4.git` to get the software library required for controlling the funkit. Then move this directory (`ence260-game`) to `ence260-ucfk4/assignment/ence260-game`.

Additional documentation on the funkit can be found at http://ecewiki.elec.canterbury.ac.nz/mediawiki/index.php/UCFK4.

## Running the program

To load the program onto the board, run `make program` on each board from the root directory.

To start the program, click the button.

It will then display an R. This means it is trying to receive a signal from the other fun kit. After a short amount of time of not receiving anything, it will display an S. This means it is sending a signal to the other fun kit.

The fun kit that displays S when it connects will be player 1. This can be confirmed by the fact that player 1 will display a cursor first; player 2 will not.

The board will now load: on player 1’s screen, there will be two dots flashing at 5 Hz. This is the cursor. Use the navswitch to move the cursor (the LED will light when the cursor is on a valid location). Push the navswitch to select a position -- this will only work if the LED is on. It is now player 2’s turn. A cursor will appear on their screen, and the play the same way as player 2.

Player 1 is represented by two solid dots, and player 2 is represented by two dots flashing at 2 Hz. Play alternates until someone has one or the board is full; the result will then be shown. To play again, click the button.
