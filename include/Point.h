#pragma once

struct Point {
public:
	double x, y;

	Point(double x, double y);
	Point();

    double length();
    Point operator+(const Point& other) const;
    Point operator*(const Point& other) const;
    Point operator*(double f) const;
    friend Point operator*(double f, const Point& p);
    Point unit();
};