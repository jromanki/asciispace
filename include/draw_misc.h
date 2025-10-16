#ifndef DRAW_MISC_H
#define DRAW_MISC_H

void draw_line_pixels(int y0 ,int x0, int y1, int x1, char sign);
void draw_borders(int y_0 ,int x_0, int y_max, int x_max);
void you_lost_text(int x_size, int y_size);
Point* stars_init(int x_size, int y_size, int star_amount);
void draw_stars(Point* star_arr, int star_amount);

#endif