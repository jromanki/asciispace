#ifndef DRAW_SHAPE_H
#define DRAW_SHAPE_H

char opacity(float value);
void draw_line_aliased(float y0 ,float x0, float y1, float x1);
void draw_shape(Shape* s);

#endif
