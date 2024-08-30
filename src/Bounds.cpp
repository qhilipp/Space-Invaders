#include "Point.h"
#include "Bounds.h"

Bounds::Bounds(Point position, Point size): position(position), size(size) {}
Bounds::Bounds(double x, double y, double width, double height): position(Point(x, y)), size(Point(width, height)) {}

bool Bounds::collides(Bounds other) {
    return ((position.x >= other.position.x && position.x <= other.position.x + other.size.x) || (other.position.x >= position.x && other.position.x <= position.x + size.x)) && 
     ((position.y >= other.position.y && position.y <= other.position.y + other.size.y) || (other.position.y >= position.y && other.position.y <= position.y + size.y));
}

bool Bounds::contains(Bounds other) {
    return other.position.x >= position.x && other.position.x + other.size.x <= position.x + size.x && other.position.y >= position.y && other.position.y + other.size.y <= position.y + size.y;
}