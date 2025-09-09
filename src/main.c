#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "shape.h"
#include "draw_misc.h"
#include "draw_shapes.h"

#define Y_SIZE 46
#define X_SIZE 180
#define FPS 60
#define SPEED_FACTOR 0.14f
#define ANGULAR_LIMIT 0.1f

void translate(Shape* s, float x, float y){
    for (int i = 0; i < s->vertex_count; i++){
        s->vertices[i].x += x;
        s->vertices[i].y += y;
    }
    s->center.x += x;
    s->center.y += y;
}

void rotate(Shape* s, float theta){
    for (int i = 0; i < s->vertex_count; i++){
        float x = s->vertices[i].x;
        float y = s->vertices[i].y;
        s->vertices[i].x = x * cos(theta) - y * sin(theta);
        s->vertices[i].y = x * sin(theta) + y * cos(theta);
    }
    float x_facing = s->facing.x;
    float y_facing = s->facing.y;
    s->facing.x = x_facing * cos(theta) - y_facing * sin(theta);
    s->facing.y = x_facing * sin(theta) + y_facing * cos(theta);
}



bool check_loose(Shape* s) {
    for (int i = 0; i < s->vertex_count; i++){
        if (s->vertices[i].x <= 0 || s->vertices[i].x >= X_SIZE/2){
            return true;
        } else if (s->vertices[i].y <= 0 || s->vertices[i].y >= Y_SIZE){
            return true;
        }
    }
    return false;
}

int main() {
    WINDOW* win = initscr();
    curs_set(0);
    keypad(win, true);
    nodelay(win, true);
    srand(time(NULL)); // seed the rng with current time

    Shape ship;
    ship.vertex_count = 3;
    ship.vertices = malloc(ship.vertex_count * sizeof(Point));
    ship.vertices[0] = (Point){0, -8};
    ship.vertices[1] = (Point){3, 2};
    ship.vertices[2] = (Point){-3, 2};
    ship.center = (Point){0, 0};
    ship.facing = (Vector){0, -1};

    int x_start = X_SIZE / 2 / 2;
    int y_start = Y_SIZE / 2;
    translate(&ship, (float) x_start, (float) y_start);
    float dx = 0;
    float dy = 0;
    float angular_vel = 0;
    bool playing = true;

    while (playing) {
        int pressed = wgetch(win);

        if (pressed != ERR) {
            if (pressed == KEY_LEFT){
                if (angular_vel > - ANGULAR_LIMIT) angular_vel -= 0.01;
            }
            if (pressed == KEY_RIGHT) {
                if (angular_vel < ANGULAR_LIMIT) angular_vel += 0.01;
            }
            if (pressed == KEY_UP){
                dx += ship.facing.x * SPEED_FACTOR;
                dy += ship.facing.y * SPEED_FACTOR;
            }
        }

        float current_x = ship.center.x;
        float current_y = ship.center.y;
        translate(&ship, -current_x, -current_y);
        rotate(&ship, angular_vel);
        translate(&ship, current_x + dx, current_y + dy);

        erase();
        draw_shape(&ship);
        draw_borders(0, 0, Y_SIZE, X_SIZE);
        refresh();
        if (check_loose(&ship) == true){
            playing = false;
        }
        usleep(1000000 / FPS);
    }

    while (true){}
    endwin();

    return 0;
}