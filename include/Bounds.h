#pragma once

#include "Point.h"

struct Bounds {
public:
    Point position, size;

    Bounds(Point position, Point size);
    Bounds(double x, double y, double width, double height);

    bool collides(Bounds other);
    bool contains(Bounds other);
};