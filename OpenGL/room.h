#ifndef ROOM_H
#define ROOM_H

#include <ostream>
#include "plane.h"

class Room {
public:
	Plane* planes;
	int numPlanes;
	int numTriangles;

	Room(int nt, int np) {
		numTriangles = nt;
		numPlanes = np;
		planes = new Plane[numPlanes];

		switch (numPlanes) {
		case 6:
			genCube();
			break;
		}
	}

	void genCube() {
		Point* floorPoints = new Point[4]{ {-1, -1, 1}, {-1, -1, -1}, {1, -1, 1} ,{1, -1, -1} };
		Plane floor = Plane(floorPoints, 4, numTriangles);
		floor.name = "floor";

		Point* ceilingPoints = new Point[4]{ {-1, 1, 1}, {1, 1, 1}, {-1, 1, -1}, {1, 1, -1} };
		Plane ceiling = { ceilingPoints, 4, numTriangles };
		ceiling.name = "ceiling";

		Point* leftWallPoints = new Point[4]{ {-1, -1, 1}, {-1, 1, 1}, {-1, -1, -1}, {-1, 1, -1} };
		Plane leftWall = { leftWallPoints, 4, numTriangles };
		leftWall.name = "leftWall";

		Point* rightWallPoints = new Point[4]{ {1, -1, 1}, {1, -1, -1}, {1, 1, 1}, {1, 1, -1} };
		Plane rightWall = { rightWallPoints, 4, numTriangles };
		rightWall.name = "rightWall";

		Point* frontWallPoints = new Point[4]{ {-1, -1, 1},{1, -1, 1},{-1, 1, 1}, {1, 1, 1} };
		Plane frontWall = { frontWallPoints, 4, numTriangles };
		frontWall.name = "frontWall";

		Point* backWallPoints = new Point[4]{ {-1, -1, -1}, {-1, 1, -1}, {1, -1, -1}, {1, 1, -1} };
		Plane backWall = { backWallPoints, 4, numTriangles };
		backWall.name = "backWall";

		planes[0] = floor;
		planes[1] = ceiling;
		planes[2] = leftWall;
		planes[3] = rightWall;
		planes[4] = frontWall;
		planes[5] = backWall;

	}

	Vect* getNormals() {
		Vect* normals = new Vect[numPlanes];
		for (int i = 0; i < numPlanes; i++) {
			normals[i] = planes[i].getNormal();
		}
		return normals;
	}

	double** getAllVertices() {
		int tnt = numTriangles * numPlanes;
		double** vertices = new double* [tnt];

		int k = 0;
		for (int i = 0; i < tnt; i++) {
			double* flat = planes[k].triangles[i % numTriangles].flatArray();
			vertices[i] = new double[9];
			for (int j = 0; j < 9; j++) {
				vertices[i][j] = flat[j];
			}
			if (i % numTriangles == numTriangles - 1) {
				k++;
			}
		}

		return vertices;
	}


	Plane* SurpassedPlane(const Point& p) {
		for (int i = 0; i < numPlanes; i++) {
			if (planes[i].hasSurpassed(p)) {
				return &planes[i];
			}
		}
		return nullptr;
	}

	Plane* nearestSurpassedPlane(const Point& p, const Vect& incidence) {
		Plane* nearest = &planes[0];

		if (SurpassedPlane(p) == nullptr) {
			return nullptr;
		}

		double minDistance = nearest->distance(p);
		double distance = nearest->distance(p);

		for (int i = 0; i < numPlanes; i++) {
			distance = planes[i].distance(p);
			if (distance < minDistance) {
				minDistance = distance;
				nearest = &planes[i];
			}
		}
		return nearest;
	}

};

#endif // !ROOM_H
