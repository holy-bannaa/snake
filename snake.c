#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define HEIGHT 20
#define WIDTH 20
#define TEXTURE ". "
#define FLESH "* "
#define SNAKE_HEAD "O "
#define SNAKE_BOD "o "

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

// snake length
int length = 0;
// tail history
int tail[HEIGHT * WIDTH][2];
// growth prep
int grow;

// has the game been started
int isgame = 0;

int draw(char key);
void enableRawMode(void);
void disableRawMode(void);
int input(void);
char getch(void);
void xrand(void);

int main(void) {
    srand(time(NULL));
    char key = ' ';
    char kkey = ' ';
    enableRawMode();
    xrand();
    draw(key);
    while (1) {
        if (input()) {
            // set kkey to any valid movment key
            key = getch();
            if (key == 'q') break;
            switch (key) {
                case 'w':
                    kkey = key;
                    break;
                case 'a':
                    kkey = key;
                    break;
                case 's':
                    kkey = key;
                    break;
                case 'd':
                    kkey = key;
                    break;
            }
        }
        if (kkey != ' ') {
            isgame = 1;
            switch (kkey) {
                case 'w':
                    move = 1;
                    break;
                case 'a':
                    move = 2;
                    break;
                case 's':
                    move = 3;
                    break;
                case 'd':
                    move = 4;
            }
            // game over screen
            if (draw(kkey) == 1) {
                printf("game over\n");
                printf("final score: %i\n", score);
                return 1;
            }
            usleep(150000);
        }
    }
}

