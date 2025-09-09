#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// #define Y_SIZE 16
// #define X_SIZE 64

#define Y_SIZE 46
#define X_SIZE 180

const char GRAYSCALE[] = {
    '@', '%', '#', '*', '+', '=', '-', ':', '.', ' ', '\0'
};

char opacity(float opacity){
    int scale_size = sizeof(GRAYSCALE) - 1;
    int sign_num = roundf((1 - opacity) * scale_size);
    return GRAYSCALE[sign_num];
}

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

void draw_line_aliased(float y0 ,float x0, float y1, float x1){
    if (fabsf(y1 - y0) < fabsf(x1 - x0)) {
        if (x1 < x0){
            float temp;
            temp = x0;
            x0 = x1;
            x1 = temp;
            temp = y0;
            y0 = y1;
            y1 = temp;
        }
        float dx = x1 - x0;
        float dy = y1 - y0;
        float m;
        if (dx != 0){
            m = dy/dx;
        } else {
            m = 1;
        }
        for (int i = 0; i < (int) dx+1; i++){
            float x = x0 + i;
            float y = y0 + i * m;
            int ix = (int) x;
            int iy = (int) y;
            float dist = y - iy;
            mvaddch(iy, ix, opacity(1-dist));
            mvaddch(iy + 1, ix, opacity(dist));
        }
    } else {
        if (y1 < y0){
            float temp;
            temp = x0;
            x0 = x1;
            x1 = temp;
            temp = y0;
            y0 = y1;
            y1 = temp;
        }
        float dx = x1 - x0;
        float dy = y1 - y0;
        float m;
        if (dy != 0){
            m = dx/dy;
        } else {
            m = 1;
        }
        for (int i = 0; i < (int) dy+1; i++){
            float x = x0 + i * m;
            float y = y0 + i;
            int ix = (int) x;
            int iy = (int) y;
            float dist = x - ix;
            mvaddch(iy, ix, opacity(1-dist));
            mvaddch(iy, ix + 1, opacity(dist));
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

        // draw_line_pixels(a_y, a_x, b_y, b_x, '*');
        draw_line_aliased(a_y, a_x, b_y, b_x);
        mvaddch(a_y, a_x, '&');
        mvaddch(b_y, b_x, '&');
        draw_borders(0, 0, Y_SIZE, X_SIZE);

        refresh();
    }

    endwin();

    return 0;
}