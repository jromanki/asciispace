#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define Y_SIZE 16
#define X_SIZE 64

char screen[Y_SIZE][X_SIZE];

void draw_line_pixels(int y0 ,int x0, int y1, int x1){
    // a'la bresenham algorithm
    int dy = y1 - y0;
    int dx = x1 - x0;
    float step = fmax(abs(dy), abs(dx));
    if (step != 0) {
        float step_y = dy / step;
        float step_x = dx / step;
        for (int i; i < step+1; i++){
            mvaddstr(roundf(y0 + i * step_y), roundf(x0 + i * step_x), "&");
        }
    }
}

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
        draw_line_pixels(a_y, a_x, b_y, b_x);
        mvaddstr(a_y, a_x, "&");
        mvaddstr(b_y, b_x, "&");
        refresh();
    }

    endwin();

    return 0;
}