#ifndef SHAPE_H
#define SHAPE_H

typedef struct {
    float x;
    float y;
} Point;

typedef struct {
    float x;
    float y;
} Vector;

typedef struct {
    Point* vertices;
    int vertex_count;
    Point center;
    Vector facing;
} Shape;

#endif