#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

bool move(void);
void draw(void);
void populate(void);
void gameOver(void);
void enableRawMode(void);
void disableRawMode(void);
int input(void);
char getch(void);
void xrand(void);
bool arguments(int argc, char *argv[]);

#define WIDTH 20
#define HEIGHT 20
#define TEXTURE ". "
#define FLESH "* "
#define SNAKE_HEAD "O "
#define SNAKE_TAIL "o "

// number used for storing arguments
int num;

int gameSpeed = 50000;
int score = 0;

// has the game sarted
bool isGame = false;

// valid keys
char keys[4] = {'w', 'a', 's', 'd'};
// key pressed
char key;
// store curent movement key
char mkey = ' ';

// array for storing the random flesh locatins
int rn[2];

// is there flesh on the table
bool isFlesh = false;

// snake head location
int loc[2];
int tmp[4];
// snake tail length
int length = 4;
// tail location
int tailLoc[HEIGHT * WIDTH][2];

// table aka game board
int table[HEIGHT][WIDTH];

int main(int argc, char *argv[]) {
	if (!arguments(argc, argv)) {
		printf("syntax: %s [tail length: 1 ~ %i, default = %i] [game speed: 1 "
			   "~ 10, default = 2]\n",
			   argv[0], HEIGHT / 2 - 1, length);
		return 1;
	}
	// set randomizer seed to the curent time
	srand(time(NULL));
	enableRawMode();
	populate();
	draw();
	while (true) {
		// when a key is pressed
		if (input()) {
			// set key to whatver key was pressed
			key = getch();
			if (key == 'q') {
				disableRawMode();
				break;
			}
			// set mkey to correct key
			for (int i = 0; i < 4; i++) {
				if (key == keys[i]) {
					mkey = key;
				}
			}
		}
		// if a movement key has been pressed
		if (mkey != ' ') {
			isGame = true;
			if (!move()) {
				fflush(stdout);
				printf("\e[1;1H\e[2J");
				gameOver();
				return 0;
			}
		}

		// after the game starts
		if (isGame) {
			// re-populate table, taking into account the movements
			populate();
			// re-draw table after making changes
			draw();
			usleep(gameSpeed);
		}
	}
}

bool move(void) {
	if (mkey == 'w') {
		// store curent tail location in tmp
		tmp[0] = loc[0];
		tmp[1] = loc[1];
		// eat
		if (loc[0] - 1 == rn[0] && loc[1] == rn[1]) {
			score++;
			length++;
			isFlesh = false;
			loc[0]--;
			tailLoc[length - 1][0] = tmp[0];
			tailLoc[length - 1][1] = tmp[1];
		} else {
			// move tail
			for (int i = 1; i <= length; i++) {
				// store curent tail location in tmp -> move curent tail to new
				// location -> set tmp to old tail location which is now empty,
				// so the next tail can be moved there
				tmp[2] = tailLoc[length - i][0];
				tmp[3] = tailLoc[length - i][1];

				tailLoc[length - i][0] = tmp[0];
				tailLoc[length - i][1] = tmp[1];

				tmp[0] = tmp[2];
				tmp[1] = tmp[3];
			}
			// move head
			loc[0]--;
		}
	} else if (mkey == 'a') {
		// store curent tail location in tmp
		tmp[0] = loc[0];
		tmp[1] = loc[1];
		// eat
		if (loc[0] == rn[0] && loc[1] - 1 == rn[1]) {
			score++;
			length++;
			isFlesh = false;
			loc[1]--;
			tailLoc[length - 1][0] = tmp[0];
			tailLoc[length - 1][1] = tmp[1];
		} else {
			// move tail
			for (int i = 1; i <= length; i++) {
				// store curent tail location in tmp -> move curent tail to new
				// location -> set tmp to old tail location which is now empty,
				// so the next tail can be moved there
				tmp[2] = tailLoc[length - i][0];
				tmp[3] = tailLoc[length - i][1];

				tailLoc[length - i][0] = tmp[0];
				tailLoc[length - i][1] = tmp[1];

				tmp[0] = tmp[2];
				tmp[1] = tmp[3];
			}
			// move head
			loc[1]--;
		}
	} else if (mkey == 's') {
		// store curent tail location in tmp
		tmp[0] = loc[0];
		tmp[1] = loc[1];
		// eat
		if (loc[0] + 1 == rn[0] && loc[1] == rn[1]) {
			score++;
			length++;
			isFlesh = false;
			loc[0]++;
			tailLoc[length - 1][0] = tmp[0];
			tailLoc[length - 1][1] = tmp[1];
		} else {
			// move tail
			for (int i = 1; i <= length; i++) {
				// store curent tail location in tmp -> move curent tail to new
				// location -> set tmp to old tail location which is now empty,
				// so the next tail can be moved there
				tmp[2] = tailLoc[length - i][0];
				tmp[3] = tailLoc[length - i][1];

				tailLoc[length - i][0] = tmp[0];
				tailLoc[length - i][1] = tmp[1];

				tmp[0] = tmp[2];
				tmp[1] = tmp[3];
			}
			// move head
			loc[0]++;
		}
	} else if (mkey == 'd') {
		// store curent tail location in tmp
		tmp[0] = loc[0];
		tmp[1] = loc[1];
		// eat
		if (loc[0] == rn[0] && loc[1] + 1 == rn[1]) {
			score++;
			length++;
			isFlesh = false;
			loc[1]++;
			tailLoc[length - 1][0] = tmp[0];
			tailLoc[length - 1][1] = tmp[1];
		} else {
			// move tail
			for (int i = 1; i <= length; i++) {
				// store curent tail location in tmp -> move curent tail to new
				// location -> set tmp to old tail location which is now empty,
				// so the next tail can be moved there
				tmp[2] = tailLoc[length - i][0];
				tmp[3] = tailLoc[length - i][1];

				tailLoc[length - i][0] = tmp[0];
				tailLoc[length - i][1] = tmp[1];

				tmp[0] = tmp[2];
				tmp[1] = tmp[3];
			}
			// move head
			loc[1]++;
		}
	}
	// end game if hit wall
	if (loc[0] > HEIGHT - 1 || loc[0] < 0 || loc[1] > WIDTH - 1 || loc[1] < 0)
		return false;
	// end game if hit tail
	for (int i = 0; i < length; i++) {
		if (loc[0] == tailLoc[i][0] && loc[1] == tailLoc[i][1])
			return false;
	}
	return true;
}

