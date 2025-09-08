#include <curses.h>
#include <stdlib.h>
#include <time.h>

#define Y_SIZE 16
#define X_SIZE 64

char screen[Y_SIZE][X_SIZE];

int main() {
    WINDOW* win = initscr();
    curs_set(0);
    keypad(win, true);
    nodelay(win, true);
    srand(time(NULL)); // seed the rng with current time
    
    int a_y = rand() % Y_SIZE;
    int a_x = rand() % X_SIZE;

    int b_y = rand() % Y_SIZE;
    int b_x = rand() % X_SIZE;

    while (true) {
        erase();
        mvaddstr(a_y, a_x, "*");
        mvaddstr(b_y, b_x, "*");
        refresh();
    }

    endwin();

    return 0;
}