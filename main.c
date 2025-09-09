#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// #define Y_SIZE 16
// #define X_SIZE 64

#define Y_SIZE 46
#define X_SIZE 180

char screen[Y_SIZE][X_SIZE];
const char GRAYSCALE[] = {
    '@', '%', '#', '*', '+', '=', '-', ':', '.', ' ', '\0'
};

void draw_line_pixels(int y0 ,int x0, int y1, int x1, char sign){
    // a'la bresenham algorithm
    int dy = y1 - y0;
    int dx = x1 - x0;
    float step = fmax(abs(dy), abs(dx));
    if (step != 0) {
        float step_y = dy / step;
        float step_x = dx / step;
        for (int i = 0; i < step+1; i++){
            mvaddch(
                roundf(y0 + i * step_y),
                roundf(x0 + i * step_x),
                sign
            );
        }
    }
}

void draw_borders(int y_0 ,int x_0, int y_max, int x_max) {
    draw_line_pixels(y_max, x_0, y_0, x_0, '|');       // left
    draw_line_pixels(y_0, x_max, y_max, x_max, '|');   // right
    draw_line_pixels(y_0, x_0, y_0, x_max, '-');       // top
    draw_line_pixels(y_max, x_max, y_max, x_0, '-');   // bottom
    
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

        draw_line_pixels(a_y, a_x, b_y, b_x, '*');
        mvaddch(a_y, a_x, '&');
        mvaddch(b_y, b_x, '&');
        draw_borders(0, 0, Y_SIZE, X_SIZE);

        refresh();
    }

    endwin();

    return 0;
}