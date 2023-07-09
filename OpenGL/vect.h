#ifndef VECT_H
#define VECT_H

#include <ostream>
#include "point.h"

class Vect {
public:
	Point p1;
	Point p2;

	Vect() {
		p1 = Point();
		p2 = Point();
	}

	Vect(const Point& p) {
		p1 = Point();
		p2 = p;
	}

	Vect(const Point& pp1, const Point& pp2) {
		p1 = pp1;
		p2 = pp2;
	}

	Vect(const Vect& v) {
		p1 = v.p1;
		p2 = v.p2;
	}

	double getI() const { return p2.x - p1.x; }
	double getJ() const { return p2.y - p1.y; }
	double getK() const { return p2.z - p1.z; }

	void setP1(const Point& pp1) { p1 = pp1; }
	void setP2(const Point& pp2) { p2 = pp2; }

	int operator==(const Vect& v) const {
		return p1 == v.p1 && p2 == v.p2 ? 1 : 0;
	}

	int operator!=(const Vect& v) const {
		return p1 != v.p1 || p2 != v.p2 ? 1 : 0;
	}

	Vect& operator=(const Vect& v) {
		p1 = v.p1;
		p2 = v.p2;
		return *this;
	}

	Vect operator+(const Vect& v) const {
		return Vect(p1 + v.p1, p2 + v.p2);
	}

	Vect operator-(const Vect& v) const {
		return Vect(p1 - v.p1, p2 - v.p2);
	}

	Vect operator-() const {
		return Vect(p2, p1);
	}

	Vect operator*(double f) const {
		return Vect(p1 * f, p2 * f);
	}

	Vect operator/(double f) const {
		return Vect(p1 / f, p2 / f);
	}

	double operator*(const Vect& v) const {
		Point u = asPoint();
		Point V = v.asPoint();
		double dotx = u.x * V.x;
		double doty = u.y * V.y;
		double dotz = u.z * V.z;
		return dotx + doty + dotz;
	}

	Vect operator^(const Vect& v) const {
		Point u = asPoint();
		Point V = v.asPoint();
		double crossx = u.y * V.z - u.z * V.y;
		double crossy = u.z * V.x - u.x * V.z;
		double crossz = u.x * V.y - u.y * V.x;
		return Vect(Point(crossx, crossy, crossz));
	}

	Point asPoint() const {
		return p2 - p1;
	}

	double length() const {
		return sqrt(pow(getI(), 2) + pow(getJ(), 2) + pow(getK(), 2));
	}

	Vect unit() const {
		return Vect(p1, p2) / length();
	}

	double angle(const Vect& v) const {
		return acos((*this * v) / (length() * v.length()));
	}

	Vect proy(const Vect& v) const {
		return v.unit() * ((*this * v) / v.length());
	}

	Vect norm(const Vect& v) const {
		return *this - proy(v);
	}

	friend std::ostream& operator<<(std::ostream& os, const Vect& v) {
		os << "Vect: " << v.p1 << " -> " << v.p2;
		return os;
	}
};

#endif // !VECT_H
