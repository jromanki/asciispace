#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#define Y_SIZE 46
#define X_SIZE 180

// const char GRAYSCALE[] = {
//     '$', 'B', 'P', '%', '#', 'z', 'c', 'r', '<',
//     '*', '=', '+', '~', ':', ',', '`', '\0'
// };

typedef struct {
    float x;
    float y;
} Point;

typedef struct {
    Point* vertices;
    int vertex_count;
    Point center;
} Shape;

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

void draw_borders(int y_0 ,int x_0, int y_max, int x_max) {
    draw_line_pixels(y_max, x_0, y_0, x_0, '|');       // left
    draw_line_pixels(y_0, x_max, y_max, x_max, '|');   // right
    draw_line_pixels(y_0, x_0, y_0, x_max, '-');       // top
    draw_line_pixels(y_max, x_max, y_max, x_0, '-');   // bottom
    
}

Shape translate(Shape s, float dx, float dy){
    for (int i = 0; i < s.vertex_count; i++){
        s.vertices[i].x += dx;
        s.vertices[i].y += dy;
    }
    s.center.x += dx;
    s.center.y += dy;
    return s;
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
    ship.vertices[0] = (Point){0, -10};
    ship.vertices[1] = (Point){12, 5};
    ship.vertices[2] = (Point){-12, 5};
    ship.center = (Point){0, 0};

    
    int a_y = rand() % Y_SIZE;
    int a_x = rand() % X_SIZE;

    int b_y = rand() % Y_SIZE;
    int b_x = rand() % X_SIZE;

    ship = translate(ship, (float) a_x, (float) a_y);

    while (true) {
        erase();

        // draw_line_pixels(a_y, a_x, b_y, b_x, '*');
        // draw_line_aliased(a_y, a_x, b_y, b_x);
        draw_line_aliased(ship.vertices[0].y, ship.vertices[0].x,
            ship.vertices[1].y,ship.vertices[1].x);
        draw_line_aliased(ship.vertices[1].y, ship.vertices[1].x,
            ship.vertices[2].y,ship.vertices[2].x);
        draw_line_aliased(ship.vertices[2].y, ship.vertices[2].x,
            ship.vertices[0].y,ship.vertices[0].x);
        // mvaddch(a_y, a_x, '&');
        // mvaddch(b_y, b_x, '&');
        draw_borders(0, 0, Y_SIZE, X_SIZE);

        b_x += 1;
        refresh();
        usleep(50000);
    }

    endwin();

    return 0;
}