int draw(char key) {
    // clear terminal before re-printing the table
    fflush(stdout);
    usleep(10000);
    printf("\e[1;1H\e[2J");

    // create 2d array of the table
    char*** table = malloc(HEIGHT * sizeof(char**));
    if (table == NULL) {
        return 2;
    }

    for (int i = 0; i < HEIGHT; i++) {
        table[i] = malloc(WIDTH * sizeof(char*));
        for (int j = 0; j < WIDTH; j++) {
            table[i][j] = TEXTURE;
        }
    }

    // draw snake
    // if a key is pressed
    if (key != ' ') {
        if (move == 1) {
            // commit death if going out of bounds
            if (sloc1 - 1 < 0) {
                return 1;
            }

            // commit death if touching tail
            for (int i = 0; i < length; i++) {
                if (sloc1 - 1 == tail[i][0] && sloc2 == tail[i][1]) {
                    return 1;
                }
            }

            // draw head
            table[sloc1 - 1][sloc2] = SNAKE_HEAD;
            // draw tail
            if (grow == 1) {
                table[sloc1][sloc2] = SNAKE_BOD;
                tail[length][0] = sloc1;
                tail[length][1] = sloc2;
                grow = 0;
                length++;
            } else if (length > 0) {
                int tmp[4] = {sloc1, sloc2};
                for (int i = 0; i < length; i++) {
                    tmp[2] = tail[length - i][0];
                    tmp[3] = tail[length - i][1];

                    tail[length - i][0] = tmp[0];
                    tail[length - i][1] = tmp[1];

                    tmp[0] = tmp[2];
                    tmp[1] = tmp[3];

                    table[tail[length - i][0]][tail[length - i][1]] = SNAKE_BOD;
                }
            } else {
                table[sloc1][sloc2] = TEXTURE;
            }
            sloc1--;
        }

        else if (move == 2) {
            if (sloc2 - 1 < 0) {
                return 1;
            }

            // commit death if touching tail
            for (int i = 0; i < length; i++) {
                if (sloc1 == tail[i][0] && sloc2 - 1 == tail[i][1]) {
                    return 1;
                }
            }

            table[sloc1][sloc2 - 1] = SNAKE_HEAD;
            // draw tail
            if (grow == 1) {
                table[sloc1][sloc2] = SNAKE_BOD;
                tail[length][0] = sloc1;
                tail[length][1] = sloc2;
                grow = 0;
                length++;
            } else if (length > 0) {
                int tmp[4] = {sloc1, sloc2};
                for (int i = 0; i < length; i++) {
                    tmp[2] = tail[length - i][0];
                    tmp[3] = tail[length - i][1];

                    tail[length - i][0] = tmp[0];
                    tail[length - i][1] = tmp[1];

                    tmp[0] = tmp[2];
                    tmp[1] = tmp[3];

                    table[tail[length - i][0]][tail[length - i][1]] = SNAKE_BOD;
                }
            } else {
                table[sloc1][sloc2] = TEXTURE;
            }
            sloc2--;
        }

        else if (move == 3) {
            if (sloc1 + 1 > HEIGHT - 1) {
                return 1;
            }

            // commit death if touching tail
            for (int i = 0; i < length; i++) {
                if (sloc1 + 1== tail[i][0] && sloc2 == tail[i][1]) {
                    return 1;
                }
            }

            table[sloc1 + 1][sloc2] = SNAKE_HEAD;
            // draw tail
            if (grow == 1) {
                table[sloc1][sloc2] = SNAKE_BOD;
                tail[length][0] = sloc1;
                tail[length][1] = sloc2;
                grow = 0;
                length++;
            } else if (length > 0) {
                int tmp[4] = {sloc1, sloc2};
                for (int i = 0; i < length; i++) {
                    tmp[2] = tail[length - i][0];
                    tmp[3] = tail[length - i][1];

                    tail[length - i][0] = tmp[0];
                    tail[length - i][1] = tmp[1];

                    tmp[0] = tmp[2];
                    tmp[1] = tmp[3];

                    table[tail[length - i][0]][tail[length - i][1]] = SNAKE_BOD;
                }
            } else {
                table[sloc1][sloc2] = TEXTURE;
            }
            sloc1++;
        }

        else if (move == 4) {
            if (sloc2 + 1 > WIDTH - 1) {
                return 1;
            }

            // commit death if touching tail
            for (int i = 0; i < length; i++) {
                if (sloc1== tail[i][0] && sloc2 + 1 == tail[i][1]) {
                    return 1;
                }
            }

            table[sloc1][sloc2 + 1] = SNAKE_HEAD;
            // draw tail
            if (grow == 1) {
                table[sloc1][sloc2] = SNAKE_BOD;
                tail[length][0] = sloc1;
                tail[length][1] = sloc2;
                grow = 0;
                length++;
            } else if (length > 0) {
                int tmp[4] = {sloc1, sloc2};
                for (int i = 0; i < length; i++) {
                    tmp[2] = tail[length - i][0];
                    tmp[3] = tail[length - i][1];

                    tail[length - i][0] = tmp[0];
                    tail[length - i][1] = tmp[1];

                    tmp[0] = tmp[2];
                    tmp[1] = tmp[3];

                    table[tail[length - i][0]][tail[length - i][1]] = SNAKE_BOD;
                }
            } else {
                table[sloc1][sloc2] = TEXTURE;
            }
            sloc2++;
        }

        if (sloc1 == rn1 && sloc2 == rn2) {
            score++;
            // prepare for growth
            grow = 1;
            // spawn flesh at a different location
            isflesh = 0;
            xrand();
        }
        table[rn1][rn2] = FLESH;
    }

    // if the game just started, draw the snake in the middle and spawn flesh
    if (!isgame) {
        table[HEIGHT / 2][WIDTH / 2] = SNAKE_HEAD;

        // set snake loc variable
        sloc1 = HEIGHT / 2;
        sloc2 = WIDTH / 2;

        // make sure rn1 and r2 are not in the center at the start of the game
        while (rn1 == WIDTH / 2 && rn2 == HEIGHT / 2) {
            // draw the flesh at a random location
            xrand();
        }
        table[rn1][rn2] = FLESH;
    }

    // print table
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
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

    for (int i = 0; i < HEIGHT; i++) {
        free(table[i]);
    }
    free(table);
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
    if (isflesh == 0) {
        rn1 = rand() % 20;
        rn2 = rand() % 20;
        isflesh = 1;
    }
}
