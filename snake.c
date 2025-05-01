#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 20
#define HEIGHT 20
#define TEXTURE ". "
#define FLESH "* "

int score = 0;

// direction of movement 1 = w:up, 2 = a:left, 3 = s:down, 4 = d:right
int move = 0;

// is flesh already on the table
int isflesh = 0;

// random location for flesh spawn
int rn1;
int rn2;

// snake location
int sloc1;
int sloc2;

// has the game been started
int isgame = 0;

void draw(char key);
void enableRawMode(void);
void disableRawMode(void);
int input(void);
char getch(void);
void xrand(void);

int main(void) {
    char key = ' ';
    enableRawMode();
    xrand();
    draw(key);
    while (1) {
        if (input()) {
            key = getch();
            if (key == 'q') break;

            switch (key) {
                case 'w':
                    draw(key);
                    move = 1;
                    break;
                case 'a':
                    draw(key);
                    move = 2;
                    break;
                case 's':
                    draw(key);
                    move = 3;
                    break;
                case 'd':
                    draw(key);
                    move = 4;
                    break;
            }
        }
        usleep(10000);
    }
}

void draw(char key) {
    // clear terminal before re-printing the table
    fflush(stdout);
    usleep(10000);
    printf("\e[1;1H\e[2J");

    int bufferSize = (WIDTH * HEIGHT) + HEIGHT + 1;
    char*** table = malloc(WIDTH * sizeof(char**));
    if (table == NULL) {
        return;
    }

    for (int i = 0; i < WIDTH; i++) {
        table[i] = malloc(HEIGHT * sizeof(char*));
        for (int j = 0; j < HEIGHT; j++) {
            table[i][j] = TEXTURE;
        }
    }

    // makse sure rn1 and r2 are not in the center
    if (rn1 == HEIGHT / 2 && rn2 == WIDTH / 2) {
        xrand();
    }
    // draw the flesh at a random location
    table[rn1][rn2] = FLESH;

    // draw snake
    // if the game just started, draw the snake in the middle
    if (isgame == 0) {
        table[WIDTH / 2][HEIGHT / 2] = "O";
    }

    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            printf("%s", table[i][j]);
        }
        if (i == 5 && key != ' ') {
            printf("        last key pressed = %c", key);
        }
        printf("\n");
    }

    printf("\n");
    printf("score = %i\n", score);
    printf("Press W, A, S, D for movement.\n");
    printf("Press Q to quit the game.\n");
}

int input(void) {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

char getch() {
    char c;
    read(STDIN_FILENO, &c, 1);
    return c;
}

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
    srand((unsigned long)time(NULL));
    if (isflesh == 0) {
        rn1 = rand() % 20;
        rn2 = rand() % 20;
        isflesh = 1;
    }
}
