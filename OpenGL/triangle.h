#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <ostream>
#include "point.h"
#include "vect.h"

class Triangle {
private:
	Point a;
	Point b;
	Point c;
public:

	Triangle() {
		a = Point();
		b = Point();
		c = Point();
	}

	Triangle(const Point& aa, const Point& bb, const Point& cc) {
		a = aa;
		b = bb;
		c = cc;
	}

	Triangle(const Triangle& t) {
		a = t.getA();
		b = t.getB();
		c = t.getC();
	}

	Point getA() const { return a; }
	Point getB() const { return b; }
	Point getC() const { return c; }

	void setA(const Point& aa) { a = aa; }
	void setB(const Point& bb) { b = bb; }
	void setC(const Point& cc) { c = cc; }

	int operator==(const Triangle& t) const {
		return a == t.getA() && b == t.getB() && c == t.getC() ? 1 : 0;
	}

	int operator!=(const Triangle& t) const {
		return a != t.getA() || b != t.getB() || c != t.getC() ? 1 : 0;
	}

	Triangle& operator=(const Triangle& t) {
		a = t.getA();
		b = t.getB();
		c = t.getC();
		return *this;
	}

	Vect getNormal() const {
		Vect v1(a, b);
		Vect v2(a, c);
		return v1 ^ v2;
	}

	Point getBarycenter() const {
		return (a + b + c) / 3.0;
	}

	int isInside(const Point& p) const {
		Vect v1(a, b);
		Vect v2(a, c);
		Vect v3(a, p);
		Vect n = v1 ^ v2;
		double d = n * v3;
		return d > 0 ? 1 : 0;
	}

	double surface() const {
		Vect v1(a, b);
		Vect v2(a, c);
		Vect n = v1 ^ v2;
		return n.length() / 2.0;
	}

	double* flatArray() const {
		double* res = new double[9];
		res[0] = a.x;
		res[1] = a.y;
		res[2] = a.z;
		res[3] = b.x;
		res[4] = b.y;
		res[5] = b.z;
		res[6] = c.x;
		res[7] = c.y;
		res[8] = c.z;
		return res;
	}

	friend std::ostream& operator<<(std::ostream& os, const Triangle& t) {
		//os << "Triangle: " << t.getA() << ", " << t.getB() << ", " << t.getC();
		os << t.getA() << "\n" << t.getB() << "\n" << t.getC();
		return os;
	}
};

#endif // !TRIANGLE_H
