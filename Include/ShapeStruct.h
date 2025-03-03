#pragma once

#include <GL/glew.h>
#include <math.h>

typedef struct {
    float x, y, width, height;
    float r, g, b;
} Rect;

typedef struct {
    float cx, cy, radius;
    float r, g, b;
} Circle;

typedef struct {
    float x, y, width;
    float r, g, b;
} Bar;

typedef struct {
    float x1, y1, x2, y2, x3, y3;
    float r, g, b;
} Triangle;

typedef struct {
    float x, y, width, height;
    float r, g, b;
} Arrow;
