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
#define SPEED_FACTOR 0.07f
#define ANGULAR_FACTOR 0.01f
#define ANGULAR_LIMIT 0.1f
#define ANG_VAR_GAIN 0.05f
#define KEY_DELAY 0.2
#define SHIP_SCALE 1.2f
#define STAR_AMOUNT 20

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

void debug_print_int(int var){
    erase();
    printw("current time: %d\n", var);
    refresh();              // Update the actual screen
    while(1){
        if (getch() == 'q'){
                break;
        }
    }
}

float random_float(void) {
    float r = (float)rand() / (float)RAND_MAX;
    return r;
}

float get_angualar_variation(void){
    float r = random_float();
    return (r - 0.5) * ANGULAR_LIMIT * ANG_VAR_GAIN;
}
static double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
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
        ship.vertices[0] = (Point){0, -6};
        ship.vertices[1] = (Point){2, 1};
        ship.vertices[2] = (Point){-2, 1};
        scale_shape(&ship, SHIP_SCALE);
        ship.center = (Point){0, 0};
        ship.facing = (Vector){0, -1};
        Point* stars_arr_ptr = stars_init(x_size, y_size, STAR_AMOUNT);

        translate(&ship, (float) x_start, (float) y_start);
        float dx = 0;
        float dy = 0;
        float angular_vel = 0;
        bool playing = true;
        double last_press = now_seconds();

        while (playing) {
            double current_time = now_seconds();
            int pressed = wgetch(win);

            if (pressed != ERR) {
                if ((now_seconds() - last_press) > KEY_DELAY) {
                    last_press = current_time;
                    if (pressed == KEY_LEFT){
                        if (angular_vel > - ANGULAR_LIMIT) angular_vel -= ANGULAR_FACTOR + get_angualar_variation();
                    }
                    if (pressed == KEY_RIGHT) {
                        if (angular_vel < ANGULAR_LIMIT) angular_vel += ANGULAR_FACTOR + get_angualar_variation();
                    }
                    if (pressed == KEY_UP){
                        dx += ship.facing.x * SPEED_FACTOR;
                        dy += ship.facing.y * SPEED_FACTOR;
                    }
                    if (pressed == 'q'){
                        playing = false;
                    }
                }
            }

            float current_x = ship.center.x;
            float current_y = ship.center.y;
            translate(&ship, -current_x, -current_y);
            rotate(&ship, angular_vel);
            translate(&ship, current_x + dx, current_y + dy);

            erase();
            draw_stars(stars_arr_ptr, STAR_AMOUNT);
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