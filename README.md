# Snake
#### Video Demo:  <https://youtu.be/8YexnGIJKvI?si=XfA1ztmDjsaA6aCP>

## Description:

### Written in C

The game is written in C, because I really liked C during the CS50 course.

### Several constants are defined at the start of the C file, such as:
* The size of the game board as WIDTH & HEIGHT
* The char to be used for the snake head and tail as SNAKE_HEAD & SNAKE_TAIL
* The char to be used for the board as TEXTURE
* The char to be used for the food as FLESH

### Several global variables that later change are set afterwards, such as:

* The score as "int score"
* The last key that was pressed by the player as "char key"
* The last *valid* key pressed by the player as "char mkey"
* Whether there is flesh on the board as "bool isFlesh"
* The game speed as "int gameSpeed"
* The tail length as "int length"
* The location of the snake head, stored as an array of two,

for the x,y locations

as "int loc[2]" & "int tmp[2]".

* The location of the snake's tail, stored as a 2d array the size of the board
* The 2d array storing the game board

### The functions

The arguments function checks for command line arguments and responds accordingly.
..
..
The RawMode functions disable and enable immediate access to keyboard input,
..
allowing the player to move using WASD.
..
..
The srand function sets all future rand functions to use the current time as
..
the seed for randomness.
..
..
The rand function sets the spawn location of the flesh to a random locations
..
as long as the player is not taking up that space.
..
..
The move function sets variables to the location of the snake head and
..
its tail, as dictated by the direction of movement.
..
If the head of the snake overlaps with the location of the flesh, then the
..
move function will frow the snake.
..
..
The populate function populates a 2d array that represents the game board,
..
taking into account the changes made by the move function.
..
It populates the array with numbers that represent each of the possible
..
chars: snake head, snake tail, flesh and the board color.
..
At the start of the game, the snake will be placed in the middle of the board
..
and the tail will be placed downwards.
..
If the game has already started, the whole board is reset by setting all the values in the array to 1.
..
Then a 3 is placed at the coordinates where the snake head is as dictated my move.
..
After that, a loop places a 4 at every location where the tail should be.
..
Lastly, flesh is placed by setting the random spot to 2.
..
The draw function then prints the 2d array using a loop the follows these rules:
* for every 1, print the texture constant.
* for every 4, print the snake tail constant.
* for every 3, print the snake head constant.
* for every 2, print the flesh constant.
Then print the current score and the movement rules
..
## AI disclaimer
AI was only used for the RawMode functions, as I was unable to find good documentation on achieving that.

