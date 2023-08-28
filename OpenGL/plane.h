#ifndef PLANE_H
#define PLANE_H

#include <ostream>
#include "triangle.h"
#include "vect.h"
#include <vector>

/**
 * @brief Clase plano que contiene un arreglo de puntos y un arreglo de triangulos
 */
class Plane {
public:
	Point* points;						/* Puntos del plano */
	int numPoints;						/* Numero de puntos del plano */
	std::vector<Triangle> triangles;	/* Triangulos del plano */
	std::string name;					/* Nombre del plano */

	/**
	 * @brief Constructor por defecto
	 */
	Plane() {
		points = NULL;
		numPoints = 0;
		name = "none";
	}

	/**
	 * @brief Constructor con parametros
	 * @param p Arreglo de puntos
	 * @param n Numero de puntos
	 * @param nt Numero de triangulos
	 */
	Plane(const Point* p, int n, int nt) {
		numPoints = n;
		points = new Point[n];
		for (int i = 0; i < n; i++) {
			points[i] = p[i];
		}
		genTriangles(nt);
	}

	/**
	 * @brief Obtiene el posible número de triángulos que un plano puede tener en base al número de triángulos por cara especificado.
	 * @param n Número de triángulos por cara
	 * @return Número de triángulos que el plano puede tener
	 */
	int possibleTps(int n) {
		int tps = 1;
		while (2 * tps * tps < n) {
			tps++;
		}
		return tps;
	}

	/**
	 * @brief Genera los triángulos del plano
	 * @param nt Número de triángulos por cara
	 */
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
					Triangle T = Triangle(p1, p2, p3);
					//T.planeNormal = getNormal();
					triangles.push_back(T);

					p1 = { points[0].x + (i + 1) * deltaX, points[0].y + j * deltaY, points[0].z };
					p2 = { points[0].x + (i + 1) * deltaX, points[0].y + (j + 1) * deltaY, points[0].z };
					p3 = { points[0].x + i * deltaX, points[0].y + (j + 1) * deltaY, points[0].z };
					T = Triangle(p1, p2, p3);
					//T.planeNormal = getNormal();
					triangles.push_back(T);
				}
				else if (points[0].y == points[3].y) {
					Point p1 = { points[0].x + i * deltaX, points[0].y, points[0].z + j * deltaZ };
					Point p2 = { points[0].x + (i + 1) * deltaX, points[0].y, points[0].z + j * deltaZ };
					Point p3 = { points[0].x + i * deltaX, points[0].y, points[0].z + (j + 1) * deltaZ };
					Triangle T = Triangle(p1, p2, p3);
					//T.planeNormal = getNormal();
					triangles.push_back(T);

					p1 = { points[0].x + (i + 1) * deltaX, points[0].y, points[0].z + j * deltaZ };
					p2 = { points[0].x + (i + 1) * deltaX, points[0].y, points[0].z + (j + 1) * deltaZ };
					p3 = { points[0].x + i * deltaX, points[0].y, points[0].z + (j + 1) * deltaZ };
					T = Triangle(p1, p2, p3);
					//T.planeNormal = getNormal();
					triangles.push_back(T);
				}
				else if (points[0].x == points[3].x) {
					Point p1 = { points[0].x, points[0].y + i * deltaY, points[0].z + j * deltaZ };
					Point p2 = { points[0].x, points[0].y + (i + 1) * deltaY, points[0].z + j * deltaZ };
					Point p3 = { points[0].x, points[0].y + i * deltaY, points[0].z + (j + 1) * deltaZ };
					Triangle T = Triangle(p1, p2, p3);
					//T.planeNormal = getNormal();
					triangles.push_back(T);

					p1 = { points[0].x, points[0].y + (i + 1) * deltaY, points[0].z + j * deltaZ };
					p2 = { points[0].x, points[0].y + (i + 1) * deltaY, points[0].z + (j + 1) * deltaZ };
					p3 = { points[0].x, points[0].y + i * deltaY, points[0].z + (j + 1) * deltaZ };
					T = Triangle(p1, p2, p3);
					//T.planeNormal = getNormal();
					triangles.push_back(T);
				}
			}
		}
	}

	/**
	 * @brief Devuelve el número de triángulos que tiene el plano
	 * @return Número de triángulos que tiene el plano
	 */
	int numTriangles() {
		return triangles.size();
	}

	/**
	 * @brief Devuelve vector normal del plano
	 * @return Vector normal del plano
	 */
	Vect getNormal() {
		Vect u = { points[1], points[0] };
		Vect v = { points[1], points[3] };
		return (u ^ v).unit();
	}

	/**
	 * @brief Devuelve vector reflejo de un vector respecto al plano
	 * @param v Vector a reflejar
	 * @return Vector reflejo de v respecto al plano
	 */
	Vect reflect(const Vect& v) {
		Vect n = getNormal();
		return v - n * (2 * (v * n));
	}

	/**
	 * @brief Devuelve el punto de incidencia de una partícula con el plano
	 * @param p Punto de origen de la partícula
	 * @param i Vector director de la partícula
	 * @return Punto de incidencia la partícula con el plano
	 */
	Point incidence(const Point& p, const Vect& i) {
		Vect n = getNormal();
		Vect u = { p, points[0] };
		double dist = u * n / (i * n);
		Point pi = p + i.asPoint() * dist;
		return pi;
	}

	/**
	 * @brief Devuelve si un punto ha superado el plano
	 * @param p Punto a comprobar
	 * @return true si el punto ha superado el plano, false en caso contrario
	 */
	bool hasSurpassed(const Point& p) {
		Vect u = { points[0], p };
		return u * getNormal() < 0;
	}

	/**
	 * @brief Devuelve la distancia de un punto al plano
	 * @param p Punto a comprobar
	 * @return Distancia de p al plano
	 */
	double distance(const Point& p) {
		Vect u = { points[0], p };
		return u * getNormal();
	}
};

#endif // !PLANE_H
