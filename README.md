# Snake
#### Video Demo:  <https://youtu.be/8YexnGIJKvI?si=XfA1ztmDjsaA6aCP>
#### Description:

## Written in C

The game is written in C, because I really liked C during the CS50 course.

### Several constants are defined at the start of the C file, such as:
* The size of the game board
* The char to be used for the snake head and tail
* The char to be used for the board
* The char to be used for the food

### Several global variables that later change are set afterwards, such as:

* The score
* The last key that was pressed by the player
* Whether there is flesh on the board
* The game speed
* The tail length
* The location of the snake head
* The location of the snake's tail, stored as an array
* The array storing the game board

### the functions

The RawMode functions disable and enable immediate access to keyboard input

The rand function sets the spawn location of the flesh to a random locations
The current time is used as the seed for this randomness

The move function sets variables to the location of the snake head and
its tail, as dictated by the direction of movement.

The populate function populates a 2d array that represents the game board,
taking into account the changes made by the move function.

The Draw function then prints the 2d array according to the width and height variables.

## AI disclaimer
AI was only used for the RawMode functions, as I was unable to find good documentation on achieving that.

