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

	void setStart(const Point& p) {
		p2 = p + asPoint();
		p1 = p;
	}

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
		return Vect(p1 / length(), p2 / length());
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
		os << "Vector(" << v.p1 << "," << v.p2 << ")";
		return os;
	}

	// https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
	static Vect rodriges(Vect k, Vect v, double theta) {
		k = k.unit();
		Vect r1 = v * cos(theta);
		Vect r2 = (k ^ v) * sin(theta);
		Vect r3 = k * (k * v) * (1 - cos(theta));
		return r1 + r2 + r3;
	}

	// https://en.wikipedia.org/wiki/Rotation_matrix
	static Vect rotate(Vect u, Vect v, double theta) {
		u = u.unit();
		Vect r1 = u * (u * v);
		Vect r2 = ((u ^ v) * cos(theta)) ^ u;
		Vect r3 = (u ^ v) * sin(theta);
		return r1 + r2 + r3;
	}
};

#endif // !VECT_H
