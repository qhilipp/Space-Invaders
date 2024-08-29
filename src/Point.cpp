#include <math.h>
#include "Point.h"

Point::Point(): x(0), y(0) {}
Point::Point(double x, double y) : x(x), y(y) {}

double Point::length() {
    return sqrt(x * x + y * y);
}

Point Point::operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
}

Point Point::operator*(const Point& other) const {
    return Point(x * other.x, y * other.y);
}

Point Point::operator*(double f) const {
    return Point(x * f, y * f);
}

Point Point::unit() {
    double l = length();
    if (l == 0) return Point(0, 0);
    return Point(x / l, y / l);
}

Point operator*(double f, const Point& p) {
    return Point(p.x * f, p.y * f);
}