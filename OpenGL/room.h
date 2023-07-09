#ifndef ROOM_H
#define ROOM_H

#include <ostream>
#include "plane.h"

class Room {
public:
	Plane floor;
	Plane ceiling;
	Plane leftWall;
	Plane rightWall;
	Plane frontWall;
	Plane backWall;

	int numTriangles;

	Room(int nt) {
		numTriangles = nt;

		Point* floorPoints = new Point[4]{ {-1, -1, 1}, {-1, -1, -1}, {1, -1, 1} ,{1, -1, -1} };
		floor = Plane(floorPoints, 4, numTriangles);
		floor.name = "floor";

		Point* ceilingPoints = new Point[4]{ {-1, 1, 1}, {1, 1, 1}, {-1, 1, -1}, {1, 1, -1} };
		ceiling = { ceilingPoints, 4, numTriangles };
		ceiling.name = "ceiling";

		Point* leftWallPoints = new Point[4]{ {-1, -1, 1}, {-1, 1, 1}, {-1, -1, -1}, {-1, 1, -1} };
		leftWall = { leftWallPoints, 4, numTriangles };
		leftWall.name = "leftWall";

		Point* rightWallPoints = new Point[4]{ {1, -1, 1}, {1, -1, -1}, {1, 1, 1}, {1, 1, -1} };
		rightWall = { rightWallPoints, 4, numTriangles }; \
			rightWall.name = "rightWall";

		Point* frontWallPoints = new Point[4]{ {-1, -1, 1},{1, -1, 1},{-1, 1, 1}, {1, 1, 1} };
		frontWall = { frontWallPoints, 4, numTriangles };
		frontWall.name = "frontWall";

		Point* backWallPoints = new Point[4]{ {-1, -1, -1}, {-1, 1, -1}, {1, -1, -1}, {1, 1, -1} };
		backWall = { backWallPoints, 4, numTriangles };
		backWall.name = "backWall";
	}

	Vect* getNormals() {
		Vect* normals = new Vect[6]{
			floor.getNormal(),
			ceiling.getNormal(),
			leftWall.getNormal(),
			rightWall.getNormal(),
			frontWall.getNormal(),
			backWall.getNormal()
		};
		return normals;
	}

	Plane* SurpassedPlane(const Point& p) {
		if (floor.hasSurpassed(p)) {
			return &floor;
		}
		else if (ceiling.hasSurpassed(p)) {
			return &ceiling;
		}
		else if (leftWall.hasSurpassed(p)) {
			return &leftWall;
		}
		else if (rightWall.hasSurpassed(p)) {
			return &rightWall;
		}
		else if (frontWall.hasSurpassed(p)) {
			return &frontWall;
		}
		else if (backWall.hasSurpassed(p)) {
			return &backWall;
		}
		else {
			return nullptr;
		}
	}

	Plane* nearestSurpassedPlane(const Point& p, const Vect& incidence) {
		Plane* nearest = &floor;
		if (SurpassedPlane(p) == nullptr) {
			return nullptr;
		}
		double minDistance = floor.distance(p);
		double distance = ceiling.distance(p);
		if (distance < minDistance) {
			minDistance = distance;
			nearest = &ceiling;
		}
		distance = leftWall.distance(p);
		if (distance < minDistance) {
			minDistance = distance;
			nearest = &leftWall;
		}
		distance = rightWall.distance(p);
		if (distance < minDistance) {
			minDistance = distance;
			nearest = &rightWall;
		}
		distance = frontWall.distance(p);
		if (distance < minDistance) {
			minDistance = distance;
			nearest = &frontWall;
		}
		distance = backWall.distance(p);
		if (distance < minDistance) {
			minDistance = distance;
			nearest = &backWall;
		}
		return nearest;
	}

};

#endif // !ROOM_H
