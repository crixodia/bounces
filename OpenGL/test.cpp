//#include <iostream>
//#include "plane.h"
//#include "room.h"
//#include "vect.h"
//
//using namespace std;
//
//int main() {
//	/*Point p1 = { 0,0,0 };
//	Point p2 = { 1,0,0 };
//	Point p3 = { 0,1,0 };
//	Point p4 = { 1,1,0 };
//
//	int nt = 32;
//
//	Point* points = new Point[nt];
//	points[0] = p1;
//	points[1] = p2;
//	points[2] = p3;
//	points[3] = p4;
//
//	Plane p = Plane(points, 4, nt);
//
//	for (int i = 0; i < nt; i++) {
//		cout << p.triangles[i] << endl;
//	}*/
//
//	Vect G = Vect({ -1, -1, 1 });
//	Vect A = Vect({ 1, 1, 1 });
//	Vect F = Vect({ 1, -1, 1 });
//
//	Vect X = (F - G) ^ (F - A);
//	std::cout << F - G << std::endl;
//	std::cout << F - A << std::endl;
//	std::cout << X.unit() << std::endl;
//
//	std::cout << X.unit() * Vect({ 0, 0, 2 }) << std::endl;
//}