void draw(void) {
	// clear terminal before re-printing the table
	fflush(stdout);
	printf("\e[1;1H\e[2J");

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			switch (table[i][j]) {
			// in order of most common
			case 1:
				printf("%s", TEXTURE);
				break;
			case 4:
				printf("%s", SNAKE_TAIL);
				break;
			case 3:
				printf("%s", SNAKE_HEAD);
				break;
			case 2:
				printf("%s", FLESH);
				break;
			}
		}
		printf("\n");
	}

	printf("\n");
	printf("score = %i\n", score);
	printf("Press W, A, S, D for movement.\n");
	printf("Press Q to quit the game.\n");
}

void populate(void) {
	// clear terminal before re-printing the table
	fflush(stdout);
	printf("\e[1;1H\e[2J");

	// 1 = TEXTURE, 2 = FLESH, 3 = SNAKE_HEAD, 4 = SNAKE_TAIL

	// make the whole table the default texture
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			table[i][j] = 1;
		}
	}

	// place snake in the middle of the table at the start of the game
	if (!isGame) {
		table[HEIGHT / 2][WIDTH / 2] = 3;
		loc[0] = HEIGHT / 2;
		loc[1] = WIDTH / 2;

		// place tail
		for (int i = 1; i <= length; i++) {
			tailLoc[length - i][0] = HEIGHT / 2 + i;
			tailLoc[length - i][1] = WIDTH / 2;
		}
	}

	// place the snake at the corect location as dictaded by move()
	table[loc[0]][loc[1]] = 3;
	// place tail
	for (int i = 0; i < length; i++) {
		table[tailLoc[i][0]][tailLoc[i][1]] = 4;
	}

	// generate flesh at the random location
	if (!isFlesh) {
		xrand();
		isFlesh = true;
	}
	table[rn[0]][rn[1]] = 2;
}

void gameOver(void) {
	printf("Game Over\n");
	printf("final score: %i\n", score);
}

// listen for input
int input(void) {
	struct timeval tv = {0L, 0L};
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv);
}

// put whatever key is pressed into a variable
char getch() {
	char c;
	read(STDIN_FILENO, &c, 1);
	return c;
}

// enable access to input imediately
void enableRawMode(void) {
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void disableRawMode(void) {
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= (ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// generate two numbers between 0 ~ 20
void xrand(void) {
	int safe;
	do {
		safe = true;
		rn[0] = rand() % HEIGHT;
		rn[1] = rand() % WIDTH;
		for (int i = 0; i < length; i++) {
			if (rn[0] == tailLoc[i][0] && rn[1] == tailLoc[i][1]) {
				safe = false;
				break;
			}
		}
	} while (rn[0] == HEIGHT / 2 && rn[1] == WIDTH / 2 && !safe);
}

bool arguments(int argc, char *argv[]) {
	// if no arguments are provided, use defaults
	if (argc == 1) {
		gameSpeed = gameSpeed * 2;
		return true;
	}
	// if one argument is provided
	if (argc == 2) {
		// if argument is not an int or is out of bounds
		num = atoi(argv[1]);
		if (num == 0 || num < 0 || num > HEIGHT / 2 - 1) {
			return false;
		}
		length = atoi(argv[1]);
		gameSpeed = gameSpeed * 2;
		return true;
	}
	// if two arguments are provided
	if (argc == 3) {
		// if first argument is not an int or is out of bounds
		num = atoi(argv[1]);
		if (num == 0 || num < 0 || num > HEIGHT / 2 - 1) {
			return false;
		}
		length = atoi(argv[1]);
		// if second argument is not an int or is out of bounds
		num = atoi(argv[2]);
		if (num == 0 || num < 1 || num > 10) {
			return false;
		}
		gameSpeed = (gameSpeed * 4) / atoi(argv[2]);
		return true;
	}
	// if too many arguments are provided
	if (argc > 3) {
		return false;
	}
}
