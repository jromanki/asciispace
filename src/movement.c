#include <math.h>
#include "shape.h"

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