#ifndef PLANE_H
#define PLANE_H

#include <ostream>
#include "triangle.h"
#include "vect.h"
#include <vector>

class Plane {
public:
	Point* points;
	int numPoints;
	std::vector<Triangle> triangles;
	std::string name;

	Plane() {
		points = NULL;
		numPoints = 0;
		name = "none";
	}

	Plane(const Point* p, int n, int nt) {
		numPoints = n;
		points = new Point[n];
		for (int i = 0; i < n; i++) {
			points[i] = p[i];
		}
		genTriangles(nt);
	}

	int possibleTps(int n) {
		int tps = 1;
		while (2 * tps * tps < n) {
			tps++;
		}
		return tps;
	}

	void genTriangles(int nt) {
		if (nt <= 0 || nt % 2 != 0)
			return;

		int trianglesPerSide = possibleTps(nt);

		Point p = points[3] - points[0];
		double deltaX = p.x / trianglesPerSide;
		double deltaY = p.y / trianglesPerSide;
		double deltaZ = p.z / trianglesPerSide;

		for (int i = 0; i < trianglesPerSide; i++) {
			for (int j = 0; j < trianglesPerSide; j++) {
				if (points[0].z == points[3].z) {
					Point p1 = { points[0].x + i * deltaX, points[0].y + j * deltaY, points[0].z };
					Point p2 = { points[0].x + (i + 1) * deltaX, points[0].y + j * deltaY, points[0].z };
					Point p3 = { points[0].x + i * deltaX, points[0].y + (j + 1) * deltaY, points[0].z };
					triangles.push_back(Triangle(p1, p2, p3));

					p1 = { points[0].x + (i + 1) * deltaX, points[0].y + j * deltaY, points[0].z };
					p2 = { points[0].x + (i + 1) * deltaX, points[0].y + (j + 1) * deltaY, points[0].z };
					p3 = { points[0].x + i * deltaX, points[0].y + (j + 1) * deltaY, points[0].z };
					triangles.push_back(Triangle(p1, p2, p3));
				}
				else if (points[0].y == points[3].y) {
					Point p1 = { points[0].x + i * deltaX, points[0].y, points[0].z + j * deltaZ };
					Point p2 = { points[0].x + (i + 1) * deltaX, points[0].y, points[0].z + j * deltaZ };
					Point p3 = { points[0].x + i * deltaX, points[0].y, points[0].z + (j + 1) * deltaZ };
					triangles.push_back(Triangle(p1, p2, p3));

					p1 = { points[0].x + (i + 1) * deltaX, points[0].y, points[0].z + j * deltaZ };
					p2 = { points[0].x + (i + 1) * deltaX, points[0].y, points[0].z + (j + 1) * deltaZ };
					p3 = { points[0].x + i * deltaX, points[0].y, points[0].z + (j + 1) * deltaZ };
					triangles.push_back(Triangle(p1, p2, p3));
				}
				else if (points[0].x == points[3].x) {
					Point p1 = { points[0].x, points[0].y + i * deltaY, points[0].z + j * deltaZ };
					Point p2 = { points[0].x, points[0].y + (i + 1) * deltaY, points[0].z + j * deltaZ };
					Point p3 = { points[0].x, points[0].y + i * deltaY, points[0].z + (j + 1) * deltaZ };
					triangles.push_back(Triangle(p1, p2, p3));

					p1 = { points[0].x, points[0].y + (i + 1) * deltaY, points[0].z + j * deltaZ };
					p2 = { points[0].x, points[0].y + (i + 1) * deltaY, points[0].z + (j + 1) * deltaZ };
					p3 = { points[0].x, points[0].y + i * deltaY, points[0].z + (j + 1) * deltaZ };
					triangles.push_back(Triangle(p1, p2, p3));
				}
			}
		}
	}


	int numTriangles() {
		return triangles.size();
	}

	Vect getNormal() {
		Vect u = { points[1], points[0] };
		Vect v = { points[1], points[3] };
		return (u ^ v).unit();
	}

	Vect reflect(const Vect& v) {
		Vect n = getNormal();
		return v - n * (2 * (v * n));
	}

	Point incidence(const Point& p, const Vect& i) {
		Vect n = getNormal();
		Vect u = { p, points[0] };
		double dist = u * n / (i * n);
		Point pi = p + i.asPoint() * dist;
		return pi;
	}

	bool hasSurpassed(const Point& p) {
		Vect u = { points[0], p };
		return u * getNormal() < 0;
	}

	double distance(const Point& p) {
		Vect u = { points[0], p };
		return u * getNormal();
	}
};

#endif // !PLANE_H
