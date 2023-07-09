#ifndef POINT_H
#define POINT_H

#include <ostream>

class Point {
public:
	double x;
	double y;
	double z;

	Point() {
		x = 0;
		y = 0;
		z = 0;
	}

	Point(double xx, double yy, double zz) {
		x = xx;
		y = yy;
		z = zz;
	}

	Point(const Point& p) {
		x = p.x;
		y = p.y;
		z = p.z;
	}

	int operator==(const Point& p) const {
		return x == p.x && y == p.y && z == p.z ? 1 : 0;
	}

	int operator!=(const Point& p) const {
		return x != p.x || y != p.y || z != p.z ? 1 : 0;
	}

	Point& operator=(const Point& p) {
		x = p.x;
		y = p.y;
		z = p.z;
		return *this;
	}

	Point operator+(const Point& p) const {
		return Point(x + p.x, y + p.y, z + p.z);
	}

	Point operator-(const Point& p) const {
		return Point(x - p.x, y - p.y, z - p.z);
	}

	Point operator-() const {
		return Point(-x, -y, -z);
	}

	Point operator*(double f) const {
		return Point(x * f, y * f, z * f);
	}

	Point operator/(double f) const {
		return Point(x / f, y / f, z / f);
	}

	friend std::ostream& operator<<(std::ostream& os, const Point& p) {
		os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
		return os;
	}
};

#endif // !POINT_H