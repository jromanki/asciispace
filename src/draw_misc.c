#include <math.h>
#include <stdlib.h>
#include <curses.h>

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