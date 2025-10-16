#include <math.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>

#include "shape.h"

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

void you_lost_text(int x_size, int y_size){
    char loose_str[] = "You lost!";
    char again_str[] = "Press any key to play again.";
    mvaddstr(y_size/2-1, x_size/2-strlen(loose_str)/2, loose_str);
    mvaddstr(y_size/2, x_size/2-strlen(again_str)/2, again_str);
}

Point* stars_init(int x_size, int y_size, int star_amount){
    Point* star_arr = malloc(sizeof(Point) * star_amount);
    if (!star_arr) return NULL;
    for (int i = 0; i < star_amount; i++){
        star_arr[i].x = rand() % x_size;
        star_arr[i].y = rand() % y_size;
    }
    return star_arr;
}

void draw_stars(Point* star_arr, int star_amount){
    for (int i = 0; i < star_amount; i++){
        mvaddch(star_arr[i].y, star_arr[i].x, '*');
    }
}
