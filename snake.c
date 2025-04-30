#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define WIDTH 20
#define HEIGHT 20
#define TEXTURE ". "

int score = 0;

void draw(char key);
void enableRawMode(void);
void disableRawMode(void);
int input(void);
char getch();

int main(void) {
    char key;
    enableRawMode();
    draw(key);
    while (1) {
        if (input()) {
            key = getch();
            if (key == 'q') break;

            switch (key) {
                case 'w':
                    draw(key);
                    break;
                case 'a':
                    draw(key);
                    break;
                case 's':
                    draw(key);
                    break;
                case 'd':
                    draw(key);
                    break;
            }
        }
        usleep(10000);
    }
}

void draw(char key) {
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

    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            printf("%s", table[i][j]);
        }
        if (i == 5) {
            printf("        %c", key);
        }
        printf("\n");
    }

    printf("\n");
    printf("score = %i\n", score);
    printf("Press W, A, S, D for movement.\n");
    printf("Press X to quit the game.\n");
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
