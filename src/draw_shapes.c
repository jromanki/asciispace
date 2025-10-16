#include <math.h>
#include <curses.h>
#include "shape.h"


const char GRAYSCALE[] = {
    '`', '.', '-', '\'', ':', '_', ',', '^', '=', ';', '>', '<', '+', '!', 'r', 'c', '*',
    'z', '?', 's', 'L', 'T', 'v', ')', 'J', '7', '(', '|', 'F', 'i', '{', 'C', '}',
    'f', 'I', '3', '1', 't', 'l', 'u', '[', 'n', 'e', 'o', 'Z', '5', 'Y', 'x', 'j',
    'y', 'a', ']', '2', 'E', 'S', 'w', 'q', 'k', 'P', '6', 'h', '9', 'd', '4', 'V',
    'p', 'O', 'G', 'b', 'U', 'A', 'K', 'X', 'H', 'm', '8', 'R', 'D', '#', '$', 'B',
    'g', '0', 'M', 'N', 'W', 'Q', '%', '&', '@',
    '\0'
};

char opacity(float value){
    // clamp value to [0,1]
    if (value < 0.0f) value = 0.0f;
    if (value > 1.0f) value = 1.0f;

    int scale_size = sizeof(GRAYSCALE) - 2;
    int index = (int)(value * scale_size);
    return GRAYSCALE[index];
}

void draw_line_aliased(float y0 ,float x0, float y1, float x1){
    // Xiaolin Wu's Line Algorithm
    if (fabsf(y1 - y0) < fabsf(x1 - x0)) {
        if (x1 < x0){
            float temp;
            temp = x0; x0 = x1; x1 = temp;
            temp = y0; y0 = y1; y1 = temp;
        }
        float dx = x1 - x0;
        float dy = y1 - y0;
        float m;
        if (dx != 0){
            m = dy/dx;
        } else {
            m = 1;
        }
        float overlap = 1 - ((x0 + 0.5f) - (int) (x0 + 0.5f));
        float dist_start = y0 - (int) y0;
        mvaddch((int) y0, (int) (x0+0.5f), opacity(1-dist_start * overlap));
        mvaddch((int) y0+1, (int) (x0+0.5f), opacity(dist_start * overlap));
        
        overlap = ((x1 - 0.5f) - (int) (x1 - 0.5f));
        float dist_end = y1 - (int) y1;
        mvaddch((int) y1, (int) (x1+0.5f), opacity(1-dist_end * overlap));
        mvaddch((int) y1+1, (int) (x1+0.5f), opacity(dist_end * overlap));
        
        for (int i = 0; i < roundf(dx+0.5f); i++){
            float x = x0 + i;
            float y = y0 + i * m;
            int ix = (int) x; int iy = (int) y;
            float dist = y - iy;
            mvaddch(iy, ix, opacity(1-dist));
            mvaddch(iy + 1, ix, opacity(dist));
        }
    } else {
        if (y1 < y0){
            float temp;
            temp = x0; x0 = x1; x1 = temp;
            temp = y0; y0 = y1; y1 = temp;
        }
        float dx = x1 - x0;
        float dy = y1 - y0;
        float m;
        if (dy != 0){
            m = dx/dy;
        } else {
            m = 1;
        }
        float overlap = 1 - ((y0 + 0.5f) - (int) (y0 + 0.5f));
        float dist_start = y0 - (int) y0;
        mvaddch((int) y0, (int) (x0+0.5f), opacity(1-dist_start * overlap));
        mvaddch((int) y0+1, (int) (x0+0.5f), opacity(dist_start * overlap));
        
        overlap = ((y1 - 0.5f) - (int) (y1 - 0.5f));
        float dist_end = y1 - (int) y1;
        mvaddch((int) y1+0.5f, (int) x1, opacity(1-dist_end * overlap));
        mvaddch((int) y1+0.5f, (int) x1 + 1, opacity(dist_end * overlap));

        for (int i = 0; i < (int) dy+1; i++){
            float x = x0 + i * m;
            float y = y0 + i;
            int ix = (int) x; int iy = (int) y;
            float dist = x - ix;
            mvaddch(iy, ix, opacity(1-dist));
            mvaddch(iy, ix + 1, opacity(dist));
        }
    }
}

void scale_shape(Shape* s, float scale) {
    for (int i = 0; i < s->vertex_count; i++){
        s->vertices[i].x = s->vertices[i].x * scale;
        s->vertices[i].y = s->vertices[i].y * scale;
    }
}

void draw_shape(Shape* s) {
    for (int i = 0; i < s->vertex_count; i++){
        if (i != s->vertex_count - 1){
            draw_line_aliased(s->vertices[i].y, 2*s->vertices[i].x,
                s->vertices[i+1].y, 2*s->vertices[i+1].x);
            } else {
                draw_line_aliased(s->vertices[i].y, 2*s->vertices[i].x,
                s->vertices[0].y, 2*s->vertices[0].x);
            }
    }
}