#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "shape.h"
#include "draw_misc.h"
#include "draw_shapes.h"
#include "movement.h"

#define FPS 60
#define SPEED_FACTOR 0.14f
#define ANGULAR_LIMIT 0.1f

bool check_border_collision(Shape* s, int x_size, int y_size) {
    for (int i = 0; i < s->vertex_count; i++){
        if (s->vertices[i].x <= 0 || s->vertices[i].x >= x_size/2){
            return true;
        } else if (s->vertices[i].y <= 0 || s->vertices[i].y >= y_size){
            return true;
        }
    }
    return false;
}

int main() {
    WINDOW* win = initscr();
    int y_size; int x_size;
    getmaxyx(win, y_size, x_size);
    x_size = x_size - 1; y_size = y_size - 1;
    curs_set(0);
    keypad(win, true);
    nodelay(win, true);
    srand(time(NULL)); // seed the rng with current time
    int x_start = x_size / 2 / 2;
    int y_start = y_size / 2;

    while(true){
        Shape ship;
        ship.vertex_count = 3;
        ship.vertices = malloc(ship.vertex_count * sizeof(Point));
        ship.vertices[0] = (Point){0, -8};
        ship.vertices[1] = (Point){3, 2};
        ship.vertices[2] = (Point){-3, 2};
        ship.center = (Point){0, 0};
        ship.facing = (Vector){0, -1};

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
                if (pressed == 'q'){
                    playing = false;
                }
            }

            float current_x = ship.center.x;
            float current_y = ship.center.y;
            translate(&ship, -current_x, -current_y);
            rotate(&ship, angular_vel);
            translate(&ship, current_x + dx, current_y + dy);

            erase();
            draw_shape(&ship);
            draw_borders(0, 0, y_size, x_size);
            
            refresh();

            if (check_border_collision(&ship, x_size, y_size) == true){
                playing = false;
            }

            usleep(1000000 / FPS);
        }

        you_lost_text(x_size, y_size);
        while (true){
            int pressed = wgetch(win);
            if (pressed != ERR){
                break;
            }
        }

        free(ship.vertices);
    }

    endwin();

    return 0;
}